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

struct DapHostStatus : public TestCmsisDap {
  DapHostStatus() {
    allocateCommand(3);
  }
  void setCommand(int type, int status) {
    initCommand();
    command_length = 3;
    command[0] = CmsisDap::kCommandIdDapHostStatus;
    command[1] = type;
    command[2] = status;
  }
  void checkResponse() {
    BYTES_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapHostStatus, response[0]);
    BYTES_EQUAL(0x00, response[1]);
  }
};

TEST_GROUP_BASE(Command_DapHostStatus_IndicatorIsNotAttached, DapHostStatus) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapHostStatus_IndicatorIsNotAttached, AllPattern) {
  for (int i = 0; i <= 0xff; i++) {
    for (int j = 0; j <= 0xff; j++) {
      setCommand(i, j);
      commandCmsisDap();
      checkResponse();
    }
  }
}

TEST_GROUP_BASE(Command_DapHostStatus_IndicatorIsAttached, DapHostStatus) {
  void setup() {
    newCmsisDapMock();
    indicator = new IndicatorMock;
    cmsis_dap->attachIndicator(indicator);
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }

  void setMock(int type, int status, bool result = true) {
    SimpleString call_function_name = "Indicator::";
    switch (type) {
      case CmsisDap::kHostStatusTypeConnect:
        call_function_name += "switchConnect";
        break;
      case CmsisDap::kHostStatusTypeRunning:
        call_function_name += "switchRunning";
        break;
    }

    bool status_parameter = false;
    switch (status) {
      case CmsisDap::kHostStatusOn:
        status_parameter = true;
        break;
      case CmsisDap::kHostStatusOff:
        status_parameter = false;
        break;
    }

    mock().expectOneCall(call_function_name)
        .withParameter("on", status_parameter)
        .andReturnValue(result ? 1 : 0);
  }
};

TEST(Command_DapHostStatus_IndicatorIsAttached, ConnectOff) {
  const int type = CmsisDap::kHostStatusTypeConnect;
  const int status = CmsisDap::kHostStatusOff;

  setCommand(type, status);
  setMock(type, status);
  commandCmsisDap();
  checkResponse();
}

TEST(Command_DapHostStatus_IndicatorIsAttached, ConnectOn) {
  const int type = CmsisDap::kHostStatusTypeConnect;
  const int status = CmsisDap::kHostStatusOn;

  setCommand(type, status);
  setMock(type, status);
  commandCmsisDap();
  checkResponse();
}

TEST(Command_DapHostStatus_IndicatorIsAttached, RunningOff) {
  const int type = CmsisDap::kHostStatusTypeRunning;
  const int status = CmsisDap::kHostStatusOff;

  setCommand(type, status);
  setMock(type, status);
  commandCmsisDap();
  checkResponse();
}

TEST(Command_DapHostStatus_IndicatorIsAttached, RunningOn) {
  const int type = CmsisDap::kHostStatusTypeRunning;
  const int status = CmsisDap::kHostStatusOn;

  setCommand(type, status);
  setMock(type, status);
  commandCmsisDap();
  checkResponse();
}

TEST(Command_DapHostStatus_IndicatorIsAttached, IllegalType) {
  for (int type = 2; type <= 0xff; type++) {
    setCommand(type, CmsisDap::kHostStatusOff);
    commandCmsisDap();
    checkResponse();

    setCommand(type, CmsisDap::kHostStatusOn);
    commandCmsisDap();
    checkResponse();
  }
}

TEST(Command_DapHostStatus_IndicatorIsAttached, IllegalStatus) {
  for (int status = 2; status <= 0xff; status++) {
    setCommand(CmsisDap::kHostStatusTypeConnect, status);
    commandCmsisDap();
    checkResponse();

    setCommand(CmsisDap::kHostStatusTypeRunning, status);
    commandCmsisDap();
    checkResponse();
  }
}

}  // namespace dbger
