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

struct DapSwjClock : public TestCmsisDap {
  DapSwjClock() {
    allocateCommand(5);
  }
  void setCommand(uint32_t clock) {
    initCommand();
    command_length = 5;
    command[0] = CmsisDap::kCommandIdDapSwjClock;
    command[1] = clock & 0xff;
    command[2] = (clock >> 8) & 0xff;
    command[3] = (clock >> 16) & 0xff;
    command[4] = (clock >> 24) & 0xff;
  }

  void checkResponse(int status) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapSwjClock, response[0]);
    BYTES_EQUAL(status, response[1]);
  }
};

TEST_GROUP_BASE(Command_DapSwjClock_PortIsNotAttached, DapSwjClock) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapSwjClock_PortIsNotAttached, Default) {
  setCommand(0x00000000);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0xffffffff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0x12345678);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

struct DapSwjClock_PortIsAttached : public DapSwjClock {
  DapSwjClock_PortIsAttached() {
    newCmsisDapMock();
  }
  ~DapSwjClock_PortIsAttached() {
    mock().checkExpectations();
    mock().clear();
  }
  void setMock(uint32_t clock, bool result = true) {
    mock().expectOneCall("DapPort::setClock")
        .withParameter("clock", clock)
        .andReturnValue(result ? 1 : 0);
  }
  void setMockSwd(uint32_t clock, bool result = true) {
    mock().expectOneCall("DapPortSwd::setClock")
        .withParameter("clock", clock)
        .andReturnValue(result ? 1 : 0);
  }
  void setMockJtag(uint32_t clock, bool result = true) {
    mock().expectOneCall("DapPortJtag::setClock")
        .withParameter("clock", clock)
        .andReturnValue(result ? 1 : 0);
  }
};

TEST_GROUP_BASE(Command_DapSwjClock_PortSwdIsAttached,
                DapSwjClock_PortIsAttached) {
  void setup() {
    attachPortSwd();
  }
};

TEST(Command_DapSwjClock_PortSwdIsAttached, Ok) {
  setCommand(0x00000000);
  setMockSwd(0x00000000);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xffffffff);
  setMockSwd(0xffffffff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0x12345678);
  setMockSwd(0x12345678);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapSwjClock_PortSwdIsAttached, Error) {
  setCommand(0x00000000);
  setMockSwd(0x00000000, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0xffffffff);
  setMockSwd(0xffffffff, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0x12345678);
  setMockSwd(0x12345678, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST_GROUP_BASE(Command_DapSwjClock_PortJtagIsAttached,
                DapSwjClock_PortIsAttached) {
  void setup() {
    attachPortJtag();
  }
};

TEST(Command_DapSwjClock_PortJtagIsAttached, Ok) {
  setCommand(0x00000000);
  setMockJtag(0x00000000);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xffffffff);
  setMockJtag(0xffffffff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0x12345678);
  setMockJtag(0x12345678);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapSwjClock_PortJtagIsAttached, Error) {
  setCommand(0x00000000);
  setMockJtag(0x00000000, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0xffffffff);
  setMockJtag(0xffffffff, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0x12345678);
  setMockJtag(0x12345678, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST_GROUP_BASE(Command_DapSwjClock_PortSwjIsAttached,
                DapSwjClock_PortIsAttached) {
  void setup() {
    attachPortSwj();
  }
};

TEST(Command_DapSwjClock_PortSwjIsAttached, Ok) {
  setCommand(0x00000000);
  setMockSwd(0x00000000);
  setMockJtag(0x00000000);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xffffffff);
  setMockSwd(0xffffffff);
  setMockJtag(0xffffffff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0x12345678);
  setMockSwd(0x12345678);
  setMockJtag(0x12345678);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapSwjClock_PortSwjIsAttached, Error) {
  setCommand(0x00000000);
  setMockSwd(0x00000000, false);
  setMockJtag(0x00000000, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0xffffffff);
  setMockSwd(0xffffffff, false);
  setMockJtag(0xffffffff, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(0x12345678);
  setMockSwd(0x12345678, false);
  setMockJtag(0x12345678, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST(Command_DapSwjClock_PortSwjIsAttached, Swd_Error) {
  setCommand(0x00000000);
  setMockSwd(0x00000000, false);
  setMockJtag(0x00000000);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xffffffff);
  setMockSwd(0xffffffff, false);
  setMockJtag(0xffffffff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0x12345678);
  setMockSwd(0x12345678, false);
  setMockJtag(0x12345678);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapSwjClock_PortSwjIsAttached, Jtag_Error) {
  setCommand(0x00000000);
  setMockSwd(0x00000000);
  setMockJtag(0x00000000, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xffffffff);
  setMockSwd(0xffffffff);
  setMockJtag(0xffffffff, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0x12345678);
  setMockSwd(0x12345678);
  setMockJtag(0x12345678, false);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

}  // namespace dbger
