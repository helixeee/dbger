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

#include "test/core/cases/cmsis_dap/dap_transfer/test_transfer.h"

namespace dbger {

TEST_GROUP_BASE(Command_DapTransfer_PortIsNotConnected, DapTransfer) {
  void setup() {
    allocateCommand();
    newCmsisDap();
  }
};

TEST(Command_DapTransfer_PortIsNotConnected, Zero) {
  setCommand(0);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

TEST(Command_DapTransfer_PortIsNotConnected, Read) {
  Transfer transfer[1];
  transfer[0].APnDP = false;
  transfer[0].address = 0;
  transfer[0].transfer_request_type = Transfer::kTransferTypeNormalRead;

  setCommand(0, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

TEST(Command_DapTransfer_PortIsNotConnected, Write) {
  Transfer transfer[1];
  transfer[0].APnDP = false;
  transfer[0].address = 0;
  transfer[0].transfer_request_type = Transfer::kTransferTypeNormalWrite;
  transfer[0].transfer_data = 0x12345678;

  setCommand(0, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

TEST(Command_DapTransfer_PortIsNotConnected, ValueMatch) {
  Transfer transfer[1];
  transfer[0].APnDP = false;
  transfer[0].address = 0;
  transfer[0].transfer_request_type = Transfer::kTransferTypeValueMatch;
  transfer[0].transfer_data = 0x23456789;

  setCommand(0, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

TEST(Command_DapTransfer_PortIsNotConnected, MatchMask) {
  Transfer transfer[1];
  transfer[0].APnDP = false;
  transfer[0].address = 0;
  transfer[0].transfer_request_type = Transfer::kTransferTypeMatchMask;
  transfer[0].transfer_data = 0x3456789a;

  setCommand(0, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

}  // namespace dbger
