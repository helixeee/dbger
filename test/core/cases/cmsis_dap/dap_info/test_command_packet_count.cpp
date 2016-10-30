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

struct DapInfo_PacketCount : public TestCmsisDap {
  DapInfo_PacketCount() {
    newCmsisDap();

    allocateCommand();
    setCommand();
  }
  void setCommand() {
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapInfo;
    command[1] = CmsisDap::kInfoIdPacketCount;
  }
  void checkResponse(uint8_t packet_count) {
    CHECK_EQUAL(3, response_length);

    BYTES_EQUAL(CmsisDap::kCommandIdDapInfo, response[0]);
    BYTES_EQUAL(1, response[1]);
    BYTES_EQUAL(packet_count, response[2]);
  }
};

TEST_GROUP_BASE(Command_DapInfo_PacketCount, DapInfo_PacketCount) {
};

TEST(Command_DapInfo_PacketCount, Default) {
  commandCmsisDap();
  checkResponse(0);
}

TEST_GROUP_BASE(Command_DapInfo_PacketCount_Override, DapInfo_PacketCount) {
  void setup() {
    newHostAdapterMock();
    cmsis_dap->attachHostAdapter(host_adapter);
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }

  void setMock(uint8_t max_packet_count) {
    mock().expectOneCall("HostAdapter::getMaxPacketCount")
        .andReturnValue(max_packet_count);
  }
};

TEST(Command_DapInfo_PacketCount_Override, Count_Zero) {
  const uint8_t max_packet_count = 0x00;

  setMock(max_packet_count);
  commandCmsisDap();
  checkResponse(max_packet_count);
}

TEST(Command_DapInfo_PacketCount_Override, Count_Min) {
  const uint8_t max_packet_count = 0x01;

  setMock(max_packet_count);
  commandCmsisDap();
  checkResponse(max_packet_count);
}

TEST(Command_DapInfo_PacketCount_Override, Count_Max) {
  const uint8_t max_packet_count = 0xff;

  setMock(max_packet_count);
  commandCmsisDap();
  checkResponse(max_packet_count);
}

}  // namespace dbger
