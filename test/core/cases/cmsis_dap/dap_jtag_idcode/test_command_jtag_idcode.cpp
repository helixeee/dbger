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

struct DapJtagIDCODE : public TestCmsisDap {
  DapJtagIDCODE() {
    allocateCommand(6);
  }

  void setCommand(int index) {
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapJtagIDCODE;
    command[1] = index & 0xff;
  }
  void checkResponse(int status) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapJtagIDCODE, response[0]);
    BYTES_EQUAL(status, response[1]);
  }
  void checkResponse(int status, uint32_t idcode) {
    CHECK_EQUAL(6, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapJtagIDCODE, response[0]);
    BYTES_EQUAL(status, response[1]);
    BYTES_EQUAL(idcode & 0xff, response[2]);
    BYTES_EQUAL((idcode >> 8) & 0xff, response[3]);
    BYTES_EQUAL((idcode >> 16) & 0xff, response[4]);
    BYTES_EQUAL((idcode >> 24) & 0xff, response[5]);
  }
};

TEST_GROUP_BASE(Command_DapJtagIDCODE_PortIsNotConnected, DapJtagIDCODE) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapJtagIDCODE_PortIsNotConnected, Default) {
  for (int i = 0; i <= 0xff; i++) {
    setCommand(i);
    commandCmsisDap();
    checkResponse(CmsisDap::kResponseStatusError);
  }
}

TEST_GROUP_BASE(Command_DapJtagIDCODE_PortIsConnected, DapJtagIDCODE) {
  void setup() {
    newCmsisDapMock();
    attachPort();
    connectPortJtag();
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }
  void setMock(int jtag_index, uint32_t idcode, bool result = true) {
    mock().expectOneCall("DapPort::readJtagIdcode")
        .withParameter("jtag_index", jtag_index)
        .withOutputParameterReturning("idcode", &idcode, sizeof(idcode))
        .andReturnValue(result ? 1 : 0);
  }
};

TEST(Command_DapJtagIDCODE_PortIsConnected, Ok) {
  for (int i = 0; i <= 0xff; i++) {
    setCommand(i);
    uint32_t idcode = (i + 1) * (i + 2);
    setMock(i, idcode);
    commandCmsisDap();
    checkResponse(CmsisDap::kResponseStatusOk, idcode);
  }
}

TEST(Command_DapJtagIDCODE_PortIsConnected, Error) {
  for (int i = 0; i <= 0xff; i++) {
    setCommand(i);
    uint32_t idcode = (i + 1) * (i + 2);
    setMock(i, idcode, false);
    commandCmsisDap();
    checkResponse(CmsisDap::kResponseStatusError);
  }
}

}  // namespace dbger
