/**
 * Copyright (C) 2016 helixeee
 *
 * This file is part of dbger.
 *
 * dbger is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dbger is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the
 * GNU General Public License
 * along with dbger.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "core/cmsis_dap.h"

#include <cstddef>

#include "core/dap_port.h"
#include "core/host_adapter.h"
#include "core/indicator.h"

#include "config/cxx.h"

namespace {

using dbger::CmsisDap;

int checkResponseStatus(bool condition) {
  if (condition) {
    return CmsisDap::kResponseStatusOk;
  } else {
    return CmsisDap::kResponseStatusError;
  }
}

template<typename SIZE>
int convertDataToResponse(SIZE data, uint8_t **response) {
  const int size = static_cast<int>(sizeof(data) / sizeof(**response));
  for (int i = 0; i < size; i++) {
    *((*response)++) = (data >> (i * 8));
  }
  return size;
}

int convertShortToResponse(uint16_t short_data, uint8_t **response) {
  return convertDataToResponse<uint16_t>(short_data, response);
}

int convertWordToResponse(uint32_t word_data, uint8_t **response) {
  return convertDataToResponse<uint32_t>(word_data, response);
}

template<typename SIZE>
SIZE convertCommandToData(const uint8_t **command) {
  SIZE data = 0;
  for (int i = 0;
       i < static_cast<int>(sizeof(data) / sizeof(**command));
       i++) {
    data |= *((*command)++) << (i * 8);
  }
  return data;
}

uint16_t convertCommandToShort(const uint8_t **command) {
  return convertCommandToData<uint16_t>(command);
}

uint32_t convertCommandToWord(const uint8_t **command) {
  return convertCommandToData<uint32_t>(command);
}

void buildTransferResponse(int response_transfer_count,
                           int transfer_response,
                           int response_data_count,
                           uint32_t *response_length,
                           uint8_t *response_count_lsb,
                           uint8_t *response_count_msb) {
  *response_count_msb++ = (response_transfer_count >> 8) & 0xff;
  *response_count_msb = transfer_response;
  *response_count_lsb = response_transfer_count & 0xff;
  *response_length +=
      (response_data_count *
       (sizeof(*response_length) / sizeof(*response_count_lsb)));
}

}  // end namespace

namespace dbger {

CmsisDap::CmsisDap()
    : indicator_(nullptr),
      swd_port_(nullptr),
      jtag_port_(nullptr),
      access_port_(nullptr),
      command_length_(0),
      command_(nullptr),
      response_length_(nullptr),
      response_(nullptr),
      host_adapter_(nullptr),
      transfer_wait_retry_(0),
      transfer_match_retry_(0),
      transfer_abort_(false) {
}

CmsisDap::~CmsisDap() {
}

int CmsisDap::command(uint32_t command_length,
                      const uint8_t command[],
                      uint32_t *response_length,
                      uint8_t response[]) {
  command_length_ = command_length;
  command_ = command;
  response_length_ = response_length;
  response_ = response;

  int result = kCmsisDapStatusError;
  switch (*response_++ = *command_++) {
    case kCommandIdDapInfo:
      result = commandDapInfo();
      break;
    case kCommandIdDapHostStatus:
      result = commandDapHostStatus();
      break;
    case kCommandIdDapConnect:
      result = commandDapConnect();
      break;
    case kCommandIdDapDisconnect:
      result = commandDapDisconnect();
      break;
    case kCommandIdDapWriteABORT:
      result = commandDapWriteABORT();
      break;
    case kCommandIdDapDelay:
      result = commandDapDelay();
      break;
    case kCommandIdDapResetTarget:
      result = commandDapResetTarget();
      break;
    case kCommandIdDapSwjPins:
      result = commandDapSwjPins();
      break;
    case kCommandIdDapSwjClock:
      result = commandDapSwjClock();
      break;
    case kCommandIdDapSwjSequence:
      result = commandDapSwjSequence();
      break;
    case kCommandIdDapSwdConfigure:
      result = commandDapSwdConfigure();
      break;
    case kCommandIdDapJtagSequence:
      result = commandDapJtagSequence();
      break;
    case kCommandIdDapJtagConfigure:
      result = commandDapJtagConfigure();
      break;
    case kCommandIdDapJtagIDCODE:
      result = commandDapJtagIDCODE();
      break;
    case kCommandIdDapTransferConfigure:
      result = commandDapTransferConfigure();
      break;
    case kCommandIdDapTransfer:
      result = commandDapTransfer();
      break;
    case kCommandIdDapTransferBlock:
      result = commandDapTransferBlock();
      break;
    default:
      result = commandDapVendorCommand();
  }

  command_length_ = 0;
  command_ = nullptr;
  response_length_ = nullptr;
  response_ = nullptr;

  return result;
}

bool CmsisDap::abortTransfer() {
  transfer_abort_ = true;
  return true;
}

HostAdapter* CmsisDap::attachHostAdapter(HostAdapter *host_adapter) {
  HostAdapter *old_host_adapter = host_adapter_;
  host_adapter_ = host_adapter;
  return old_host_adapter;
}

Indicator* CmsisDap::attachIndicator(Indicator *indicator) {
  Indicator *old_indicator = indicator_;
  indicator_ = indicator;
  return old_indicator;
}

DapPort* CmsisDap::attachSwdPort(DapPort *swd_port) {
  DapPort *old_swd_port = swd_port_;
  swd_port_ = swd_port;
  return old_swd_port;
}

DapPort* CmsisDap::attachJtagPort(DapPort *jtag_port) {
  DapPort *old_jtag_port = jtag_port_;
  jtag_port_ = jtag_port;
  return old_jtag_port;
}

int CmsisDap::commandDapVendorCommand() {
  *response_length_ = 1;
  return kCmsisDapStatusUndefinedCommandId;
}

uint32_t CmsisDap::copyInfoString(const char *info_string) {
  if (host_adapter_ == nullptr) {
    return 0;
  }

  const uint32_t max_packet_size = host_adapter_->getMaxPacketSize();
  if (max_packet_size <= 3) {
    return 0;
  }

  uint32_t position = 0;
  while (position++ < (max_packet_size - 3)) {
    if (*info_string == '\0') {
      break;
    }
    *response_++ = *info_string++;
  }

  if (position == 1) {
    return 0;
  }

  *response_ = '\0';

  return position;
}

uint32_t CmsisDap::getInfoVendorId() {
  return 0;
}

uint32_t CmsisDap::getInfoProductId() {
  return 0;
}

uint32_t CmsisDap::getInfoSerialNumber() {
  return 0;
}

uint32_t CmsisDap::getInfoFirmwareVersion() {
  const char firmware_version[] = "0.0.1";
  return copyInfoString(firmware_version);
}

uint32_t CmsisDap::getInfoTargetDeviceVendor() {
  return 0;
}

uint32_t CmsisDap::getInfoTargetDeviceName() {
  return 0;
}

int CmsisDap::connectDefaultPortOnDapConnect() {
  return connectPortOnDapConnect(&swd_port_, kDapPortModeSwd);
}

int CmsisDap::connectPortOnDapConnect(DapPort **dap_port,
                                      DapPortMode response_port) {
  CmsisDapStatus status = kCmsisDapStatusOk;
  if (*dap_port != nullptr) {
    access_port_ = *dap_port;
    if (!access_port_->connect()) {
      response_port = kDapPortModeFailed;
    }
  } else {
    response_port = kDapPortModeFailed;
    status = kCmsisDapStatusPortIsNotConnected;
  }
  *response_ = response_port;
  return status;
}


bool CmsisDap::delay(uint16_t /*delay_usec*/) {
  return true;
}

int CmsisDap::commandDapInfo() {
  uint8_t *response_info_length = response_++;

  int info_length = 0;
  int dap_status = kCmsisDapStatusOk;
  switch (*command_) {
    case kInfoIdVendorId:
      info_length = getInfoVendorId();
      break;
    case kInfoIdProductId:
      info_length = getInfoProductId();
      break;
    case kInfoIdSerialNumber:
      info_length = getInfoSerialNumber();
      break;
    case kInfoIdFirmwareVersion:
      info_length = getInfoFirmwareVersion();
      break;
    case kInfoIdTargetDeviceVendor:
      info_length = getInfoTargetDeviceVendor();
      break;
    case kInfoIdTargetDeviceName:
      info_length = getInfoTargetDeviceName();
      break;
    case kInfoIdCapabilities:
      info_length = sizeof(*response_);
      *response_ = getInfoCapabilities();
      break;
    case kInfoIdSwoTraceBufferSize:
      info_length =
          convertWordToResponse(getInfoSwoTraceBufferSize(), &response_);
      break;
    case kInfoIdPacketCount:
      info_length = sizeof(*response_);
      *response_ = getInfoPacketCount();
      break;
    case kInfoIdPacketSize:
      info_length = convertShortToResponse(getInfoPacketSize(), &response_);
      break;
    default:
      dap_status = kCmsisDapStatusUndefinedInfoId;
  }

  *response_length_ = (*response_info_length = info_length) + 2;

  return dap_status;
}

uint8_t CmsisDap::getInfoCapabilities() {
  uint8_t capabilities = kDapInfoCapabilitiesNone;
  if (swd_port_ != nullptr) {
    capabilities |= kDapInfoCapabilitiesSwd;
  }
  if (jtag_port_ != nullptr) {
    capabilities |= kDapInfoCapabilitiesJtag;
  }
  return capabilities;
}

uint32_t CmsisDap::getInfoSwoTraceBufferSize() {
  return 0;
}

uint8_t CmsisDap::getInfoPacketCount() {
  if (host_adapter_ == nullptr) {
    return 0;
  }

  return host_adapter_->getMaxPacketCount();
}

uint16_t CmsisDap::getInfoPacketSize() {
  if (host_adapter_ == nullptr) {
    return 0;
  }

  return host_adapter_->getMaxPacketSize();
}

int CmsisDap::commandDapHostStatus() {
  *response_length_ = 2;
  *response_ = 0x00;

  if (indicator_ == nullptr) {
    return kCmsisDapStatusIndicatorIsNotConnected;
  }

  const int type = *command_++;
  const int status = *command_;

  bool switch_on = false;
  switch (status) {
    case kHostStatusOff:
      switch_on = false;
      break;
    case kHostStatusOn:
      switch_on = true;
      break;
    default:
      return kCmsisDapStatusIllegalCommandField;
  }

  switch (type) {
    case kHostStatusTypeConnect:
      indicator_->switchConnect(switch_on);
      break;
    case kHostStatusTypeRunning:
      indicator_->switchRunning(switch_on);
      break;
    default:
      return kCmsisDapStatusIllegalCommandField;
  }

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapConnect() {
  *response_length_ = 2;

  if (access_port_ != nullptr) {
    access_port_->disconnect();
    access_port_ = nullptr;
  }

  int status = kCmsisDapStatusOk;
  switch (*command_) {
    case kDapPortModeDefault:
      status = connectDefaultPortOnDapConnect();
      break;
    case kDapPortModeSwd:
      status = connectPortOnDapConnect(&swd_port_, kDapPortModeSwd);
      break;
    case kDapPortModeJtag:
      status = connectPortOnDapConnect(&jtag_port_, kDapPortModeJtag);
      break;
    default:
      *response_ = kDapPortModeFailed;
      status = kCmsisDapStatusIllegalCommandField;
  }
  return status;
}

int CmsisDap::commandDapDisconnect() {
  *response_length_ = 2;

  if (access_port_ == nullptr) {
    *response_ = static_cast<uint8_t>(kResponseStatusError);
    return kCmsisDapStatusPortIsNotConnected;
  }

  ResponseStatus response_status = kResponseStatusError;
  if (access_port_->disconnect()) {
    response_status = kResponseStatusOk;
    access_port_ = nullptr;
  } else {
    response_status = kResponseStatusError;
  }

  *response_ = response_status;

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapWriteABORT() {
  *response_length_ = 2;

  if (access_port_ == nullptr) {
    *response_ = static_cast<uint8_t>(kResponseStatusError);
    return kCmsisDapStatusPortIsNotConnected;
  }

  const uint8_t index = *command_++;
  const uint32_t abort = convertCommandToWord(&command_);

  *response_ = checkResponseStatus(access_port_->writeAbort(index, abort));

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapDelay() {
  *response_length_ = 2;

  const uint16_t delay_us = convertCommandToShort(&command_);

  *response_ = checkResponseStatus(delay(delay_us));

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapResetTarget() {
  *response_length_ = 3;

  if (access_port_ == nullptr) {
    *response_++ = kResponseStatusOk;
    *response_ = kResetSequenceNotImplemented;
    return kCmsisDapStatusPortIsNotConnected;
  }

  ResponseStatus response_status = kResponseStatusOk;
  ResetSequence response_execute = kResetSequenceNotImplemented;
  if (access_port_->isImplementedResetSequence()) {
    response_execute = kResetSequenceImplemented;
    if (!access_port_->reset()) {
      response_status = kResponseStatusError;
    }
  }
  *response_++ = response_status;
  *response_ = response_execute;

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapSwjPins() {
  *response_length_ = 2;

  if (access_port_ == nullptr) {
    *response_ = 0x00;
    return kCmsisDapStatusPortIsNotConnected;
  }

  const uint8_t pin_output = *command_++;
  const uint8_t pin_select = *command_++;

  const uint32_t pin_wait_usec = convertCommandToWord(&command_);

  *response_ = access_port_->setPins(pin_output, pin_select, pin_wait_usec);

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapSwjClock() {
  *response_length_ = 2;

  const uint32_t clock = convertCommandToWord(&command_);

  ResponseStatus response_status = kResponseStatusError;
  if (swd_port_ != nullptr && swd_port_->setClock(clock)) {
    response_status = kResponseStatusOk;
  }
  if (jtag_port_ != nullptr && jtag_port_->setClock(clock)) {
    response_status = kResponseStatusOk;
  }
  *response_ = response_status;

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapSwjSequence() {
  *response_length_ = 2;

  if (access_port_ == nullptr) {
    *response_ = static_cast<uint8_t>(kResponseStatusError);
    return kCmsisDapStatusPortIsNotConnected;
  }

  int sequence_bit_count = *command_;
  if (sequence_bit_count == 0) {
    sequence_bit_count = 256;
  }

  ResponseStatus response_status = kResponseStatusOk;
  for (int i = 0; i < sequence_bit_count / DapPort::kSwjSequenceMaxCycle; i++) {
    if (!access_port_->sequenceSwj(DapPort::kSwjSequenceMaxCycle,
                                   *++command_)) {
      response_status = kResponseStatusError;
    }
  }
  if ((sequence_bit_count % DapPort::kSwjSequenceMaxCycle) > 0) {
    if (!access_port_->sequenceSwj((sequence_bit_count %
                                    DapPort::kSwjSequenceMaxCycle),
                                   *++command_)) {
      response_status = kResponseStatusError;
    }
  }
  *response_ = response_status;

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapSwdConfigure() {
  *response_length_ = 2;

  if (access_port_ == nullptr) {
    *response_ = static_cast<uint8_t>(kResponseStatusError);
    return kCmsisDapStatusPortIsNotConnected;
  }

  const uint8_t configuration = *command_;

  const uint8_t turnaround_clock =
      configuration & kDapSwdConfigureTurnaroundClock;
  const bool generate_data_phase =
      ((configuration & kDapSwdConfigureGenerateDataPhase) != 0x0) ?
      true : false;

  *response_ =
      checkResponseStatus(access_port_->configurateSwd(turnaround_clock,
                                                       generate_data_phase));

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapJtagSequence() {
  uint32_t length = 2;

  if (access_port_ == nullptr) {
    *response_length_ = length;
    *response_ = static_cast<uint8_t>(kResponseStatusError);
    return kCmsisDapStatusPortIsNotConnected;
  }

  uint8_t *response_status = response_;

  ResponseStatus result_status = kResponseStatusOk;
  for (int i = *command_; i > 0; i--) {
    const uint8_t sequence_info = *(++command_);

    int tck_cycles = (sequence_info & kJtagSequenceInfoTckCycles);
    if (tck_cycles == 0) {
      tck_cycles = 64;
    }

    const int tms_value =
        ((sequence_info & kJtagSequenceInfoTmsValue) != 0) ? 1 : 0;
    const bool tdo_capture =
        ((sequence_info & kJtagSequenceInfoTdoCapture) != 0) ? true : false;

    for ( ; tck_cycles > 0; tck_cycles -= 8) {
      uint8_t tdo_data = 0;
      if (!access_port_->sequenceJtag(tck_cycles, tms_value,
                                      *(++command_), &tdo_data)) {
        result_status = kResponseStatusError;
      }
      if (tdo_capture) {
        *(++response_) = tdo_data;
        length++;
      }
    }
  }

  *response_length_ = length;
  *response_status = result_status;

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapJtagConfigure() {
  *response_length_ = 2;

  if (access_port_ == nullptr) {
    *response_ = static_cast<uint8_t>(kResponseStatusError);
    return kCmsisDapStatusPortIsNotConnected;
  }

  const int count = *command_;
  ResponseStatus result_status = kResponseStatusOk;
  if (access_port_->setNumberOfJtagDevices(count)) {
    for (int i = 0; i < count; i++) {
      if (!access_port_->setJtagIrLength(i, *(++command_))) {
        result_status = kResponseStatusError;
      }
    }
  } else {
    result_status = kResponseStatusError;
  }

  *response_ = result_status;

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapJtagIDCODE() {
  uint32_t length = 2;

  if (access_port_ == nullptr) {
    *response_length_ = length;
    *response_ = static_cast<uint8_t>(kResponseStatusError);
    return kCmsisDapStatusPortIsNotConnected;
  }

  ResponseStatus result_status = kResponseStatusError;
  uint8_t *response_status = response_++;
  uint32_t idcode = 0;
  if (access_port_->readJtagIdcode(*command_, &idcode)) {
    length += convertWordToResponse(idcode, &response_);
    result_status = kResponseStatusOk;
  }

  *response_length_ = length;
  *response_status = result_status;

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapTransferConfigure() {
  *response_length_ = 2;

  const int transfer_idle_cycles = *command_++;
  if (access_port_ != nullptr) {
    access_port_->setIdleCycles(transfer_idle_cycles);
  }
  transfer_wait_retry_ = convertCommandToShort(&command_);
  transfer_match_retry_ = convertCommandToShort(&command_);

  *response_ = kResponseStatusOk;

  return kCmsisDapStatusOk;
}

int CmsisDap::commandDapTransfer() {
  int response_transfer_count = 0;
  int transfer_response = kTransferAckError;
  int response_data_count = 0;
  *response_length_ = 3;
  uint8_t *response_head = response_;

  if (access_port_ == nullptr) {
    buildTransferResponse(response_transfer_count,
                          transfer_response, response_data_count,
                          response_length_, response_head, response_head);
    return kCmsisDapStatusPortIsNotConnected;
  }

  if (!access_port_->initStateMachine(*command_++)) {
    buildTransferResponse(response_transfer_count,
                          transfer_response, response_data_count,
                          response_length_, response_head, response_head);
    return kCmsisDapStatusError;
  }

  response_ += 2;
  uint32_t transfer_mask = 0xffffffff;
  transfer_abort_ = false;
  for (int i = *command_++; i > 0; i--) {
    DapPort::transfer_func_ptr transfer_function =
        access_port_->transitionStateMachine(*command_++);

    uint32_t transfer_data = 0;
    if (access_port_->isFlagWrite()) {
      transfer_data = convertCommandToWord(&command_);
      if (access_port_->isFlagMask()) {
        transfer_mask = transfer_data;
        continue;
      }
    }

    uint32_t read_data = 0;
    bool mismatched = access_port_->isFlagMatch() ? true :false;

    for (int j = transfer_match_retry_; j >= 0; j--) {
      for (int k = transfer_wait_retry_; k >= 0; k--) {
        transfer_response =
            (access_port_->*transfer_function)(transfer_data, &read_data);
        if (transfer_abort_) {
          buildTransferResponse(response_transfer_count,
                                transfer_response, response_data_count,
                                response_length_, response_head, response_head);
          return kCmsisDapStatusTransferAbort;
        } else if (transfer_response != kTransferAckWait) {
          break;
        }
      }

      if (transfer_response != kTransferAckOk) {
        buildTransferResponse(response_transfer_count,
                              transfer_response, response_data_count,
                              response_length_, response_head, response_head);
        return kCmsisDapStatusOk;
      }

      if (mismatched &&
          ((read_data & transfer_mask) == transfer_data)) {
        mismatched = false;
        break;
      }
    }

    if (access_port_->isFlagResponse()) {
      response_transfer_count++;
      if (access_port_->isFlagRead()) {
        response_data_count++;
        convertWordToResponse(read_data, &response_);
      }
      if (mismatched) {
        transfer_response |= kTransferReadMismatch;
        buildTransferResponse(response_transfer_count,
                              transfer_response, response_data_count,
                              response_length_, response_head, response_head);
        return kCmsisDapStatusOk;
      }
    }

    if (access_port_->isFlagRewind()) {
      i++;
      command_--;
    }
  }

  return terminateDapTransfer(response_transfer_count,
                              response_data_count,
                              response_head,
                              response_head);
}

int CmsisDap::commandDapTransferBlock() {
  int response_transfer_count = 0;
  int transfer_response = kTransferAckError;
  int response_data_count = 0;
  *response_length_ = 4;
  uint8_t *response_head = response_;

  if (access_port_ == nullptr) {
    buildTransferResponse(response_transfer_count,
                          transfer_response, response_data_count,
                          response_length_, response_head, response_head + 1);
    return kCmsisDapStatusPortIsNotConnected;
  }

  if (!access_port_->initStateMachine(*command_++)) {
    buildTransferResponse(response_transfer_count,
                          transfer_response, response_data_count,
                          response_length_, response_head, response_head + 1);
    return kCmsisDapStatusError;
  }

  int transfer_count = convertCommandToShort(&command_);
  const uint8_t transfer_request = *command_++;

  response_ += 3;
  transfer_abort_ = false;

  while (transfer_count-- > 0) {
    DapPort::transfer_func_ptr transfer_function =
        access_port_->transitionStateMachine(transfer_request);

    const uint32_t transfer_data =
        access_port_->isFlagWrite() ? convertCommandToWord(&command_) : 0;

    uint32_t read_data = 0;
    for (int i = transfer_wait_retry_; i >= 0; i--) {
      transfer_response =
          (access_port_->*transfer_function)(transfer_data, &read_data);
      if (transfer_abort_) {
        buildTransferResponse(response_transfer_count,
                              transfer_response, response_data_count,
                              response_length_,
                              response_head, response_head + 1);
        return kCmsisDapStatusTransferAbort;
      } else if (transfer_response != kTransferAckWait) {
        break;
      }
    }

    if (transfer_response != kTransferAckOk) {
      buildTransferResponse(response_transfer_count,
                            transfer_response, response_data_count,
                            response_length_, response_head, response_head + 1);
      return kCmsisDapStatusOk;
    }

    if (access_port_->isFlagResponse()) {
      response_transfer_count++;
      if (access_port_->isFlagRead()) {
        response_data_count++;
        convertWordToResponse(read_data, &response_);
      }
    }

    if (access_port_->isFlagRewind()) {
      transfer_count++;
    }
  }

  return terminateDapTransfer(response_transfer_count,
                              response_data_count,
                              response_head,
                              response_head + 1);
}

int CmsisDap::terminateDapTransfer(int response_transfer_count,
                                   int response_data_count,
                                   uint8_t *response_count_lsb,
                                   uint8_t *response_count_msb) {
  DapPort::transfer_func_ptr transfer_function =
      access_port_->terminateStateMachine();

  int transfer_response = kTransferAckError;
  uint32_t read_data = 0;
  for (int i = transfer_wait_retry_; i >= 0; i--) {
    transfer_response = (access_port_->*transfer_function)(0, &read_data);
    if (transfer_abort_) {
      buildTransferResponse(response_transfer_count,
                            transfer_response, response_data_count,
                            response_length_,
                            response_count_lsb, response_count_msb);
      return kCmsisDapStatusTransferAbort;
    } else if (transfer_response != kTransferAckWait) {
      break;
    }
  }

  if (transfer_response != kTransferAckOk) {
    buildTransferResponse(response_transfer_count,
                          transfer_response, response_data_count,
                          response_length_,
                          response_count_lsb, response_count_msb);
    return kCmsisDapStatusOk;
  }

  if (access_port_->isFlagResponse()) {
    response_transfer_count++;
    if (access_port_->isFlagRead()) {
      response_data_count++;
      convertWordToResponse(read_data, &response_);
    }
  }

  buildTransferResponse(response_transfer_count,
                        transfer_response, response_data_count,
                        response_length_,
                        response_count_lsb, response_count_msb);

  return kCmsisDapStatusOk;
}

}  // end namespace dbger
