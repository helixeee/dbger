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

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "test/core/cases/test_cmsis_dap.h"

namespace dbger {

struct DapSwjPins : public TestCmsisDap {
  DapSwjPins() {
    allocateCommand(7);
  }

  void setCommand(uint8_t output, uint8_t select, uint32_t wait) {
    initCommand();
    command_length = 7;
    command[0] = CmsisDap::kCommandIdDapSwjPins;
    command[1] = output;
    command[2] = select;
    command[3] = wait & 0xff;
    command[4] = (wait >> 8) & 0xff;
    command[5] = (wait >> 16) & 0xff;
    command[6] = (wait >> 24) & 0xff;
  }
  void checkResponse(uint8_t input) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapSwjPins, response[0]);
    BYTES_EQUAL(input, response[1]);
  }
};

TEST_GROUP_BASE(Command_DapSwjPins_PortIsNotConnected, DapSwjPins) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapSwjPins_PortIsNotConnected, Default) {
  setCommand(0, 0, 0);
  commandCmsisDap();
  checkResponse(0);

  setCommand(0, 0, 0xffffffff);
  commandCmsisDap();
  checkResponse(0);

  setCommand(0, 0xff, 0);
  commandCmsisDap();
  checkResponse(0);

  setCommand(0, 0xff, 0xffffffff);
  commandCmsisDap();
  checkResponse(0);

  setCommand(0xff, 0, 0);
  commandCmsisDap();
  checkResponse(0);

  setCommand(0xff, 0, 0xffffffff);
  commandCmsisDap();
  checkResponse(0);

  setCommand(0xff, 0xff, 0);
  commandCmsisDap();
  checkResponse(0);

  setCommand(0xff, 0xff, 0xffffffff);
  commandCmsisDap();
  checkResponse(0);

  setCommand(0xab, 0xcd, 0xef012345);
  commandCmsisDap();
  checkResponse(0);
}

TEST_GROUP_BASE(Command_DapSwjPins_PortIsConnected, DapSwjPins) {
  void setup() {
    newCmsisDapMock();
    attachPort();
    connectPortSwd();
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }
  void setMock(uint8_t output, uint8_t select, uint32_t wait, uint8_t input) {
    mock().expectOneCall("DapPort::setPins")
        .withParameter("pin_output", output)
        .withParameter("pin_select", select)
        .withParameter("pin_wait", wait)
        .andReturnValue(input);
  }
};

TEST(Command_DapSwjPins_PortIsConnected, Default) {
  setCommand(0x00, 0x00, 0x00000000);
  setMock(0x00, 0x00, 0x00000000, 0x00);
  commandCmsisDap();
  checkResponse(0x00);

  setCommand(0xff, 0xff, 0xffffffff);
  setMock(0xff, 0xff, 0xffffffff, 0xff);
  commandCmsisDap();
  checkResponse(0xff);

  setCommand(0x12, 0x34, 0x56789abc);
  setMock(0x12, 0x34, 0x56789abc, 0xef);
  commandCmsisDap();
  checkResponse(0xef);
}

}  // namespace dbger
