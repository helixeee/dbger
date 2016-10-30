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

struct DapDisconnect : public TestCmsisDap {
  DapDisconnect() {
    allocateCommand();
  }
  void setCommand() {
    initCommand();
    command_length = 1;
    command[0] = CmsisDap::kCommandIdDapDisconnect;
  }
  void checkResponse(int status) {
    BYTES_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapDisconnect, response[0]);
    BYTES_EQUAL(status, response[1]);
  }
};

TEST_GROUP_BASE(Command_DapDisconnect_PortIsNotConnected, DapDisconnect) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapDisconnect_PortIsNotConnected, Default) {
  setCommand();
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

struct DapDisconnect_PortSwjIsAttached : public DapDisconnect {
  DapDisconnect_PortSwjIsAttached() {
    newCmsisDapMock();
    attachPortSwj();
  }
  ~DapDisconnect_PortSwjIsAttached() {
    mock().checkExpectations();
    mock().clear();
  }

  void setMock(int port, bool result = true) {
    SimpleString call_function_name;
    switch (port) {
      case CmsisDap::kDapPortModeSwd:
        call_function_name = "DapPortSwd::disconnect";
        break;
      case CmsisDap::kDapPortModeJtag:
        call_function_name = "DapPortJtag::disconnect";
        break;
      default:
        return;
    }

    mock().expectOneCall(call_function_name)
        .andReturnValue(result ? 1 : 0);
  }
};

TEST_GROUP_BASE(Command_DapDisconnect_PortSwdIsConnected,
                DapDisconnect_PortSwjIsAttached) {
  void setup() {
    connectPortSwd();
  }
};

TEST(Command_DapDisconnect_PortSwdIsConnected, Ok) {
  setCommand();
  setMock(CmsisDap::kDapPortModeSwd);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapDisconnect_PortSwdIsConnected, Error) {
  setCommand();
  setMock(CmsisDap::kDapPortModeSwd, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST(Command_DapDisconnect_PortSwdIsConnected, Repeat) {
  setCommand();
  for (int i = 0; i < 3; i++) {
    setMock(CmsisDap::kDapPortModeSwd, false);
    commandCmsisDap();
    checkResponse(CmsisDap::kResponseStatusError);
  }

  setMock(CmsisDap::kDapPortModeSwd);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  for (int i = 0; i < 3; i++) {
    commandCmsisDap();
    checkResponse(CmsisDap::kResponseStatusError);
  }
}

TEST_GROUP_BASE(Command_DapDisconnect_PortJtagIsConnected,
                DapDisconnect_PortSwjIsAttached) {
  void setup() {
    connectPortJtag();
  }
};

TEST(Command_DapDisconnect_PortJtagIsConnected, Ok) {
  setCommand();
  setMock(CmsisDap::kDapPortModeJtag);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapDisconnect_PortJtagIsConnected, Error) {
  setCommand();
  setMock(CmsisDap::kDapPortModeJtag, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST(Command_DapDisconnect_PortJtagIsConnected, Repeat) {
  setCommand();

  for (int i = 0; i < 3; i++) {
    setMock(CmsisDap::kDapPortModeJtag, false);
    commandCmsisDap();
    checkResponse(CmsisDap::kResponseStatusError);
  }

  setMock(CmsisDap::kDapPortModeJtag);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  for (int i = 0; i < 3; i++) {
    commandCmsisDap();
    checkResponse(CmsisDap::kResponseStatusError);
  }
}

}  // namespace dbger
