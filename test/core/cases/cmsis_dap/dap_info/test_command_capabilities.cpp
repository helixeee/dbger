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

#include "test/core/cases/test_cmsis_dap.h"

namespace dbger {

struct DapInfo_Capabilities : public TestCmsisDap {
  DapInfo_Capabilities() {
    allocateCommand();
    setCommand();
  }
  void setCommand() {
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapInfo;
    command[1] = CmsisDap::kInfoIdCapabilities;
  }
  void checkResponse(int capabilities) {
    CHECK_EQUAL(3, response_length);

    BYTES_EQUAL(CmsisDap::kCommandIdDapInfo, response[0]);
    BYTES_EQUAL(1, response[1]);
    BYTES_EQUAL(capabilities, response[2]);
  }
};

TEST_GROUP_BASE(Command_DapInfo_Capabilities_PortIsNotAttached,
                DapInfo_Capabilities) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapInfo_Capabilities_PortIsNotAttached, Default) {
  commandCmsisDap();
  checkResponse(CmsisDap::kDapInfoCapabilitiesNone);
}

TEST_GROUP_BASE(Command_DapInfo_Capabilities_PortSwdIsAttached,
                DapInfo_Capabilities) {
  void setup() {
    newCmsisDapMock();
    attachPortSwd();
  }
};

TEST(Command_DapInfo_Capabilities_PortSwdIsAttached, Swd) {
  commandCmsisDap();
  checkResponse(CmsisDap::kDapInfoCapabilitiesSwd);
}

TEST_GROUP_BASE(Command_DapInfo_Capabilities_PortJtagIsAttached,
                DapInfo_Capabilities) {
  void setup() {
    newCmsisDapMock();
    attachPortJtag();
  }
};

TEST(Command_DapInfo_Capabilities_PortJtagIsAttached, Jtag) {
  commandCmsisDap();
  checkResponse(CmsisDap::kDapInfoCapabilitiesJtag);
}

TEST_GROUP_BASE(Command_DapInfo_Capabilities_PortSwjIsAttached,
                DapInfo_Capabilities) {
  void setup() {
    newCmsisDapMock();
    attachPortSwj();
  }
};

TEST(Command_DapInfo_Capabilities_PortSwjIsAttached, Swj) {
  commandCmsisDap();
  checkResponse(CmsisDap::kDapInfoCapabilitiesSwd |
                CmsisDap::kDapInfoCapabilitiesJtag);
}

}  // namespace dbger
