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

#include <cstring>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "test/core/cases/cmsis_dap/dap_info/test_dap_info_string.h"

namespace dbger {

struct DapInfo_TargetDeviceName : public DapInfo_String {
  DapInfo_TargetDeviceName() {
    allocateCommand();
    setCommand(CmsisDap::kInfoIdTargetDeviceName);
  }
};

TEST_GROUP_BASE(Command_DapInfo_TargetDeviceName, DapInfo_TargetDeviceName) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapInfo_TargetDeviceName, Default) {
  commandCmsisDap();
  checkResponse(nullptr);
}

TEST_GROUP_BASE(Command_DapInfo_TargetDeviceName_Override,
                DapInfo_TargetDeviceName) {
  void setup() {
    newCmsisDapMock();
    newHostAdapterMock();
    cmsis_dap->attachHostAdapter(host_adapter);
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }
  void setMock(const char *info_string,
               int max_command_size = kDefaultMaxCommandSize) {
    int length = 0;
    if (info_string != nullptr) {
      length = std::strlen(info_string) + 1;
    }

    mock().expectOneCall("CmsisDap::getInfoTargetDeviceName")
        .withOutputParameterReturning("target_device_name",
                                      info_string, length);
    mock().expectOneCall("HostAdapter::getMaxPacketSize")
        .andReturnValue(max_command_size);
  }
};

TEST(Command_DapInfo_TargetDeviceName_Override, NullString) {
  const char info[] = "";

  setMock(info);
  commandCmsisDap();
  checkResponse(nullptr);
}

TEST(Command_DapInfo_TargetDeviceName_Override, String) {
  const char info[] = "Target Device Name";

  setMock(info);
  commandCmsisDap();
  checkResponse(info);
}

TEST(Command_DapInfo_TargetDeviceName_Override, LargeStringThanPacketSize) {
  const char info[] = "Target Device Name";
  const int max_command_size = 4;
  const char expected_info[] = "T";

  setMock(info, max_command_size);
  commandCmsisDap();
  checkResponse(expected_info);
}

TEST(Command_DapInfo_TargetDeviceName_Override, SmallPakcetSize) {
  const char info[] = "Target Device Name";
  const int max_command_size = 3;

  setMock(info, max_command_size);
  commandCmsisDap();
  checkResponse(nullptr);
}

}  // namespace dbger
