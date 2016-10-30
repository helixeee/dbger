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

#include "platforms/mbed/mbed_dap_port.h"

#include "core/cmsis_dap.h"
#include "platforms/mbed/mbed_pin_controller.h"

#include "Timer.h"
#include "wait_api.h"

#include "config/cxx.h"

namespace dbger {

namespace arm_mbed {

using mbed::Timer;

MbedDapPortSwd::MbedDapPortSwd(MbedPinControllerSwd *pin_controller)
    : pin_controller_(pin_controller),
      half_cycle_usec_(0) {
}

MbedDapPortSwd::~MbedDapPortSwd() {
}

bool MbedDapPortSwd::connect() {
  pin_controller_->setModeSwclkTck(MbedPinController::kPinModeOutput);
  pin_controller_->writeSwclkTck(1);

  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeOutput);
  pin_controller_->writeSwdioTms(1);

  pin_controller_->setModeNReset(MbedPinController::kPinModeOutput);
  pin_controller_->writeNReset(1);

  return true;
}

bool MbedDapPortSwd::disconnect() {
  pin_controller_->setModeSwclkTck(MbedPinController::kPinModeInput);
  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeInput);
  pin_controller_->setModeNReset(MbedPinController::kPinModeInput);

  return true;
}

uint8_t MbedDapPortSwd::setPins(uint8_t pin_output,
                                uint8_t pin_select,
                                uint32_t wait_usec) {
  int output_swclk = 0;
  bool select_swclk = false;
  if ((pin_select & kPinMapSwclkTck) != 0) {
    select_swclk = true;
    if ((pin_output & kPinMapSwclkTck) != 0) {
      output_swclk = 1;
    }
    pin_controller_->writeSwclkTck(output_swclk);
  }

  int output_swdio = 0;
  bool select_swdio = false;
  if ((pin_select & kPinMapSwdioTms) != 0) {
    select_swdio = true;
    if ((pin_output & kPinMapSwdioTms) != 0) {
      output_swdio = 1;
    }
    pin_controller_->writeSwdioTms(output_swdio);
  }

  int output_n_reset = 0;
  bool select_n_reset = false;
  if ((pin_select & kPinMapNReset) != 0) {
    select_n_reset = true;
    if ((pin_output & kPinMapNReset) != 0) {
      output_n_reset = 1;
    }
    pin_controller_->writeNReset(output_n_reset);
  }

  if (!select_swclk && !select_swdio && !select_n_reset) {
    return 0;
  }

  Timer timer;
  for (timer.start(); static_cast<uint32_t>(timer.read_us()) < wait_usec; ) {
    if (select_swclk && (pin_controller_->readSwclkTck() != output_swclk)) {
      continue;
    } else if (select_swdio &&
               (pin_controller_->readSwdioTms() != output_swdio)) {
      continue;
    } else if (select_n_reset &&
               (pin_controller_->readNReset() != output_n_reset)) {
      continue;
    }
    break;
  }
  timer.stop();

  uint8_t input = 0;
  if (select_swclk && pin_controller_->readSwclkTck()) {
    input |= kPinMapSwclkTck;
  }
  if (select_swdio && pin_controller_->readSwdioTms()) {
    input |= kPinMapSwdioTms;
  }
  if (select_n_reset && pin_controller_->readNReset()) {
    input |= kPinMapNReset;
  }
  return input;
}

bool MbedDapPortSwd::setClock(uint32_t clock) {
  if (clock > 500000) {
    half_cycle_usec_ = 0;
  } else {
    half_cycle_usec_ = 1000000 / 2 / clock;
  }
  return true;
}

bool MbedDapPortSwd::sequenceSwj(int bit_count,
                                 uint8_t bit_data) {
  pin_controller_->setModeSwclkTck(MbedPinController::kPinModeOutput);
  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeOutput);

  while (bit_count-- > 0) {
    cycleWrite(bit_data & 0x1);
    bit_data >>= 1;
  }

  return true;
}

int MbedDapPortSwd::readImpl(uint32_t /*write_data*/, uint32_t *read_data) {
  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeOutput);
  phaseRequest(true);
  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeInput);
  phaseTurnaround();

  int ack = phaseAck();

  if (ack == CmsisDap::kTransferAckOk) {
    if (!phaseRead(read_data)) {
      ack |= CmsisDap::kTransferSwdProtocolError;
    }
    phaseTurnaround();
  } else if ((ack == CmsisDap::kTransferAckWait) ||
             (ack == CmsisDap::kTransferAckFault)) {
    if (generate_data_phase_) {
      phaseDummyRead();
    }
    phaseTurnaround();
  } else {
    ack = CmsisDap::kTransferAckError;
  }

  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeOutput);
  pin_controller_->writeSwdioTms(1);

  return ack;
}

int MbedDapPortSwd::writeImpl(uint32_t write_data,
                              uint32_t * /*read_data*/) {
  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeOutput);
  phaseRequest(false);
  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeInput);
  phaseTurnaround();
  int ack = phaseAck();
  phaseTurnaround();

  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeOutput);

  if (ack == CmsisDap::kTransferAckOk) {
    phaseWrite(write_data);
  } else if ((ack == CmsisDap::kTransferAckWait) ||
             (ack == CmsisDap::kTransferAckFault)) {
    phaseWrite(0);
  } else {
    ack = CmsisDap::kTransferAckError;
  }

  pin_controller_->writeSwdioTms(1);

  return ack;
}

int MbedDapPortSwd::cycleWrite(int write_data) {
  pin_controller_->writeSwdioTms(write_data);
  pin_controller_->writeSwclkTck(0);

  wait_us(half_cycle_usec_);

  pin_controller_->writeSwclkTck(1);

  wait_us(half_cycle_usec_);

  return 0;
}

int MbedDapPortSwd::cycleRead() {
  pin_controller_->writeSwclkTck(0);

  wait_us(half_cycle_usec_);

  int read_bit = pin_controller_->readSwdioTms();

  pin_controller_->writeSwclkTck(1);

  wait_us(half_cycle_usec_);

  return read_bit;
}

bool MbedDapPortSwd::phaseRequest(bool read_not_write) {
  int APnDP = 0;
  switch (transfer_information.current_ir_register) {
    case kIrRegisterApacc:
      APnDP = kSwdProtocolAPnDPApacc;
      break;
    case kIrRegisterDpacc:
      APnDP = kSwdProtocolAPnDPDpacc;
      break;
    default:
      return false;
  }

  const int RnW = read_not_write ? kSwdProtocolRnWRead : kSwdProtocolRnWWrite;
  const int A2 = (transfer_information.address & (0x1 << 2)) ? 1 : 0;
  const int A3 = (transfer_information.address & (0x1 << 3)) ? 1 : 0;
  const int parity = (APnDP ^ RnW ^ A2 ^ A3);

  cycleWrite(kSwdProtocolStart);
  cycleWrite(APnDP);
  cycleWrite(RnW);
  cycleWrite(A2);
  cycleWrite(A3);
  cycleWrite(parity);
  cycleWrite(kSwdProtocolStop);
  cycleWrite(kSwdProtocolPark);

  return true;
}

int MbedDapPortSwd::phaseAck() {
  int ack = 0;
  for (int i = 0; i < kSwdCycleAck; i++) {
    ack |= (cycleRead() << i);
  }
  return ack;
}

void MbedDapPortSwd::phaseWrite(uint32_t write_data) {
  int parity = 0;
  for (int i = 0; i < kDataSize; i++) {
    const int value = write_data & 0x1;
    cycleWrite(value);
    parity ^= value;
    write_data >>= 1;
  }
  cycleWrite(parity);
}

bool MbedDapPortSwd::phaseRead(uint32_t *read_data) {
  uint32_t read = 0;
  int parity = 0;
  for (int i = 0; i < kDataSize; i++) {
    const uint32_t value = cycleRead();
    read |= (value << i);
    parity ^= value;
  }
  *read_data = read;

  parity ^= cycleRead();

  return (parity ? false : true);
}

bool MbedDapPortSwd::phaseDummyRead() {
  for (int i = 0; i <= kDataSize; i++) {
    cycleRead();
  }
  return true;
}

void MbedDapPortSwd::phaseTurnaround() {
  for (int i = turnaround_clock_; i >= 0; i--) {
    cycleRead();
  }
}

MbedDapPortJtag::MbedDapPortJtag(MbedPinControllerJtag *pin_controller)
    : pin_controller_(pin_controller),
      half_cycle_usec_(0),
      device_count_(1), device_index_(0), device_ir_length_(nullptr) {
  device_ir_length_ = new uint8_t[1];
  device_ir_length_[0] = kJtagChainIrLength;
}

MbedDapPortJtag::~MbedDapPortJtag() {
  delete[] device_ir_length_;
}

bool MbedDapPortJtag::connect() {
  pin_controller_->setModeSwclkTck(MbedPinController::kPinModeOutput);
  pin_controller_->writeSwclkTck(1);

  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeOutput);
  pin_controller_->writeSwdioTms(1);

  pin_controller_->setModeTdi(MbedPinController::kPinModeOutput);
  pin_controller_->writeTdi(1);

  pin_controller_->setModeTdo(MbedPinController::kPinModeInput);

  pin_controller_->setModeNTrst(MbedPinController::kPinModeOutput);
  pin_controller_->writeNTrst(1);

  pin_controller_->setModeNReset(MbedPinController::kPinModeOutput);
  pin_controller_->writeNReset(1);

  return true;
}

bool MbedDapPortJtag::disconnect() {
  pin_controller_->setModeSwclkTck(MbedPinController::kPinModeInput);
  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeInput);
  pin_controller_->setModeTdi(MbedPinController::kPinModeInput);
  pin_controller_->setModeNTrst(MbedPinController::kPinModeInput);
  pin_controller_->setModeNReset(MbedPinController::kPinModeInput);

  return true;
}

bool MbedDapPortJtag::writeAbort(uint8_t index, uint32_t abort) {
  writeIrRegister(index, kIrRegisterAbort);
  transferDrRegister(index, false, kDpRegisterSwAbort, abort, nullptr);

  if (index == device_index_) {
    transfer_information.previous_ir_register = kIrRegisterAbort;
  }

  return false;
}

uint8_t MbedDapPortJtag::setPins(uint8_t pin_output,
                                 uint8_t pin_select,
                                 uint32_t wait_usec) {
  int output_tck = 0;
  bool select_tck = false;
  if ((pin_select & kPinMapSwclkTck) != 0) {
    select_tck = true;
    if ((pin_output & kPinMapSwclkTck) != 0) {
      output_tck = 1;
    }
    pin_controller_->writeSwclkTck(output_tck);
  }

  int output_tms = 0;
  bool select_tms = false;
  if ((pin_select & kPinMapSwdioTms) != 0) {
    select_tms = true;
    if ((pin_output & kPinMapSwdioTms) != 0) {
      output_tms = 1;
    }
    pin_controller_->writeSwdioTms(output_tms);
  }

  int output_tdi = 0;
  bool select_tdi = false;
  if ((pin_select & kPinMapTdi) != 0) {
    select_tdi = true;
    if ((pin_output & kPinMapTdi) != 0) {
      output_tdi = 1;
    }
    pin_controller_->writeTdi(output_tdi);
  }

  int output_tdo = 0;
  bool select_tdo = false;
  if ((pin_select & kPinMapTdo) != 0) {
    select_tdo = true;
    if ((pin_output & kPinMapTdo) != 0) {
      output_tdo = 1;
    }
    pin_controller_->writeTdo(output_tdo);
  }

  int output_n_trst = 0;
  bool select_n_trst = false;
  if ((pin_select & kPinMapNTrst) != 0) {
    select_n_trst = true;
    if ((pin_output & kPinMapNTrst) != 0) {
      output_n_trst = 1;
    }
    pin_controller_->writeNTrst(output_n_trst);
  }

  int output_n_reset = 0;
  bool select_n_reset = false;
  if ((pin_select & kPinMapNReset) != 0) {
    select_n_reset = true;
    if ((pin_output & kPinMapNReset) != 0) {
      output_n_reset = 1;
    }
    pin_controller_->writeNReset(output_n_reset);
  }

  Timer timer;
  for (timer.start(); (uint32_t)timer.read_us() < wait_usec; ) {
    if (select_tck && (pin_controller_->readSwclkTck() != output_tck)) {
      continue;
    } else if (select_tms && (pin_controller_->readSwdioTms() != output_tms)) {
      continue;
    } else if (select_tdi && (pin_controller_->readTdi() != output_tdi)) {
      continue;
    } else if (select_n_trst &&
               (pin_controller_->readNTrst() != output_n_trst)) {
      continue;
    } else if (select_n_reset &&
               (pin_controller_->readNReset() != output_n_reset)) {
      continue;
    }
    break;
  }
  timer.stop();

  uint8_t input = 0;
  if (select_tck && pin_controller_->readSwclkTck()) {
    input |= kPinMapSwclkTck;
  }
  if (select_tms && pin_controller_->readSwdioTms()) {
    input |= kPinMapSwdioTms;
  }
  if (select_tdi && pin_controller_->readTdi()) {
    input |= kPinMapTdi;
  }
  if (select_tdo && pin_controller_->readTdo()) {
    input |= kPinMapTdo;
  }
  if (select_n_trst && pin_controller_->readNTrst()) {
    input |= kPinMapNTrst;
  }
  if (select_n_reset && pin_controller_->readNReset()) {
    input |= kPinMapNReset;
  }

  return input;
}

bool MbedDapPortJtag::setClock(uint32_t clock) {
  if (clock > 500000) {
    half_cycle_usec_ = 0;
  } else {
    half_cycle_usec_ = 1000000 / 2 / clock;
  }
  return true;
}

bool MbedDapPortJtag::sequenceSwj(int bit_count,
                                  uint8_t bit_data) {
  pin_controller_->setModeSwdioTms(MbedPinController::kPinModeOutput);
  pin_controller_->setModeSwclkTck(MbedPinController::kPinModeOutput);

  while (bit_count-- > 0) {
    cycle(bit_data & 0x1, 1);
    bit_data >>= 1;
  }

  return true;
}

bool MbedDapPortJtag::configurateJtag(int count, const uint8_t *ir_length) {
  delete[] device_ir_length_;

  device_count_ = count;
  device_ir_length_ = new uint8_t[count];
  for (int i = 0; i < count; i++) {
    device_ir_length_[i]= ir_length[i];
  }

  return true;
}

int MbedDapPortJtag::readImpl(uint32_t write_data, uint32_t *read_data) {
  return transfer(true, write_data, read_data);
}

int MbedDapPortJtag::writeImpl(uint32_t write_data, uint32_t *read_data) {
  return transfer(false, write_data, read_data);
}

bool MbedDapPortJtag::writeIrRegister(int device_index, int ir_length) {
  transitionToSelectScan();
  transitionToSelectScan();
  transitionToCapture();
  transitionToShift();

  int index = 0;
  for ( ; index < device_index; index++) {
    for (int i = 0; i < device_ir_length_[index]; i++) {
      transitionRepeatShift(1);
    }
  }

  for (int i = 0; i < device_ir_length_[index] - 1; i++) {
    transitionRepeatShift(ir_length & 0x1);
    ir_length >>= 1;
  }

  ir_length &= 0x1;

  if (++index < device_count_) {
    transitionRepeatShift(ir_length);
    ir_length = 1;

    for ( ; index < device_count_ - 1; index++) {
      for (int i = 0; i < device_ir_length_[index]; i++) {
        transitionRepeatShift(1);
      }
    }
    for (int i = 0; i < device_ir_length_[index] - 1; i++) {
      transitionRepeatShift(1);
    }
  }
  transitionToExit1(ir_length);
  transitionToUpdate();

  return true;
}

int MbedDapPortJtag::transferDrRegister(int device_index,
                                        bool read_not_write,
                                        int address,
                                        uint32_t write_data,
                                        uint32_t *read_data) {
  transitionToSelectScan();
  transitionToCapture();
  transitionToShift();

  int *capture_tdo = new int[device_index + (3 + 32)];
  int *capture_pointer = capture_tdo;

  for (int i = 0; i < device_index; i++) {
    *capture_pointer++ = transitionRepeatShift(1);
  }

  *capture_pointer++ = transitionRepeatShift(read_not_write ? 1 : 0);
  *capture_pointer++ =
      transitionRepeatShift((address >> 2) & 0x1);
  *capture_pointer++ =
      transitionRepeatShift((address >> 3) & 0x1);

  for (int i = 0; i < kDataSize - 1; i++) {
    *capture_pointer++ = transitionRepeatShift(write_data & 0x1);
    write_data >>= 1;
  }
  *capture_pointer = transitionToExit1(write_data & 0x1);

  transitionToUpdate();
  transitionToIdle();
  for (int i = 0; i < idle_cycles_; i++) {
    transitionToIdle();
  }

  int *read_pointer = &capture_tdo[device_count_ - (device_index + 1)];

  int ack = 0;
  ack |= *read_pointer++ << 1;
  ack |= *read_pointer++ << 0;
  ack |= *read_pointer++ << 2;

  uint32_t read = 0;
  for (int i = 0; i < kDataSize; i++) {
    read |= *read_pointer++ << i;
  }

  *read_data = read;

  delete[] capture_tdo;

  return ack;
}

int MbedDapPortJtag::transfer(bool read_not_write,
                              uint32_t write_data,
                              uint32_t *read_data) {
  if (transfer_information.previous_ir_register !=
      transfer_information.current_ir_register) {
    writeIrRegister(device_index_, transfer_information.current_ir_register);

    transfer_information.previous_ir_register =
        transfer_information.current_ir_register;
  }

  int ack = transferDrRegister(device_index_,
                               read_not_write,
                               transfer_information.address,
                               write_data,
                               read_data);

  return ack;
}

int MbedDapPortJtag::cycle(int tms, int tdi) {
  pin_controller_->writeSwdioTms(tms);
  pin_controller_->writeTdi(tdi);
  pin_controller_->writeSwclkTck(0);

  wait_us(half_cycle_usec_);

  pin_controller_->writeSwclkTck(1);

  wait_us(half_cycle_usec_);

  return pin_controller_->readTdo();
}

inline void MbedDapPortJtag::transitionToSelectScan() {
  cycle(1, 1);
}

inline void MbedDapPortJtag::transitionToCapture() {
  cycle(0, 1);
}

inline void MbedDapPortJtag::transitionToShift() {
  cycle(0, 1);
}

inline int MbedDapPortJtag::transitionRepeatShift(int tdi) {
  return cycle(0, tdi);
}

inline int MbedDapPortJtag::transitionToExit1(int tdi) {
  return cycle(1, tdi);
}

inline void MbedDapPortJtag::transitionToUpdate() {
  cycle(1, 1);
}

inline void MbedDapPortJtag::transitionToIdle() {
  cycle(0, 1);
}

}  // namespace arm_mbed

}  // namespace dbger
