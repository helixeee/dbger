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

#include "core/dap_port.h"

#include "core/cmsis_dap.h"

#include "config/cxx.h"

namespace {

using dbger::DapPort;

enum TransferState {
  kTransferStateStart,
  kTransferStateRead,
  kTransferStateReadMatch,
  kTransferStateDone,
};

enum TransferRequestType {
  kTransferRequestTypeMaskRead =
      (DapPort::kTransferRequestRnW | DapPort::kTransferRequestValueMatch),
  kTransferRequestTypeNormalRead = DapPort::kTransferRequestRnW,
  kTransferRequestTypeValueMatch =
      (DapPort::kTransferRequestRnW | DapPort::kTransferRequestValueMatch),
  kTransferRequestTypeMaskWrite = DapPort::kTransferRequestMatchMask,
  kTransferRequestTypeNormalWrite = 0,
  kTransferRequestTypeMatchMask = DapPort::kTransferRequestMatchMask,
};

bool isAPnDP(uint8_t transfer_request) {
  return ((transfer_request & DapPort::kTransferRequestAPnDP) ==
          DapPort::kTransferRequestAPnDP);
}

bool isRnW(uint8_t transfer_request) {
  return ((transfer_request & DapPort::kTransferRequestRnW) ==
          DapPort::kTransferRequestRnW);
}

bool isValueMatch(uint8_t transfer_request) {
  return ((transfer_request & DapPort::kTransferRequestValueMatch) ==
          DapPort::kTransferRequestValueMatch);
}

bool isMatchMask(uint8_t transfer_request) {
  return ((transfer_request & DapPort::kTransferRequestMatchMask) ==
          DapPort::kTransferRequestMatchMask);
}

int getAddress(uint8_t transfer_request) {
  return (transfer_request & DapPort::kTransferRequestAddress);
}

int getIrRegister(uint8_t transfer_request) {
  return isAPnDP(transfer_request) ?
      DapPort::kIrRegisterApacc : DapPort::kIrRegisterDpacc;
}

bool isTransferTypeMatchMask(uint8_t transfer_requst) {
  return ((transfer_requst & kTransferRequestTypeMaskWrite) ==
          kTransferRequestTypeMatchMask);
}

}  // end namespace

namespace dbger {

DapPort::DapPort() {
}

DapPort::~DapPort() {
}

bool DapPort::connect() {
  return false;
}

bool DapPort::disconnect() {
  return true;
}

bool DapPort::writeAbort(uint8_t /*index*/, uint32_t abort) {
  transfer_information.current_ir_register = kIrRegisterDpacc;
  transfer_information.address = kDpRegisterSwAbort;

  if (writeImpl(abort, nullptr) == CmsisDap::kTransferAckOk) {
    return true;
  } else {
    return false;
  }
}

bool DapPort::isImplementedResetSequence() const {
  return false;
}

bool DapPort::reset() {
  return true;
}

uint8_t DapPort::setPins(uint8_t /*pin_output*/,
                         uint8_t /*pin_select*/,
                         uint32_t /*wait_usec*/) {
  return 0;
}

bool DapPort::setClock(uint32_t /*clock*/) {
  return true;
}

bool DapPort::sequenceSwj(int /*bit_count*/,
                          uint8_t /*bit_data*/) {
  return false;
}

bool DapPort::configurateSwd(uint8_t /*turnaround_clock*/,
                             bool /*generate_data_phase*/) {
  return false;
}

bool DapPort::sequenceJtag(int /*tck_cycles*/,
                           int /*tms_value*/,
                           uint8_t /*tdi_data*/,
                           uint8_t * /*tdo_data*/) {
  return false;
}

bool DapPort::setNumberOfJtagDevices(int /*count*/) {
  return false;
}

bool DapPort::setJtagIrLength(int /*index*/, uint8_t /*ir_length*/) {
  return false;
}

bool DapPort::readJtagIdcode(uint8_t /*jtag_index*/, uint32_t * /*idcode*/) {
  return false;
}

bool DapPort::checkJtagIndex(uint8_t /*index*/) {
  return true;
}

bool DapPort::setIdleCycles(int idle_cycles) {
  idle_cycles_ = idle_cycles;
  return true;
}

DapPort::transfer_func_ptr
DapPort::transitionStateMachine(uint8_t /*transfer_request*/) {
  return &DapPort::nop;
}

DapPort::transfer_func_ptr DapPort::terminateStateMachine() {
  clearStateFlag();

  switch (transfer_information.next_state) {
    case kTransferStateRead:
      transfer_information.flag_read_data = true;
      transfer_information.flag_response = true;
      // fall-through
    case kTransferStateDone:
      transfer_information.current_ir_register = kIrRegisterDpacc;
      transfer_information.address = kDpRegisterRdbuff;
      return &DapPort::read;
    default:
      return &DapPort::nop;
  }
}

bool DapPort::initStateMachine(uint8_t device_index) {
  if (!checkJtagIndex(device_index)) {
    return false;
  }

  initState();

  return true;
}

void DapPort::initState() {
  clearNextState();

  transfer_information.current_ir_register = kIrRegisterNone;
  transfer_information.previous_ir_register = kIrRegisterNone;
  transfer_information.address = 0;

  clearStateFlag();
}

void DapPort::clearNextState() {
  transfer_information.next_state = kTransferStateStart;
}

void DapPort::clearStateFlag() {
  transfer_information.flag_write_data = false;
  transfer_information.flag_read_data = false;
  transfer_information.flag_rewind_command = false;
  transfer_information.flag_mask = false;
  transfer_information.flag_match = false;
  transfer_information.flag_response = false;
}

bool DapPort::isFlagWrite() const {
  return transfer_information.flag_write_data;
}

bool DapPort::isFlagRead() const {
  return transfer_information.flag_read_data;
}

bool DapPort::isFlagRewind() const {
  return transfer_information.flag_rewind_command;
}

bool DapPort::isFlagMask() const {
  return transfer_information.flag_mask;
}

bool DapPort::isFlagMatch() const {
  return transfer_information.flag_match;
}

bool DapPort::isFlagResponse() const {
  return transfer_information.flag_response;
}

int DapPort::read(uint32_t write_data, uint32_t *read_data) {
  const int ack = readImpl(write_data, read_data);
  updateTransfer();
  return ack;
}

int DapPort::write(uint32_t write_data, uint32_t *read_data) {
  const int ack = writeImpl(write_data, read_data);
  updateTransfer();
  return ack;
}

int DapPort::nop(uint32_t write_data, uint32_t *read_data) {
  const int ack = nopImpl(write_data, read_data);
  updateTransfer();
  return ack;
}

int DapPort::readImpl(uint32_t /*write_data*/, uint32_t * /*read_data*/) {
  return CmsisDap::kTransferAckOk;
}

int DapPort::writeImpl(uint32_t /*write_data*/, uint32_t * /*read_data*/) {
  return CmsisDap::kTransferAckOk;
}

int DapPort::nopImpl(uint32_t /*write_data*/, uint32_t * /*read_data*/) {
  return CmsisDap::kTransferAckOk;
}

void DapPort::updateTransfer() {
  transfer_information.previous_ir_register =
      transfer_information.current_ir_register;
}

DapPortSwd::DapPortSwd() : turnaround_clock_(0), generate_data_phase_(false) {
}

DapPortSwd::~DapPortSwd() {
}

bool DapPortSwd::writeAbort(uint8_t /*index*/, uint32_t abort) {
  transfer_information.current_ir_register = kIrRegisterDpacc;
  transfer_information.address = kDpRegisterSwAbort;

  if (writeImpl(abort, nullptr) == CmsisDap::kTransferAckOk) {
    return true;
  } else {
    return false;
  }
}

bool DapPortSwd::configurateSwd(uint8_t turnaround_clock,
                                bool generate_data_phase) {
  turnaround_clock_ = turnaround_clock;
  generate_data_phase_ = generate_data_phase;
  return true;
}

DapPort::transfer_func_ptr
DapPortSwd::transitionStateMachine(uint8_t transfer_request) {
  transfer_information.current_ir_register = getIrRegister(transfer_request);
  transfer_information.address = getAddress(transfer_request);

  const int previous_state = transfer_information.next_state;

  clearNextState();
  clearStateFlag();

  DapPort::transfer_func_ptr return_transfer_function = &DapPort::nop;

  switch (previous_state) {
    case kTransferStateStart:
      if (isRnW(transfer_request)) {
        return_transfer_function = &DapPort::read;

        if (isValueMatch(transfer_request)) {
          if (isAPnDP(transfer_request)) {
            transfer_information.next_state = kTransferStateReadMatch;
            transfer_information.flag_rewind_command = true;
          } else {
            transfer_information.flag_write_data = true;
            transfer_information.flag_read_data = true;
            transfer_information.flag_match = true;
            transfer_information.flag_response = true;
          }
        } else {
          if (isAPnDP(transfer_request)) {
            transfer_information.next_state = kTransferStateRead;
          } else {
            transfer_information.flag_read_data = true;
            transfer_information.flag_response = true;
          }
        }
      } else {
        transfer_information.flag_write_data = true;

        if (isMatchMask(transfer_request)) {
          transfer_information.flag_mask = true;
        } else {
          transfer_information.next_state = kTransferStateDone;
          return_transfer_function = &DapPort::write;
          transfer_information.flag_response = true;
        }
      }
      break;
    case kTransferStateRead:
      return_transfer_function = &DapPort::read;

      transfer_information.flag_read_data = true;
      transfer_information.flag_response = true;

      if (isRnW(transfer_request)) {
        if (isValueMatch(transfer_request)) {
          transfer_information.next_state = kTransferStateReadMatch;
          transfer_information.flag_rewind_command = true;

          if (!isAPnDP(transfer_request)) {
            transfer_information.address = DapPort::kDpRegisterRdbuff;
          }
        } else {
          if (isAPnDP(transfer_request)) {
            transfer_information.next_state = kTransferStateRead;
          } else {
            transfer_information.address = DapPort::kDpRegisterRdbuff;
            transfer_information.flag_rewind_command = true;
          }
        }
      } else {
        transfer_information.current_ir_register = DapPort::kIrRegisterDpacc;
        transfer_information.address = DapPort::kDpRegisterRdbuff;
        transfer_information.flag_rewind_command = true;
      }
      break;
    case kTransferStateReadMatch:
      return_transfer_function = &DapPort::read;

      transfer_information.flag_write_data = true;
      transfer_information.flag_read_data = true;
      transfer_information.flag_match = true;
      transfer_information.flag_response = true;
      break;
    case kTransferStateDone:
      if (isTransferTypeMatchMask(transfer_request)) {
        transfer_information.next_state = kTransferStateDone;
        transfer_information.flag_write_data = true;
        transfer_information.flag_mask = true;
      } else {
        transfer_information.flag_rewind_command = true;
      }
      break;
  }

  return return_transfer_function;
}

DapPortJtag::~DapPortJtag() {
}

DapPort::transfer_func_ptr
DapPortJtag::transitionStateMachine(uint8_t transfer_request) {
  transfer_information.current_ir_register = getIrRegister(transfer_request);
  transfer_information.address = getAddress(transfer_request);

  const int previous_state = transfer_information.next_state;

  clearNextState();
  clearStateFlag();

  DapPort::transfer_func_ptr return_transfer_function = &DapPort::nop;

  switch (previous_state) {
    case kTransferStateStart:
      if (isRnW(transfer_request)) {
        return_transfer_function = &DapPort::read;

        if (isValueMatch(transfer_request)) {
          transfer_information.next_state = kTransferStateReadMatch;
          transfer_information.flag_rewind_command = true;
        } else {
          transfer_information.next_state = kTransferStateRead;
        }
      } else {
        transfer_information.flag_write_data = true;

        if (isMatchMask(transfer_request)) {
          transfer_information.flag_mask = true;
        } else {
          transfer_information.next_state = kTransferStateDone;
          return_transfer_function = &DapPort::write;
          transfer_information.flag_response = true;
        }
      }
      break;
    case kTransferStateRead:
      return_transfer_function = &DapPort::read;

      transfer_information.flag_read_data = true;
      transfer_information.flag_response = true;

      if (isRnW(transfer_request)) {
        if (isValueMatch(transfer_request)) {
          transfer_information.current_ir_register = kIrRegisterDpacc;
          transfer_information.address = kDpRegisterRdbuff;

          transfer_information.flag_rewind_command = true;
        } else {
          if (transfer_information.previous_ir_register ==
              transfer_information.current_ir_register) {
            transfer_information.next_state = kTransferStateRead;
          } else {
            transfer_information.current_ir_register = kIrRegisterDpacc;
            transfer_information.address = kDpRegisterRdbuff;

            transfer_information.flag_rewind_command = true;
          }
        }
      } else {
        transfer_information.current_ir_register = kIrRegisterDpacc;
        transfer_information.address = kDpRegisterRdbuff;

        transfer_information.flag_rewind_command = true;
      }
      break;
    case kTransferStateReadMatch:
      return_transfer_function = &DapPort::read;

      transfer_information.flag_write_data = true;
      transfer_information.flag_read_data = true;
      transfer_information.flag_match = true;
      transfer_information.flag_response = true;
      break;
    case kTransferStateDone:
      return_transfer_function = &DapPort::nop;

      if (isMatchMask(transfer_request)) {
        transfer_information.next_state = kTransferStateDone;

        transfer_information.flag_write_data = true;
        transfer_information.flag_mask = true;
      } else {
        transfer_information.flag_rewind_command = true;
      }
      break;
  }

  return return_transfer_function;
}

}  // end namespace dbger
