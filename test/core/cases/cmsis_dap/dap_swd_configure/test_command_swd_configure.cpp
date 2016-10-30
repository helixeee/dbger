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

struct DapSwdConfigure : public TestCmsisDap {
  enum TruanaroundClock {
    kTruanaroundClock_1Clock = 0,
    kTruanaroundClock_2Clock = 1,
    kTruanaroundClock_3Clock = 2,
    kTruanaroundClock_4Clock = 3,
    kTruanaroundClock_Default = kTruanaroundClock_1Clock,
  };
  DapSwdConfigure() {
    allocateCommand();
  }

  void setCommand(int turnaround, bool data_phase) {
    if (turnaround > 3 || turnaround < 0) {
      FAIL("Error: DapSwdConfigure turnaround");
    }
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapSwdConfigure;
    command[1] = (turnaround & 0x3) + (data_phase ? (1 << 2) : 0);
  }
  void setCommand(uint8_t configuration) {
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapSwdConfigure;
    command[1] = configuration;
  }
  void checkResponse(int status) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapSwdConfigure, response[0]);
    BYTES_EQUAL(status & 0xff, response[1]);
  }
};

TEST_GROUP_BASE(Command_DapSwdConfigure_PortIsNotConnected, DapSwdConfigure) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapSwdConfigure_PortIsNotConnected, PortIsNotConnected) {
  for (int i = 0; i <= 0xff; i++) {
    setCommand(i);
    commandCmsisDap();
    checkResponse(CmsisDap::kResponseStatusError);
  }
}

TEST_GROUP_BASE(Command_DapSwdConfigure_PortIsConnected, DapSwdConfigure) {
  void setup() {
    newCmsisDapMock();
    attachPort();
    connectPortSwd();
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }

  void setMock(int turnaround, bool data_phase, bool result = true) {
    mock().expectOneCall("DapPort::configurateSwd")
        .withParameter("turnaround_clock", turnaround)
        .withParameter("generate_data_phase", data_phase ? 1 : 0)
        .andReturnValue(result ? 1 : 0);
  }
};

TEST(Command_DapSwdConfigure_PortIsConnected, Ok) {
  for (int i = 0; i <= 0xff; i++) {
    const int turnaround = i & 0x3;
    const int data_phase = ((i & 0x4) != 0) ? true : false;
    setCommand(i);
    setMock(turnaround , data_phase);
    commandCmsisDap();
    checkResponse(CmsisDap::kResponseStatusOk);
  }
}

TEST(Command_DapSwdConfigure_PortIsConnected, Error) {
  for (int i = 0; i <= 0xff; i++) {
    const int turnaround = i & 0x3;
    const int data_phase = ((i & 0x4) != 0) ? true : false;
    setCommand(i);
    setMock(turnaround , data_phase, false);
    commandCmsisDap();
    checkResponse(CmsisDap::kResponseStatusError);
  }
}

}  // namespace dbger
