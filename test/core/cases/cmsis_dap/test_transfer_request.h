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

#ifndef TEST_CORE_CASES_CMSIS_DAP_TEST_TRANSFER_REQUEST_H_
#define TEST_CORE_CASES_CMSIS_DAP_TEST_TRANSFER_REQUEST_H_

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "test/core/cases/test_cmsis_dap.h"

struct TransferRequest {
  enum TransferRequestType {
    kTransferTypeNormalRead,
    kTransferTypeNormalWrite,
    kTransferTypeValueMatch,
    kTransferTypeMatchMask,
  };
  bool APnDP;
  int address;
  int transfer_request_type;
  uint8_t getTransferRequest() {
    uint8_t transfer_request = 0;
    transfer_request |= APnDP ? (1 << 0) : 0;
    transfer_request |= address & (0x3 << 2);
    switch (transfer_request_type) {
      case kTransferTypeNormalRead:
        transfer_request |= (1 << 1);
        break;
      case kTransferTypeNormalWrite:
        transfer_request |= (0 << 1);
        break;
      case kTransferTypeValueMatch:
        transfer_request |= (1 << 1);
        transfer_request |= (1 << 4);
        break;
      case kTransferTypeMatchMask:
        transfer_request |= (0 << 1);
        transfer_request |= (1 << 5);
        break;
    }
    return transfer_request;
  }
};

#endif  // TEST_CORE_CASES_CMSIS_DAP_TEST_TRANSFER_REQUEST_H_
