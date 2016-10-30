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

struct DapWriteABORT : public TestCmsisDap {
  DapWriteABORT() {
    allocateCommand(6);
  }

  void setCommand(int index, uint32_t abort) {
    initCommand();
    command_length = 6;
    command[0] = CmsisDap::kCommandIdDapWriteABORT;
    command[1] = index;
    command[2] = abort & 0xff;
    command[3] = (abort >> 8) & 0xff;
    command[4] = (abort >> 16) & 0xff;
    command[5] = (abort >> 24) & 0xff;
  }
  void checkResponse(int status) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapWriteABORT, response[0]);
    BYTES_EQUAL(status, response[1]);
  }
};

TEST_GROUP_BASE(Command_DapWriteABORT_PortIsNotConnected, DapWriteABORT) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapWriteABORT_PortIsNotConnected, Default) {
  setCommand(0x00, 0x00000000);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0xff, 0xffffffff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0x12, 0x3456789a);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST_GROUP_BASE(Command_DapWriteABORT_PortIsConnected, DapWriteABORT) {
  void setup() {
    newCmsisDapMock();
    attachPort();
    connectPortSwd();
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }

  void setMock(int index, uint32_t abort, bool result = true) {
    mock().expectOneCall("DapPort::writeAbort")
        .withParameter("index", index)
        .withParameter("abort", abort)
        .andReturnValue(result ? 1 : 0);
  }
};

TEST(Command_DapWriteABORT_PortIsConnected, Ok) {
  setCommand(0x00, 0x00000000);
  setMock(0x00, 0x00000000);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xff, 0xffffffff);
  setMock(0xff, 0xffffffff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0x23, 0x456789ab);
  setMock(0x23, 0x456789ab);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapWriteABORT_PortIsConnected, Error) {
  setCommand(0x00, 0x00000000);
  setMock(0x00, 0x00000000, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0xff, 0xffffffff);
  setMock(0xff, 0xffffffff, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0x45, 0x6789abcd);
  setMock(0x45, 0x6789abcd, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

}  // namespace dbger
