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

struct DapJtagConfigure : public TestCmsisDap {
  uint8_t ir_length[255];

  DapJtagConfigure() {
    allocateCommand(257);
  }
  void setCommand() {
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapJtagConfigure;
    command[1] = 0;
  }
  void setCommand(int count) {
    if (count > 0xff || count < 0) {
      FAIL("Error: count");
    }
    initCommand();
    command_length = 2 + count;
    command[0] = CmsisDap::kCommandIdDapJtagConfigure;
    command[1] = count;
    for (int i = 0; i < count; i++) {
      command[i + 2] = ir_length[i];
    }
  }
  void checkResponse(int status) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapJtagConfigure, response[0]);
    BYTES_EQUAL(status & 0xff, response[1]);
  }
};

TEST_GROUP_BASE(Command_DapJtagConfigure_PortIsNotConnected, DapJtagConfigure) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapJtagConfigure_PortIsNotConnected, Default) {
  const int count = 255;
  for (int i = 0; i < count; i++) {
    ir_length[i] = i + 1;
  }

  setCommand(0);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(1);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(255);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST_GROUP_BASE(Command_DapJtagConfigure_PortIsConnected, DapJtagConfigure) {
  bool ir_length_result[255];
  void setup() {
    newCmsisDapMock();
    attachPort();
    connectPortJtag();
    initResult();
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }
  void setMock(int count, bool count_result = true) {
    mock().expectOneCall("DapPort::setNumberOfJtagDevices")
        .withParameter("count", count)
        .andReturnValue(count_result? 1 : 0);
    if (!count_result) {
      return;
    }
    for (int i = 0; i < count; i++) {
      mock().expectOneCall("DapPort::setJtagIrLength")
          .withParameter("index", i)
          .withParameter("ir_length", ir_length[i])
          .andReturnValue(ir_length_result[i] ? 1 : 0);
    }
  }
  void initResult() {
    for (int i = 0; i < 255; i++) {
      ir_length_result[i] = true;
    }
  }
};

TEST(Command_DapJtagConfigure_PortIsConnected, Count_0_Ok) {
  const int count = 255;
  for (int i = 0; i < count; i++) {
    ir_length[i] = i + 1;
  }

  setCommand(0);
  setMock(0);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapJtagConfigure_PortIsConnected, Count_1_Ok) {
  const int count = 255;
  for (int i = 0; i < count; i++) {
    ir_length[i] = i + 1;
  }

  setCommand(1);
  setMock(1);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapJtagConfigure_PortIsConnected, Count_255_Ok) {
  const int count = 255;
  for (int i = 0; i < count; i++) {
    ir_length[i] = i + 1;
  }

  setCommand(255);
  setMock(255);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapJtagConfigure_PortIsConnected, Error) {
  const int count = 255;
  for (int i = 0; i < count; i++) {
    ir_length[i] = i + 1;
  }

  setCommand(255);
  setMock(255, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(255);
  ir_length_result[0] = false;
  setMock(255);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

}  // namespace dbger
