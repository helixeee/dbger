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

#ifndef TEST_CORE_CASES_CMSIS_DAP_DAP_INFO_TEST_DAP_INFO_STRING_H_
#define TEST_CORE_CASES_CMSIS_DAP_DAP_INFO_TEST_DAP_INFO_STRING_H_

#include <cstring>

#include "CppUTest/TestHarness.h"

#include "test/core/cases/test_cmsis_dap.h"

namespace dbger {

struct DapInfo_String : public TestCmsisDap {
  void setCommand(int id) {
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapInfo;
    command[1] = id;
  }
  void checkResponse(const char *info) {
    int length = 0;
    if (info != nullptr && (length = std::strlen(info)) != 0) {
      length++;
    }

    CHECK_EQUAL(length + 2, response_length);

    BYTES_EQUAL(CmsisDap::kCommandIdDapInfo, response[0]);
    BYTES_EQUAL(length, response[1]);

    for (int i = 0; i < length; i++) {
      BYTES_EQUAL(info[i], response[2 + i]);
    }
  }
};

}  // namespace dbger

#endif  // TEST_CORE_CASES_CMSIS_DAP_DAP_INFO_TEST_DAP_INFO_STRING_H_
