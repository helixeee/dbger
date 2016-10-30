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

struct DapDelay : public TestCmsisDap {
  DapDelay() {
    allocateCommand(3);
  }
  void setCommand(uint16_t delay) {
    initCommand();
    command_length = 3;
    command[0] = CmsisDap::kCommandIdDapDelay;
    command[1] = delay & 0xff;
    command[2] = (delay >> 8) & 0xff;
  }
  void checkResponse(int status) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapDelay, response[0]);
    BYTES_EQUAL(status, response[1]);
  }
};

TEST_GROUP_BASE(Command_DapDelay, DapDelay) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapDelay, Default) {
  setCommand(0x0000);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0x00ff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xff00);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xffff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xa5f0);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST_GROUP_BASE(Command_DapDelay_Override, DapDelay) {
  void setup() {
    newCmsisDapMock();
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }

  void setMock(uint16_t delay, bool result = true) {
    mock().expectOneCall("CmsisDap::delay")
        .withParameter("delay", delay)
        .andReturnValue(result ? 1 : 0);
  }
};

TEST(Command_DapDelay_Override, Ok) {
  setCommand(0x0000);
  setMock(0x0000);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xffff);
  setMock(0xffff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xa5f0);
  setMock(0xa5f0);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapDelay_Override, Error) {
  setCommand(0x0000);
  setMock(0x0000, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0xffff);
  setMock(0xffff, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0xa5f0);
  setMock(0xa5f0, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

}  // namespace dbger
