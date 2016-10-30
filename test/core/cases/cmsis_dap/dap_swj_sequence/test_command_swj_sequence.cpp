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

struct DapSwjSequence : public TestCmsisDap {
  DapSwjSequence() {
    allocateCommand(34);
  }
  void setCommand(int bit_count, const uint8_t *sequence) {
    if (bit_count > 256 || bit_count <= 0) {
      FAIL("Illegal bit_count");
    }

    initCommand();
    command_length = 2 + (bit_count + 7) / 8;
    command[0] = CmsisDap::kCommandIdDapSwjSequence;
    command[1] = bit_count & 0xff;
    for (int i = 0; i < (bit_count + 7) / 8; i++) {
      command[2 + i] = sequence[i];
    }
  }
  void checkResponse(int status) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapSwjSequence, response[0]);
    BYTES_EQUAL(status, response[1]);
  }

  uint8_t sequence[256 / 8];
};

TEST_GROUP_BASE(Command_DapSwjSequence_PortIsNotConnected, DapSwjSequence) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapSwjSequence_PortIsNotConnected, Count_Min) {
  sequence[0] = 0;
  setCommand(1, sequence);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  sequence[0] = 0xff;
  setCommand(1, sequence);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST(Command_DapSwjSequence_PortIsNotConnected, Count_Max) {
  for (int i = 0; i < 256 / 8; i++) {
    sequence[i] = 0x00;
  }
  setCommand(255, sequence);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  for (int i = 0; i < 256 / 8; i++) {
    sequence[i] = 0xff;
  }
  setCommand(255, sequence);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  for (int i = 0; i < 256 / 8; i++) {
    sequence[i] = i;
  }
  setCommand(255, sequence);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST(Command_DapSwjSequence_PortIsNotConnected, Count_EncodedAsZero) {
  for (int i = 0; i < 256 / 8; i++) {
    sequence[i] = 0x00;
  }
  setCommand(256, sequence);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  for (int i = 0; i < 256 / 8; i++) {
    sequence[i] = 0xff;
  }
  setCommand(256, sequence);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  for (int i = 0; i < 256 / 8; i++) {
    sequence[i] = i;
  }
  setCommand(256, sequence);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST_GROUP_BASE(Command_DapSwjSequence_PortIsConnected, DapSwjSequence) {
  void setup() {
    newCmsisDapMock();
    attachPort();
    connectPortSwd();
    initResult();
  }
  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }

  void setMock(int bit_count) {
    int i = 0;
    for ( ; i < bit_count / 8; i++) {
      mock().expectOneCall("DapPort::sequenceSwj")
          .withParameter("bit_count", 8)
          .withParameter("bit_data", sequence[i])
          .andReturnValue(result[i] ? 1 : 0);
    }
    if (bit_count % 8 > 0) {
      mock().expectOneCall("DapPort::sequenceSwj")
          .withParameter("bit_count", bit_count % 8)
          .withParameter("bit_data", sequence[i])
          .andReturnValue(result[i] ? 1 : 0);
    }
  }
  bool result[256 / 8];
  void initResult() {
    for (int i = 0; i < 256 / 8; i++) {
      result[i] = true;
    }
  }
};

TEST(Command_DapSwjSequence_PortIsConnected, Ok) {
  for (int i = 0; i < 256 / 8; i++) {
    sequence[i] = i;
  }

  setCommand(1, sequence);
  setMock(1);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(255, sequence);
  setMock(255);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(256, sequence);
  setMock(256);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapSwjSequence_PortIsConnected, ErrorAtLast) {
  for (int i = 0; i < 256 / 8; i++) {
    sequence[i] = i;
  }
  int bit_count;

  bit_count = 1;
  initResult();
  result[(bit_count - 1) / 8] = false;
  setCommand(bit_count, sequence);
  setMock(bit_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  bit_count = 255;
  initResult();
  result[(bit_count - 1) / 8] = false;
  setCommand(bit_count, sequence);
  setMock(bit_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  bit_count = 256;
  initResult();
  result[(bit_count - 1) / 8] = false;
  setCommand(bit_count, sequence);
  setMock(bit_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST(Command_DapSwjSequence_PortIsConnected, ErrorAtFirst) {
  for (int i = 0; i < 256 / 8; i++) {
    sequence[i] = i;
  }

  result[0] = false;

  setCommand(1, sequence);
  setMock(1);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(255, sequence);
  setMock(255);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);

  setCommand(256, sequence);
  setMock(256);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

}  // namespace dbger
