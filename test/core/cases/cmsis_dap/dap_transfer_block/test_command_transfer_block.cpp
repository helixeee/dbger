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

#include "test/core/cases/cmsis_dap/dap_transfer_block/test_transfer_block.h"

namespace dbger {

TEST_GROUP_BASE(Command_DapTransferBlock_PortIsNotConnected, DapTransferBlock) {
  void setup() {
    allocateCommand();
    newCmsisDap();
  }
};

TEST(Command_DapTransferBlock_PortIsNotConnected, Zero) {
  setCommand(0);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

TEST(Command_DapTransferBlock_PortIsNotConnected, Read) {
  transfer_request.APnDP = true;
  transfer_request.address = 0x0;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalRead;

  setCommand(0x5a, 1, &transfer_request, nullptr);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

TEST(Command_DapTransferBlock_PortIsNotConnected, Write) {
  transfer_request.APnDP = true;
  transfer_request.address = 0x0;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalWrite;

  uint32_t transfer_data[1] = {0x12345678};

  setCommand(0x5a, 1, &transfer_request, transfer_data);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

}  // namespace dbger
