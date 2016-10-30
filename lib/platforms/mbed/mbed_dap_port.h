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

#ifndef LIB_PLATFORMS_MBED_MBED_DAP_PORT_H_
#define LIB_PLATFORMS_MBED_MBED_DAP_PORT_H_

#include "core/dap_port.h"

namespace dbger {

namespace arm_mbed {

class MbedPinControllerSwd;

class MbedDapPortSwd : public DapPortSwd {
 public:
  explicit MbedDapPortSwd(MbedPinControllerSwd *pin_controller);
  virtual ~MbedDapPortSwd();

  virtual bool connect();
  virtual bool disconnect();
  virtual uint8_t setPins(uint8_t pin_output,
                          uint8_t pin_select,
                          uint32_t wait_usec);
  virtual bool setClock(uint32_t clock);
  virtual bool sequenceSwj(int bit_count, uint8_t bit_data);

 protected:
  virtual int readImpl(uint32_t write_data, uint32_t *read_data);
  virtual int writeImpl(uint32_t write_data, uint32_t *read_data);

  int cycleWrite(int write_data);
  int cycleRead();

  bool phaseRequest(bool read_not_write);
  int phaseAck();
  void phaseWrite(uint32_t data);
  bool phaseRead(uint32_t *data);
  bool phaseDummyRead();
  void phaseTurnaround();

  MbedPinControllerSwd *pin_controller_;

  uint32_t half_cycle_usec_;

 private:
  MbedDapPortSwd();
};

class MbedPinControllerJtag;

class MbedDapPortJtag : public DapPortJtag {
 public:
  explicit MbedDapPortJtag(MbedPinControllerJtag *pin_controller);
  virtual ~MbedDapPortJtag();

  virtual bool connect();
  virtual bool disconnect();
  virtual bool writeAbort(uint8_t index, uint32_t abort);
  virtual uint8_t setPins(uint8_t pin_output,
                          uint8_t pin_select,
                          uint32_t wait_usec);
  virtual bool setClock(uint32_t clock);
  virtual bool sequenceSwj(int bit_count,
                           uint8_t bit_data);
  virtual bool configurateJtag(int count, const uint8_t *ir_length);

 protected:
  int transfer(bool read_not_write, uint32_t write_data, uint32_t *read_data);
  bool writeIrRegister(int device_index, int ir_length);
  int transferDrRegister(int device_index,
                         bool read_not_write, int address,
                         uint32_t write_data, uint32_t *read_data);

  virtual int readImpl(uint32_t write_data, uint32_t *read_data);
  virtual int writeImpl(uint32_t write_data, uint32_t *read_data);

  int cycle(int tms, int tdi);

  void transitionToSelectScan();
  void transitionToCapture();
  void transitionToShift();
  int transitionRepeatShift(int tdi);
  int transitionToExit1(int tdi);
  void transitionToUpdate();
  void transitionToIdle();

  MbedPinControllerJtag *pin_controller_;

  uint32_t half_cycle_usec_;

  int device_count_;
  int device_index_;
  uint8_t *device_ir_length_;

 private:
  MbedDapPortJtag();
};

}  // namespace arm_mbed

}  // namespace dbger

#endif  // LIB_PLATFORMS_MBED_MBED_DAP_PORT_H_
