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

struct DapResetTarget : public TestCmsisDap {
  DapResetTarget() {
    allocateCommand(3);
  }

  void setCommand() {
    initCommand();
    command_length = 1;
    command[0] = CmsisDap::kCommandIdDapResetTarget;
  }
  void checkResponse(int status, int execute) {
    CHECK_EQUAL(3, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapResetTarget, response[0]);
    BYTES_EQUAL(status, response[1]);
    BYTES_EQUAL(execute, response[2]);
  }
};

TEST_GROUP_BASE(Command_DapResetTarget_PortIsNotConnected, DapResetTarget) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapResetTarget_PortIsNotConnected, Default) {
  setCommand();
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk,
                CmsisDap::kResetSequenceNotImplemented);
}

TEST_GROUP_BASE(Command_DapResetTarget_PortIsConnected, DapResetTarget) {
  void setup() {
    newCmsisDapMock();
    attachPort();
    connectPortSwd();
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }

  void setMock(bool implemented, bool result = true) {
    mock().expectOneCall("DapPort::isImplementedResetSequence")
        .andReturnValue(implemented ? 1 : 0);
    if (!implemented) {
      return;
    }
    mock().expectOneCall("DapPort::reset")
        .andReturnValue(result ? 1 : 0);
  }
};

TEST(Command_DapResetTarget_PortIsConnected, NotImplemented) {
  setCommand();
  setMock(false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk,
                CmsisDap::kResetSequenceNotImplemented);
}

TEST(Command_DapResetTarget_PortIsConnected, Implemented_Ok) {
  setCommand();
  setMock(true);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk,
                CmsisDap::kResetSequenceImplemented);
}

TEST(Command_DapResetTarget_PortIsConnected, Implemented_Error) {
  setCommand();
  setMock(true, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError,
                CmsisDap::kResetSequenceImplemented);
}

}  // namespace dbger
