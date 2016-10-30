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

struct Command_DapTransfer_PortSwdIsConnected
    : public DapTransfer, DapTransferMockSwd {
  Command_DapTransfer_PortSwdIsConnected() {
    newCmsisDapMock();
    attachPortSwd();
    connectPortSwd();
  }
};

TEST_GROUP_BASE(Command_DapTransfer_PortSwdIsConnected_Count_1,
                Command_DapTransfer_PortSwdIsConnected) {
  void setup() {
    allocateCommand(8);
    transfer = new Transfer[1];
  }
};

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ReadApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x3456789a;
  transfer[0].response_data = 0x456789ab;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x2 << 2);
  transfer[0].transfer_data = 0x56789abc;
  transfer[0].response_data = 0x6789abcd;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x3 << 2);
  transfer[0].transfer_data = 0x789abcde;
  transfer[0].response_data = 0x89abcdef;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ReadApacc_Error) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0, CmsisDap::kTransferAckError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0, CmsisDap::kTransferAckError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ReadApacc_Wait) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0, CmsisDap::kTransferAckWait);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckWait);

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0, CmsisDap::kTransferAckWait);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckWait);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ReadApacc_Fault) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0, CmsisDap::kTransferAckFault);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckFault);

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0, CmsisDap::kTransferAckFault);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckFault);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ReadApacc_NoAck) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0, CmsisDap::kTransferNoAck);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferNoAck);

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0, CmsisDap::kTransferNoAck);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferNoAck);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1,
     ReadApacc_SwdProtocolError) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0, CmsisDap::kTransferSwdProtocolError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferSwdProtocolError);

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0, CmsisDap::kTransferSwdProtocolError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferSwdProtocolError);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ReadDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0xdcba9876;
  transfer[0].response_data = 0xcba98765;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x2 << 2);
  transfer[0].transfer_data = 0xba987654;
  transfer[0].response_data = 0xa9876543;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x3 << 2);
  transfer[0].transfer_data = 0x98765432;
  transfer[0].response_data = 0x87654321;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ReadDpacc_Error) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0, CmsisDap::kTransferAckError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ReadDpacc_Wait) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0, CmsisDap::kTransferAckWait);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckWait);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ReadDpacc_Fault) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0, CmsisDap::kTransferAckFault);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckFault);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ReadDpacc_NoAck) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0, CmsisDap::kTransferNoAck);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferNoAck);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1,
     ReadDpacc_SwdProtocolError) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0, CmsisDap::kTransferSwdProtocolError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferSwdProtocolError);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, WriteApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0xdcba9876;
  transfer[0].response_data = 0xcba98765;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x2 << 2);
  transfer[0].transfer_data = 0xba987654;
  transfer[0].response_data = 0xa9876543;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x3 << 2);
  transfer[0].transfer_data = 0x98765432;
  transfer[0].response_data = 0x87654321;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, WriteDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0xdcba9876;
  transfer[0].response_data = 0xcba98765;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x2 << 2);
  transfer[0].transfer_data = 0xba987654;
  transfer[0].response_data = 0xa9876543;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x3 << 2);
  transfer[0].transfer_data = 0x98765432;
  transfer[0].response_data = 0x87654321;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, Write_Error) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0, CmsisDap::kTransferAckError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0xdcba9876;
  transfer[0].response_data = 0xcba98765;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0, CmsisDap::kTransferAckError);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckError, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, Write_Wait) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0, CmsisDap::kTransferAckWait);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckWait);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0xdcba9876;
  transfer[0].response_data = 0xcba98765;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0, CmsisDap::kTransferAckWait);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckWait, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, Write_Fault) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0, CmsisDap::kTransferAckFault);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckFault);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0xdcba9876;
  transfer[0].response_data = 0xcba98765;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0, CmsisDap::kTransferAckFault);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckFault, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, Write_NoAck) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0, CmsisDap::kTransferNoAck);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferNoAck);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0xdcba9876;
  transfer[0].response_data = 0xcba98765;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0, CmsisDap::kTransferNoAck);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferNoAck, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, Write_SwdProtocolError) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xfedcba98;
  transfer[0].response_data = 0xedcba987;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0, CmsisDap::kTransferSwdProtocolError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferSwdProtocolError);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0xdcba9876;
  transfer[0].response_data = 0xcba98765;
  setCommand(0x5a, 1, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0, CmsisDap::kTransferSwdProtocolError);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferSwdProtocolError, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ValueMatchApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x456789ab;
  transfer[0].response_data = 0x456789ab;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x2 << 2);
  transfer[0].transfer_data = 0x6789abcd;
  transfer[0].response_data = 0x6789abcd;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x3 << 2);
  transfer[0].transfer_data = 0x89abcdef;
  transfer[0].response_data = 0x89abcdef;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ValueMatchApacc_Error) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0, CmsisDap::kTransferAckError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x456789ab;
  transfer[0].response_data = 0x56789abc;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ValueMatchApacc_Wait) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xabcdef01;
  transfer[0].response_data = 0xabcdef01;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0, CmsisDap::kTransferAckWait);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckWait);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x456789ab;
  transfer[0].response_data = 0x56789abc;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckWait);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckWait);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ValueMatchApacc_Fault) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xabcdef01;
  transfer[0].response_data = 0xabcdef01;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0, CmsisDap::kTransferAckFault);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckFault);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x456789ab;
  transfer[0].response_data = 0x56789abc;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckFault);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckFault);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ValueMatchApacc_NoAck) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xabcdef01;
  transfer[0].response_data = 0xabcdef01;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0, CmsisDap::kTransferNoAck);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferNoAck);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x456789ab;
  transfer[0].response_data = 0x56789abc;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferNoAck);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferNoAck);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1,
     ValueMatchApacc_SwdProtocolError) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0xabcdef01;
  transfer[0].response_data = 0xabcdef01;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0, CmsisDap::kTransferSwdProtocolError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferSwdProtocolError);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x456789ab;
  transfer[0].response_data = 0x56789abc;
  setCommand(0x5a, 1, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferSwdProtocolError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferSwdProtocolError);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ValueMatchDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x456789ab;
  transfer[0].response_data = 0x456789ab;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x2 << 2);
  transfer[0].transfer_data = 0x6789abcd;
  transfer[0].response_data = 0x6789abcd;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);

  transfer[0].address = (0x3 << 2);
  transfer[0].transfer_data = 0x89abcdef;
  transfer[0].response_data = 0x89abcdef;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ValueMatchDpacc_Error) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0, CmsisDap::kTransferAckError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckError);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ValueMatchDpacc_Wait) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0, CmsisDap::kTransferAckWait);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckWait);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ValueMatchDpacc_Fault) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0, CmsisDap::kTransferAckFault);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckFault);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, ValueMatchDpacc_NoAck) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0, CmsisDap::kTransferNoAck);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferNoAck);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1,
     ValueMatchDpacc_SwdProtocolError) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  setMockReadDpacc(0, CmsisDap::kTransferSwdProtocolError);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferSwdProtocolError);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_1, MatchMask) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeMatchMask;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckOk);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x3456789a;
  transfer[0].response_data = 0x456789ab;
  setCommand(0x5a, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckOk);

  transfer[0].address = (0x2 << 2);
  transfer[0].transfer_data = 0x56789abc;
  transfer[0].response_data = 0x6789abcd;
  setCommand(0x5a, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckOk);

  transfer[0].address = (0x3 << 2);
  transfer[0].transfer_data = 0x789abcde;
  transfer[0].response_data = 0x89abcdef;
  setCommand(0x5a, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckOk);

  transfer[0].APnDP = false;

  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;
  setCommand(0x5a, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckOk);

  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x3456789a;
  transfer[0].response_data = 0x456789ab;
  setCommand(0x5a, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckOk);

  transfer[0].address = (0x2 << 2);
  transfer[0].transfer_data = 0x56789abc;
  transfer[0].response_data = 0x6789abcd;
  setCommand(0x5a, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckOk);

  transfer[0].address = (0x3 << 2);
  transfer[0].transfer_data = 0x789abcde;
  transfer[0].response_data = 0x89abcdef;
  setCommand(0x5a, 1, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckOk);
}

TEST_GROUP_BASE(Command_DapTransfer_PortSwdIsConnected_Count_2,
                Command_DapTransfer_PortSwdIsConnected) {
  void setup() {
    allocateCommand(13);
    transfer = new Transfer[2];
  }
};

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, ReadApacc_ReadApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, ReadApacc_ReadDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, ReadApacc_WriteApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, ReadApacc_WriteDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ReadApacc_ValueMatchApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(1);
  setMockReadApacc(1, 1, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ReadApacc_ValueMatchDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, ReadApacc_MatchMask) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, ReadDpacc_ReadApacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockDummyReadApacc(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, ReadDpacc_ReadDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, ReadDpacc_WriteApacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, ReadDpacc_WriteDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ReadDpacc_ValueMatchApacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockDummyReadApacc(1);
  setMockReadApacc(1, 1, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ReadDpacc_ValueMatchDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, ReadDpacc_MatchMask) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, WriteApacc_ReadApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockDummyReadApacc(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, WriteApacc_ReadDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, WriteApacc_WriteApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, WriteApacc_WriteDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     WriteApacc_ValueMatchApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockDummyReadApacc(1);
  setMockReadApacc(1, 1, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     WriteApacc_ValueMatchDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, WriteApacc_MatchMask) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, WriteDpacc_ReadApacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockDummyReadApacc(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, WriteDpacc_ReadDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, WriteDpacc_WriteApacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, WriteDpacc_WriteDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     WriteDpacc_ValueMatchApacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockDummyReadApacc(1);
  setMockReadApacc(1, 1, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     WriteDpacc_ValueMatchDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, WriteDpacc_MatchMask) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockWrite(0);
  setMockReadRdbuff(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchApacc_ReadApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  setMockDummyReadApacc(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchApacc_ReadApacc_SameAddress) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x2 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x2 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  setMockDummyReadApacc(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchApacc_ReadDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchApacc_WriteApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchApacc_WriteDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchApacc_ValueMatchApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  setMockDummyReadApacc(1);
  setMockReadApacc(1, 1, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchApacc_ValueMatchApacc_SameAddress) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x1 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x1 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  setMockDummyReadApacc(1);
  setMockReadApacc(1, 1, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchApacc_ValueMatchDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchApacc_MatchMask) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(0);
  setMockReadApacc(0, 0, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchDpacc_ReadApacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockDummyReadApacc(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchDpacc_ReadDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchDpacc_WriteApacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchDpacc_WriteDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchDpacc_ValueMatchApacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockDummyReadApacc(1);
  setMockReadApacc(1, 1, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchDpacc_ValueMatchDpacc) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x23456789;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     ValueMatchDpacc_MatchMask) {
  transfer[0].APnDP = false;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x23456789;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(0);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, transfer);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, MatchMask_ReadApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer[1]);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, MatchMask_ReadDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalRead;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer[1]);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, MatchMask_WriteApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer[1]);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, MatchMask_WriteDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeNormalWrite;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x789abcde;
  transfer[1].response_data = 0x89abcdef;

  setCommand(0x5a, 2, transfer);
  setMockWrite(1);
  setMockReadRdbuff(1);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer[1]);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     MatchMask_ValueMatchApacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0xa9b8c7d6 & 0x12345678;
  transfer[1].response_data = 0xa9b8c7d6;

  setCommand(0x5a, 2, transfer);
  setMockDummyReadApacc(1);
  setMockReadApacc(1, 1, CmsisDap::kTransferAckOk);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer[1]);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2,
     MatchMask_ValueMatchDpacc) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = false;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeValueMatch;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0xf0e1d2c3 & 0x12345678;
  transfer[1].response_data = 0xf0e1d2c3;

  setCommand(0x5a, 2, transfer);
  setMockReadDpacc(1);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer[1]);
}

TEST(Command_DapTransfer_PortSwdIsConnected_Count_2, MatchMask_MatchMask) {
  transfer[0].APnDP = true;
  transfer[0].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[0].address = (0x0 << 2);
  transfer[0].transfer_data = 0x12345678;
  transfer[0].response_data = 0x23456789;

  transfer[1].APnDP = true;
  transfer[1].transfer_request_type = TransferRequest::kTransferTypeMatchMask;
  transfer[1].address = (0x3 << 2);
  transfer[1].transfer_data = 0x12345678;
  transfer[1].response_data = 0x23456789;

  setCommand(0x5a, 2, transfer);
  commandCmsisDap();
  checkResponse(CmsisDap::kTransferAckOk);
}

}  // namespace dbger
