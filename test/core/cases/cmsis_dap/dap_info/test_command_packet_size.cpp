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

#include "CppUTestExt/MockSupport.h"

#include "test/core/cases/test_cmsis_dap.h"

namespace dbger {

struct DapInfo_PacketSize : public TestCmsisDap {
  DapInfo_PacketSize() {
    newCmsisDap();

    allocateCommand();
    setCommand();
  }
  void setCommand() {
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapInfo;
    command[1] = CmsisDap::kInfoIdPacketSize;
  }
  void checkResponse(uint16_t packet_size) {
    CHECK_EQUAL(4, response_length);

    BYTES_EQUAL(CmsisDap::kCommandIdDapInfo, response[0]);
    BYTES_EQUAL(2, response[1]);
    BYTES_EQUAL(packet_size & 0xff, response[2]);
    BYTES_EQUAL((packet_size >> 8) & 0xff, response[3]);
  }
};

TEST_GROUP_BASE(Command_DapInfo_PacketSize, DapInfo_PacketSize) {
};

TEST(Command_DapInfo_PacketSize, Default) {
  commandCmsisDap();

  checkResponse(0);
}

TEST_GROUP_BASE(Command_DapInfo_PacketSize_Override, DapInfo_PacketSize) {
  void setup() {
    newHostAdapterMock();
    cmsis_dap->attachHostAdapter(host_adapter);
  }

  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }

  void setMock(uint16_t max_packet_size) {
    mock().expectOneCall("HostAdapter::getMaxPacketSize")
        .andReturnValue(max_packet_size);
  }
};

TEST(Command_DapInfo_PacketSize_Override, Size_Zero) {
  const uint16_t max_packet_size = 0x0000;
  setMock(max_packet_size);

  commandCmsisDap();

  checkResponse(max_packet_size);
}

TEST(Command_DapInfo_PacketSize_Override, Size_LSB_Min) {
  const uint16_t max_packet_size = 0x0001;

  setMock(max_packet_size);
  commandCmsisDap();
  checkResponse(max_packet_size);
}

TEST(Command_DapInfo_PacketSize_Override, Size_LSB_Max) {
  const uint16_t max_packet_size = 0x00ff;

  setMock(max_packet_size);
  commandCmsisDap();
  checkResponse(max_packet_size);
}

TEST(Command_DapInfo_PacketSize_Override, Size_MSB_Min) {
  const uint16_t max_packet_size = 0x0100;

  setMock(max_packet_size);
  commandCmsisDap();
  checkResponse(max_packet_size);
}

TEST(Command_DapInfo_PacketSize_Override, Size_MSB_Max) {
  const uint16_t max_packet_size = 0xff00;

  setMock(max_packet_size);
  commandCmsisDap();
  checkResponse(max_packet_size);
}

TEST(Command_DapInfo_PacketSize_Override, Size_Max) {
  const uint16_t max_packet_size = 0xffff;

  setMock(max_packet_size);
  commandCmsisDap();
  checkResponse(max_packet_size);
}

TEST(Command_DapInfo_PacketSize_Override, Size) {
  const uint16_t max_packet_size = 0xa5f0;

  setMock(max_packet_size);
  commandCmsisDap();
  checkResponse(max_packet_size);
}

}  // namespace dbger
