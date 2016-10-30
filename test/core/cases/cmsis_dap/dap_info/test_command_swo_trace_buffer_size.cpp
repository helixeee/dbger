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

TEST_GROUP_BASE(Command_DapInfo_SwoTraceBufferSize, TestCmsisDap) {
  void setup() {
    newCmsisDap();

    allocateCommand();
    setCommand();
  }
  void setCommand() {
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapInfo;
    command[1] = CmsisDap::kInfoIdSwoTraceBufferSize;
  }
  void checkResponse(uint32_t buffer_size) {
    CHECK_EQUAL(6, response_length);

    BYTES_EQUAL(CmsisDap::kCommandIdDapInfo, response[0]);
    BYTES_EQUAL(4, response[1]);
    BYTES_EQUAL(buffer_size & 0xff, response[2]);
    BYTES_EQUAL((buffer_size >> 8) & 0xff, response[3]);
    BYTES_EQUAL((buffer_size >> 16) & 0xff, response[4]);
    BYTES_EQUAL((buffer_size >> 24) & 0xff, response[5]);
  }
};

TEST(Command_DapInfo_SwoTraceBufferSize, Default) {
  commandCmsisDap();

  checkResponse(0);
}

}  // namespace dbger
