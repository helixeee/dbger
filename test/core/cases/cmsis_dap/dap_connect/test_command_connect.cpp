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

struct DapConnect : public TestCmsisDap {
  DapConnect() {
    allocateCommand();
  }
  void setCommand(int port) {
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapConnect;
    command[1] = port;
  }
  void checkResponse(int port) {
    BYTES_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapConnect, response[0]);
    BYTES_EQUAL(port, response[1]);
  }
};

TEST_GROUP_BASE(Command_DapConnect_PortIsNotAttached, DapConnect) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapConnect_PortIsNotAttached, DefaultMode) {
  setCommand(CmsisDap::kDapPortModeDefault);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeFailed);
}

TEST(Command_DapConnect_PortIsNotAttached, SwdMode) {
  setCommand(CmsisDap::kDapPortModeSwd);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeFailed);
}

TEST(Command_DapConnect_PortIsNotAttached, JtagMode) {
  setCommand(CmsisDap::kDapPortModeJtag);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeFailed);
}

struct DapConnect_PortIsAttached : public DapConnect {
  ~DapConnect_PortIsAttached() {
    mock().checkExpectations();
    mock().clear();
  }

  void setMock(int port, bool result = true) {
    SimpleString call_function_name;
    switch (port) {
      case CmsisDap::kDapPortModeSwd:
        call_function_name = "DapPortSwd::connect";
        break;
      case CmsisDap::kDapPortModeJtag:
        call_function_name = "DapPortJtag::connect";
        break;
    }

    mock().expectOneCall(call_function_name)
        .andReturnValue(result ? 1 : 0);
  }
};

TEST_GROUP_BASE(Command_DapConnect_DefaultSwd_PortIsAttached,
                DapConnect_PortIsAttached) {
  void setup() {
    newCmsisDapMock();
    attachPortSwj();
  }
};

TEST(Command_DapConnect_DefaultSwd_PortIsAttached, DefaultMode) {
  setCommand(CmsisDap::kDapPortModeDefault);
  setMock(CmsisDap::kDapPortModeSwd);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeSwd);
}

TEST(Command_DapConnect_DefaultSwd_PortIsAttached, DefaultMode_Error) {
  setCommand(CmsisDap::kDapPortModeDefault);
  setMock(CmsisDap::kDapPortModeSwd, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeFailed);
}

TEST(Command_DapConnect_DefaultSwd_PortIsAttached, SwdMode) {
  setCommand(CmsisDap::kDapPortModeSwd);
  setMock(CmsisDap::kDapPortModeSwd);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeSwd);
}

TEST(Command_DapConnect_DefaultSwd_PortIsAttached, SwdMode_Error) {
  setCommand(CmsisDap::kDapPortModeSwd);
  setMock(CmsisDap::kDapPortModeSwd, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeFailed);
}

TEST(Command_DapConnect_DefaultSwd_PortIsAttached, JtagMode) {
  setCommand(CmsisDap::kDapPortModeJtag);
  setMock(CmsisDap::kDapPortModeJtag);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeJtag);
}

TEST(Command_DapConnect_DefaultSwd_PortIsAttached, JtagMode_Error) {
  setCommand(CmsisDap::kDapPortModeJtag);
  setMock(CmsisDap::kDapPortModeJtag, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeFailed);
}

TEST(Command_DapConnect_DefaultSwd_PortIsAttached, IllegalPort) {
  for (int i = 3; i <= 0xff; i++) {
    setCommand(i);
    commandCmsisDap();
    checkResponse(CmsisDap::kDapPortModeFailed);
  }
}

TEST_GROUP_BASE(Command_DapConnect_DefaultJtag_PortIsAttached,
                DapConnect_PortIsAttached) {
  void setup() {
    newCmsisDapMock(CmsisDap::kDapPortModeJtag);
    attachPortSwj();
  }
};

TEST(Command_DapConnect_DefaultJtag_PortIsAttached, DefaultMode) {
  setCommand(CmsisDap::kDapPortModeDefault);
  setMock(CmsisDap::kDapPortModeJtag);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeJtag);
}

TEST(Command_DapConnect_DefaultJtag_PortIsAttached, DefaultMode_Error) {
  setCommand(CmsisDap::kDapPortModeDefault);
  setMock(CmsisDap::kDapPortModeJtag, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeFailed);
}

TEST(Command_DapConnect_DefaultJtag_PortIsAttached, SwdMode) {
  setCommand(CmsisDap::kDapPortModeSwd);
  setMock(CmsisDap::kDapPortModeSwd);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeSwd);
}

TEST(Command_DapConnect_DefaultJtag_PortIsAttached, SwdMode_Error) {
  setCommand(CmsisDap::kDapPortModeSwd);
  setMock(CmsisDap::kDapPortModeSwd, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeFailed);
}

TEST(Command_DapConnect_DefaultJtag_PortIsAttached, JtagMode) {
  setCommand(CmsisDap::kDapPortModeJtag);
  setMock(CmsisDap::kDapPortModeJtag);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeJtag);
}

TEST(Command_DapConnect_DefaultJtag_PortIsAttached, JtagMode_Error) {
  setCommand(CmsisDap::kDapPortModeJtag);
  setMock(CmsisDap::kDapPortModeJtag, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kDapPortModeFailed);
}

}  // namespace dbger
