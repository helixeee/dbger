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

struct DapTransferConfigure : public TestCmsisDap {
  DapTransferConfigure() {
    allocateCommand(6);
  }
  void setCommand(uint8_t idle_cycles,
                  uint16_t wait_retry,
                  uint16_t match_retry) {
    initCommand();
    command_length = 6;
    command[0] = CmsisDap::kCommandIdDapTransferConfigure;
    command[1] = idle_cycles;
    command[2] = wait_retry & 0xff;
    command[3] = (wait_retry >> 8) & 0xff;
    command[4] = match_retry & 0xff;
    command[5] = (match_retry >> 8) & 0xff;
  }
  void checkResponse(int status) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapTransferConfigure, response[0]);
    BYTES_EQUAL(status, response[1]);
  }
};

TEST_GROUP_BASE(Command_DapTransferConfigure, DapTransferConfigure) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapTransferConfigure, Default) {
  setCommand(0, 0, 0);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xff, 0xffff, 0xffff);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

}  // namespace dbger
