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

#ifndef TEST_CORE_MOCK_DAP_PORT_MOCK_H_
#define TEST_CORE_MOCK_DAP_PORT_MOCK_H_

#include "CppUTest/SimpleString.h"

#include "core/dap_port.h"

namespace dbger {

struct Command;

class DapPortMock : public DapPort {
 public:
  DapPortMock();
  virtual ~DapPortMock();

  virtual bool connect();
  virtual bool disconnect();

  virtual bool writeAbort(uint8_t index, uint32_t abort);

  virtual bool isImplementedResetSequence() const;
  virtual bool reset();

  virtual uint8_t setPins(uint8_t pin_output,
                          uint8_t pin_select,
                          uint32_t wait_usec);
  virtual bool setClock(uint32_t clock);
  virtual bool sequenceSwj(int bit_count,
                           uint8_t bit_data);

  virtual bool configurateSwd(uint8_t turnaround_clock,
                              bool generate_data_phase);

  virtual bool sequenceJtag(int tck_cycles,
                            int tms_value,
                            uint8_t tdi_data,
                            uint8_t *tdo_data);

  virtual bool setNumberOfJtagDevices(int count);
  virtual bool setJtagIrLength(int index, uint8_t ir_length);

  virtual bool readJtagIdcode(uint8_t jtag_index, uint32_t *idcode);

  virtual bool checkJtagIndex(uint8_t index);
};

class DapPortMockSwd : public DapPortSwd {
 public:
  virtual ~DapPortMockSwd();
  virtual bool connect();
  virtual bool disconnect();
  virtual bool setClock(uint32_t clock);

  virtual int readImpl(uint32_t write_data, uint32_t *read_data);
  virtual int writeImpl(uint32_t write_data, uint32_t *read_data);
};

class DapPortMockJtag : public DapPortJtag {
 public:
  virtual ~DapPortMockJtag();
  virtual bool connect();
  virtual bool disconnect();
  virtual bool setClock(uint32_t clock);

  virtual int readImpl(uint32_t write_data, uint32_t *read_data);
  virtual int writeImpl(uint32_t write_data, uint32_t *read_data);
};

}  // namespace dbger

#endif  // TEST_CORE_MOCK_DAP_PORT_MOCK_H_
