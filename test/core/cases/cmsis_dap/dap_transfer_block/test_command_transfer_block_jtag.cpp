/**
 * Copyright (C) 2016 helixeee
 *
 * This file is part of dbger.
 *
 * dbger is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either versionof the License, or
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

struct Command_DapTransferBlock_PortJtagIsConnected
    : public DapTransferBlockMockJtag {
  Command_DapTransferBlock_PortJtagIsConnected() {
    newCmsisDapMock();
    attachPortJtag();
    connectPortJtag();
  }
  virtual ~Command_DapTransferBlock_PortJtagIsConnected() {}
};

TEST_GROUP_BASE(Command_DapTransferBlock_PortJtagIsConnected_Count_1,
                Command_DapTransferBlock_PortJtagIsConnected) {
  void setup() {
    allocateCommand(9);
    transfer_data = new uint32_t[1];
    response_data = new uint32_t[1];
  }
};

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_1, ReadApacc) {
  transfer_request.APnDP = true;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalRead;

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x12345678;
  response_data[0] = 0x23456789;
  setCommand(0x5a, 1, &transfer_request, transfer_data);
  setMockDummyReadApacc();
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x3456789a;
  response_data[0] = 0x456789ab;
  setCommand(0xa5, 1, &transfer_request, transfer_data);
  setMockDummyReadApacc();
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x56789abc;
  response_data[0] = 0x6789abcd;
  setCommand(0x0f, 1, &transfer_request, transfer_data);
  setMockDummyReadApacc();
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x789abcde;
  response_data[0] = 0x89abcdef;
  setCommand(0xf0, 1, &transfer_request, transfer_data);
  setMockDummyReadApacc();
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_1, ReadDpacc) {
  transfer_request.APnDP = false;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalRead;

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x12345678;
  response_data[0] = 0x23456789;
  setCommand(0x5a, 1, &transfer_request, transfer_data);
  setMockDummyReadDpacc();
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x3456789a;
  response_data[0] = 0x456789ab;
  setCommand(0xa5, 1, &transfer_request, transfer_data);
  setMockDummyReadDpacc();
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x56789abc;
  response_data[0] = 0x6789abcd;
  setCommand(0x0f, 1, &transfer_request, transfer_data);
  setMockDummyReadDpacc();
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x789abcde;
  response_data[0] = 0x89abcdef;
  setCommand(0xf0, 1, &transfer_request, transfer_data);
  setMockDummyReadDpacc();
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_1, WriteApacc) {
  transfer_request.APnDP = true;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalWrite;

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x12345678;
  response_data[0] = 0x23456789;
  setCommand(0x5a, 1, &transfer_request, transfer_data);
  setMockWriteApacc(transfer_data[0]);
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x3456789a;
  response_data[0] = 0x456789ab;
  setCommand(0xa5, 1, &transfer_request, transfer_data);
  setMockWriteApacc(transfer_data[0]);
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x56789abc;
  response_data[0] = 0x6789abcd;
  setCommand(0x0f, 1, &transfer_request, transfer_data);
  setMockWriteApacc(transfer_data[0]);
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x789abcde;
  response_data[0] = 0x89abcdef;
  setCommand(0xf0, 1, &transfer_request, transfer_data);
  setMockWriteApacc(transfer_data[0]);
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_1, WriteDpacc) {
  transfer_request.APnDP = false;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalWrite;

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x12345678;
  response_data[0] = 0x23456789;
  setCommand(0x5a, 1, &transfer_request, transfer_data);
  setMockWriteDpacc(transfer_data[0]);
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x3456789a;
  response_data[0] = 0x456789ab;
  setCommand(0xa5, 1, &transfer_request, transfer_data);
  setMockWriteDpacc(transfer_data[0]);
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x56789abc;
  response_data[0] = 0x6789abcd;
  setCommand(0x0f, 1, &transfer_request, transfer_data);
  setMockWriteDpacc(transfer_data[0]);
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x789abcde;
  response_data[0] = 0x89abcdef;
  setCommand(0xf0, 1, &transfer_request, transfer_data);
  setMockWriteDpacc(transfer_data[0]);
  setMockReadRdbuff(&response_data[0]);
  commandCmsisDap();
  checkResponse(1, CmsisDap::kTransferAckOk, &transfer_request, response_data);
}


TEST_GROUP_BASE(Command_DapTransferBlock_PortJtagIsConnected_Count_2,
                Command_DapTransferBlock_PortJtagIsConnected) {
  void setup() {
    allocateCommand(13);
    transfer_data = new uint32_t[2];
    response_data = new uint32_t[2];
  }
};

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_2, ReadApacc) {
  transfer_request.APnDP = true;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalRead;

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x12345678;
  response_data[0] = 0x23456789;
  transfer_data[1] = 0x3456789a;
  response_data[1] = 0x456789ab;
  setCommand(0xa5, 2, &transfer_request, transfer_data);
  setMockDummyReadApacc();
  setMockReadApacc(&response_data[0]);
  setMockReadRdbuff(&response_data[1]);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_2, ReadDpacc) {
  transfer_request.APnDP = false;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalRead;

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x12345678;
  response_data[0] = 0x23456789;
  transfer_data[1] = 0x3456789a;
  response_data[1] = 0x456789ab;
  setCommand(0x5a, 2, &transfer_request, transfer_data);
  setMockDummyReadDpacc();
  setMockReadDpacc(&response_data[0]);
  setMockReadRdbuff(&response_data[1]);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_2, WriteApacc) {
  transfer_request.APnDP = true;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalWrite;

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x12345678;
  response_data[0] = 0x23456789;
  transfer_data[1] = 0x3456789a;
  response_data[1] = 0x456789ab;
  setCommand(0x5a, 2, &transfer_request, transfer_data);
  setMockWriteApacc(transfer_data[0]);
  setMockWriteApacc(transfer_data[1]);
  setMockReadRdbuff(&response_data[1]);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_2, WriteDpacc) {
  transfer_request.APnDP = false;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalWrite;

  transfer_request.address = (0x1 << 2);
  transfer_data[0] = 0x12345678;
  response_data[0] = 0x23456789;
  transfer_data[1] = 0x3456789a;
  response_data[1] = 0x456789ab;
  setCommand(0x5a, 2, &transfer_request, transfer_data);
  setMockWriteDpacc(transfer_data[0]);
  setMockWriteDpacc(transfer_data[1]);
  setMockReadRdbuff(&response_data[1]);
  commandCmsisDap();
  checkResponse(2, CmsisDap::kTransferAckOk, &transfer_request, response_data);
}

TEST_GROUP_BASE(Command_DapTransferBlock_PortJtagIsConnected_Count_255,
                Command_DapTransferBlock_PortJtagIsConnected) {
  void setup() {
    allocateCommand(5 + 255 * 4);
    transfer_data = new uint32_t[255];
    response_data = new uint32_t[255];
  }
};

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_255, ReadApacc) {
  transfer_request.APnDP = true;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalRead;

  transfer_request.address = (0x1 << 2);
  for (int i = 0; i < 255; i++) {
    transfer_data[i] = 0x12345678 + i;
    response_data[i] = 0x23456789 + i;
  }
  setCommand(0xa5, 255, &transfer_request, transfer_data);
  setMockDummyReadApacc();
  for (int i = 0; i < 255 - 1; i++) {
    setMockReadApacc(&response_data[i]);
  }
  setMockReadRdbuff(&response_data[255 - 1]);
  commandCmsisDap();
  checkResponse(255, CmsisDap::kTransferAckOk,
                &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_255, ReadDpacc) {
  transfer_request.APnDP = false;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalRead;

  transfer_request.address = (0x1 << 2);
  for (int i = 0; i < 255; i++) {
    transfer_data[i] = 0x12345678 + i;
    response_data[i] = 0x23456789 + i;
  }
  setCommand(0xa5, 255, &transfer_request, transfer_data);
  setMockDummyReadDpacc();
  for (int i = 0; i < 255 - 1; i++) {
    setMockReadDpacc(&response_data[i]);
  }
  setMockReadRdbuff(&response_data[255 - 1]);
  commandCmsisDap();
  checkResponse(255, CmsisDap::kTransferAckOk,
                &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_255, WriteApacc) {
  transfer_request.APnDP = true;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalWrite;

  transfer_request.address = (0x1 << 2);
  for (int i = 0; i < 255; i++) {
    transfer_data[i] = 0x12345678 + i;
    response_data[i] = 0x23456789 + i;
  }
  setCommand(0xa5, 255, &transfer_request, transfer_data);
  for (int i = 0; i < 255; i++) {
    setMockWriteApacc(transfer_data[i]);
  }
  setMockReadRdbuff(&response_data[255 - 1]);
  commandCmsisDap();
  checkResponse(255, CmsisDap::kTransferAckOk,
                &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_255, WriteDpacc) {
  transfer_request.APnDP = false;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalWrite;

  transfer_request.address = (0x1 << 2);
  for (int i = 0; i < 255; i++) {
    transfer_data[i] = 0x12345678 + i;
    response_data[i] = 0x23456789 + i;
  }
  setCommand(0xa5, 255, &transfer_request, transfer_data);
  for (int i = 0; i < 255; i++) {
    setMockWriteDpacc(transfer_data[i]);
  }
  setMockReadRdbuff(&response_data[255 - 1]);
  commandCmsisDap();
  checkResponse(255, CmsisDap::kTransferAckOk,
                &transfer_request, response_data);
}

TEST_GROUP_BASE(Command_DapTransferBlock_PortJtagIsConnected_Count_256,
                Command_DapTransferBlock_PortJtagIsConnected) {
  void setup() {
    allocateCommand(5 + 256 * 4);
    transfer_data = new uint32_t[256];
    response_data = new uint32_t[256];
  }
};

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_256, ReadApacc) {
  transfer_request.APnDP = true;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalRead;

  transfer_request.address = (0x1 << 2);
  for (int i = 0; i < 256; i++) {
    transfer_data[i] = 0x12345678 + i;
    response_data[i] = 0x23456789 + i;
  }
  setCommand(0xa5, 256, &transfer_request, transfer_data);
  setMockDummyReadApacc();
  for (int i = 0; i < 256 - 1; i++) {
    setMockReadApacc(&response_data[i]);
  }
  setMockReadRdbuff(&response_data[256 - 1]);
  commandCmsisDap();
  checkResponse(256, CmsisDap::kTransferAckOk,
                &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_256, ReadDpacc) {
  transfer_request.APnDP = false;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalRead;

  transfer_request.address = (0x1 << 2);
  for (int i = 0; i < 256; i++) {
    transfer_data[i] = 0x12345678 + i;
    response_data[i] = 0x23456789 + i;
  }
  setCommand(0xa5, 256, &transfer_request, transfer_data);
  setMockDummyReadDpacc();
  for (int i = 0; i < 256 - 1; i++) {
    setMockReadDpacc(&response_data[i]);
  }
  setMockReadRdbuff(&response_data[256 - 1]);
  commandCmsisDap();
  checkResponse(256, CmsisDap::kTransferAckOk,
                &transfer_request, response_data);
}


TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_256, WriteApacc) {
  transfer_request.APnDP = true;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalWrite;

  transfer_request.address = (0x1 << 2);
  for (int i = 0; i < 256; i++) {
    transfer_data[i] = 0x12345678 + i;
    response_data[i] = 0x23456789 + i;
  }
  setCommand(0xa5, 256, &transfer_request, transfer_data);
  for (int i = 0; i < 256; i++) {
    setMockWriteApacc(transfer_data[i]);
  }
  setMockReadRdbuff(&response_data[256 - 1]);
  commandCmsisDap();
  checkResponse(256, CmsisDap::kTransferAckOk,
                &transfer_request, response_data);
}

TEST(Command_DapTransferBlock_PortJtagIsConnected_Count_256, WriteDpacc) {
  transfer_request.APnDP = false;
  transfer_request.transfer_request_type =
      TransferRequest::kTransferTypeNormalWrite;

  transfer_request.address = (0x1 << 2);
  for (int i = 0; i < 256; i++) {
    transfer_data[i] = 0x12345678 + i;
    response_data[i] = 0x23456789 + i;
  }
  setCommand(0xa5, 256, &transfer_request, transfer_data);
  for (int i = 0; i < 256; i++) {
    setMockWriteDpacc(transfer_data[i]);
  }
  setMockReadRdbuff(&response_data[256 - 1]);
  commandCmsisDap();
  checkResponse(256, CmsisDap::kTransferAckOk,
                &transfer_request, response_data);
}

}  // namespace dbger
