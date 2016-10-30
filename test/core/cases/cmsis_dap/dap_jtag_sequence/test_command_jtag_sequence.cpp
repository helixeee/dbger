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

struct JtagSequence {
  int tck_cycles;
  int tms_value;
  bool tdo_capture;
  uint8_t tdi_data[8];
  uint8_t tdo_data[8];
  uint8_t getSequenceInfo() {
    if (tck_cycles > 64 || tck_cycles <= 0) {
      return 0;
    }
    uint8_t sequence_info = 0;
    sequence_info |= tck_cycles & 0x3f;
    sequence_info |= (tms_value != 0) ? (0x1 << 6) : 0;
    sequence_info |= tdo_capture ? (0x1 << 7) : 0;
    return sequence_info;
  }
};

struct DapJtagSequence : public TestCmsisDap {
  void setCommand() {
    initCommand();
    command_length = 2;
    command[0] = CmsisDap::kCommandIdDapJtagSequence;
    command[1] = 0;
  }
  void setCommand(int sequence_count, JtagSequence *jtag_sequence) {
    if (sequence_count > 0xff || sequence_count < 0) {
      FAIL("Error: sequence_count");
    }
    initCommand();
    command[0] = CmsisDap::kCommandIdDapJtagSequence;
    command[1] = sequence_count & 0xff;
    int index = 2;
    for (int i = 0; i < sequence_count; i++) {
      command[index++] = jtag_sequence[i].getSequenceInfo();
      for (int j = 0; j < (jtag_sequence[i].tck_cycles + 7) / 8; j++) {
        command[index++] = jtag_sequence[i].tdi_data[j];
      }
    }
    command_length = index;
  }
  void checkResponse(int status) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapJtagSequence, response[0]);
    BYTES_EQUAL(status, response[1]);
  }
  void checkResponse(int status, int sequence_count,
                     JtagSequence *jtag_sequence) {
    BYTES_EQUAL(CmsisDap::kCommandIdDapJtagSequence, response[0]);
    BYTES_EQUAL(status, response[1]);
    int index = 2;
    for (int i = 0; i < sequence_count; i++) {
      if (jtag_sequence[i].tdo_capture) {
        for (int j = 0; j < (jtag_sequence[i].tck_cycles + 7) / 8; j++) {
          BYTES_EQUAL(jtag_sequence[i].tdo_data[j], response[index++]);
        }
      }
    }
    CHECK_EQUAL(index, response_length);
  }
};

TEST_GROUP_BASE(Command_DapJtagSequence_PortIsNotConnected, DapJtagSequence) {
  void setup() {
    newCmsisDap();
  }
};

TEST(Command_DapJtagSequence_PortIsNotConnected, SequenceCount_Zero) {
  allocateCommand();

  setCommand();
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST(Command_DapJtagSequence_PortIsNotConnected, SequenceCount_One) {
  allocateCommand();

  JtagSequence jtag_sequence[1];
  jtag_sequence[0].tck_cycles = 1;
  jtag_sequence[0].tms_value = 0;
  jtag_sequence[0].tdo_capture = true;
  for (int i = 0; i < 8; i++) {
    jtag_sequence[0].tdi_data[i] = i;
    jtag_sequence[0].tdo_data[i] = i * i;
  }

  setCommand(1, jtag_sequence);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST(Command_DapJtagSequence_PortIsNotConnected, SequenceCount_Max) {
  allocateCommand(2297);

  JtagSequence jtag_sequence[255];
  for (int i = 0; i < 255; i++) {
    jtag_sequence[i].tck_cycles = 64;
    jtag_sequence[i].tms_value = 0;
    jtag_sequence[i].tdo_capture = true;
    for (int j = 0; j < 8; j++) {
      jtag_sequence[i].tdi_data[j] = i * j;
      jtag_sequence[i].tdo_data[j] = i + j;
    }
  }

  setCommand(255, jtag_sequence);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError);
}

TEST_GROUP_BASE(Command_DapJtagSequence_PortIsConnected, DapJtagSequence) {
  JtagSequence *jtag_sequence;
  bool (*result)[8];

  void setup() {
    newCmsisDapMock();
    attachPort();
    connectPortJtag();
  }
  void teardown() {
    delete[] jtag_sequence;
    delete[] result;
    mock().checkExpectations();
    mock().clear();
  }
  void setMock(int sequence_count) {
    for (int i = 0; i < sequence_count; i++) {
      int tck_cycles = jtag_sequence[i].tck_cycles;
      int tms_value = jtag_sequence[i].tms_value;
      int tdo_capture = jtag_sequence[i].tdo_capture ? 1 : 0;
      for (int j = 0; tck_cycles > 0; tck_cycles -= 8, j++) {
        mock().expectOneCall("DapPort::sequenceJtag")
            .withParameter("tck_cycles", tck_cycles)
            .withParameter("tms_value", tms_value)
            .withParameter("tdi_data", jtag_sequence[i].tdi_data[j])
            .withOutputParameterReturning("tdo_data",
                                          &jtag_sequence[i].tdo_data[j],
                                          tdo_capture ?
                                          sizeof(jtag_sequence[i].tdo_data[j]) :
                                          0)
            .andReturnValue(result[i][j] ? 1 : 0);
      }
    }
  }

  void allocateSequence(int size) {
    jtag_sequence = new JtagSequence[size];
    result = new bool[size][8];
  }
};

TEST(Command_DapJtagSequence_PortIsConnected, Sequence_0_Ok) {
  allocateCommand();

  setCommand();
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk);
}

TEST(Command_DapJtagSequence_PortIsConnected, Sequence_1_TckCycles_1_Ok) {
  const int sequence_count = 1;
  allocateSequence(sequence_count);
  allocateCommand(2 + sequence_count * (1 + 8));

  for (int i = 0; i < sequence_count; i++) {
    jtag_sequence[i].tck_cycles = 1;
    jtag_sequence[i].tms_value = 0;
    jtag_sequence[i].tdo_capture = false;
    for (int j = 0; j < 8; j++) {
      jtag_sequence[i].tdi_data[j]= (i + 1) * (j + 2);
      jtag_sequence[i].tdo_data[j]= (i + 3) * (j + 4);
      result[i][j] = true;
    }
  }

  jtag_sequence[sequence_count - 1].tck_cycles = 1;

  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 1;
  jtag_sequence[sequence_count - 1].tdo_capture = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 1;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);
}

TEST(Command_DapJtagSequence_PortIsConnected, Sequence_1_TckCycles_8_Ok) {
  const int sequence_count = 1;
  allocateSequence(sequence_count);
  allocateCommand(2 + sequence_count * (1 + 8));

  for (int i = 0; i < sequence_count; i++) {
    jtag_sequence[i].tck_cycles = 1;
    jtag_sequence[i].tms_value = 0;
    jtag_sequence[i].tdo_capture = false;
    for (int j = 0; j < 8; j++) {
      jtag_sequence[i].tdi_data[j]= (i + 1) * (j + 2);
      jtag_sequence[i].tdo_data[j]= (i + 3) * (j + 4);
      result[i][j] = true;
    }
  }

  jtag_sequence[sequence_count - 1].tck_cycles = 8;

  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 1;
  jtag_sequence[sequence_count - 1].tdo_capture = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 1;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);
}

TEST(Command_DapJtagSequence_PortIsConnected, Sequence_1_TckCycles_9_Ok) {
  const int sequence_count = 1;
  allocateSequence(sequence_count);
  allocateCommand(2 + sequence_count * (1 + 8));

  for (int i = 0; i < sequence_count; i++) {
    jtag_sequence[i].tck_cycles = 1;
    jtag_sequence[i].tms_value = 0;
    jtag_sequence[i].tdo_capture = false;
    for (int j = 0; j < 8; j++) {
      jtag_sequence[i].tdi_data[j]= (i + 1) * (j + 2);
      jtag_sequence[i].tdo_data[j]= (i + 3) * (j + 4);
      result[i][j] = true;
    }
  }

  jtag_sequence[sequence_count - 1].tck_cycles = 9;

  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 1;
  jtag_sequence[sequence_count - 1].tdo_capture = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 1;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);
}

TEST(Command_DapJtagSequence_PortIsConnected, Sequence_1_TckCycles_63_Ok) {
  const int sequence_count = 1;
  allocateSequence(sequence_count);
  allocateCommand(2 + sequence_count * (1 + 8));

  for (int i = 0; i < sequence_count; i++) {
    jtag_sequence[i].tck_cycles = 1;
    jtag_sequence[i].tms_value = 0;
    jtag_sequence[i].tdo_capture = false;
    for (int j = 0; j < 8; j++) {
      jtag_sequence[i].tdi_data[j]= (i + 1) * (j + 2);
      jtag_sequence[i].tdo_data[j]= (i + 3) * (j + 4);
      result[i][j] = true;
    }
  }

  jtag_sequence[sequence_count - 1].tck_cycles = 63;

  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 1;
  jtag_sequence[sequence_count - 1].tdo_capture = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 1;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);
}

TEST(Command_DapJtagSequence_PortIsConnected, Sequence_1_TckCycles_64_Ok) {
  const int sequence_count = 1;
  allocateSequence(sequence_count);
  allocateCommand(2 + sequence_count * (1 + 8));

  for (int i = 0; i < sequence_count; i++) {
    jtag_sequence[i].tck_cycles = 1;
    jtag_sequence[i].tms_value = 0;
    jtag_sequence[i].tdo_capture = false;
    for (int j = 0; j < 8; j++) {
      jtag_sequence[i].tdi_data[j]= (i + 1) * (j + 2);
      jtag_sequence[i].tdo_data[j]= (i + 3) * (j + 4);
      result[i][j] = true;
    }
  }

  jtag_sequence[sequence_count - 1].tck_cycles = 64;

  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 1;
  jtag_sequence[sequence_count - 1].tdo_capture = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);

  jtag_sequence[sequence_count - 1].tms_value = 1;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);
}

TEST(Command_DapJtagSequence_PortIsConnected, Sequence_2_Ok) {
  const int sequence_count = 2;
  allocateSequence(sequence_count);
  allocateCommand(2 + sequence_count * (1 + 8));

  for (int i = 0; i < sequence_count; i++) {
    jtag_sequence[i].tck_cycles = 1;
    jtag_sequence[i].tms_value = 0;
    jtag_sequence[i].tdo_capture = false;
    for (int j = 0; j < 8; j++) {
      jtag_sequence[i].tdi_data[j]= (i + 5) * (j + 6);
      jtag_sequence[i].tdo_data[j]= (i + 7) * (j + 8);
      result[i][j] = true;
    }
  }

  jtag_sequence[sequence_count - 1].tck_cycles = 1;
  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);
}

TEST(Command_DapJtagSequence_PortIsConnected, Sequence_255_Ok) {
  const int sequence_count = 255;
  allocateSequence(sequence_count);
  allocateCommand(2 + sequence_count * (1 + 8));

  for (int i = 0; i < sequence_count; i++) {
    jtag_sequence[i].tck_cycles = 1;
    jtag_sequence[i].tms_value = 0;
    jtag_sequence[i].tdo_capture = false;
    for (int j = 0; j < 8; j++) {
      jtag_sequence[i].tdi_data[j]= (i + 9) * (j + 10);
      jtag_sequence[i].tdo_data[j]= (i + 11) * (j + 12);
      result[i][j] = true;
    }
  }

  jtag_sequence[sequence_count - 1].tck_cycles = 1;
  jtag_sequence[sequence_count - 1].tms_value = 0;
  jtag_sequence[sequence_count - 1].tdo_capture = true;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusOk, sequence_count, jtag_sequence);
}

TEST(Command_DapJtagSequence_PortIsConnected, Sequence_FirstError) {
  const int sequence_count = 255;
  allocateSequence(sequence_count);
  allocateCommand(2 + sequence_count * (1 + 8));

  for (int i = 0; i < sequence_count; i++) {
    jtag_sequence[i].tck_cycles = 1;
    jtag_sequence[i].tms_value = 0;
    jtag_sequence[i].tdo_capture = false;
    for (int j = 0; j < 8; j++) {
      jtag_sequence[i].tdi_data[j]= (i + 9) * (j + 10);
      jtag_sequence[i].tdo_data[j]= (i + 11) * (j + 12);
      result[i][j] = true;
    }
  }

  result[0][0] = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError, sequence_count, jtag_sequence);
}

TEST(Command_DapJtagSequence_PortIsConnected, Sequence_LastError) {
  const int sequence_count = 255;
  allocateSequence(sequence_count);
  allocateCommand(2 + sequence_count * (1 + 8));

  for (int i = 0; i < sequence_count; i++) {
    jtag_sequence[i].tck_cycles = 1;
    jtag_sequence[i].tms_value = 0;
    jtag_sequence[i].tdo_capture = false;
    for (int j = 0; j < 8; j++) {
      jtag_sequence[i].tdi_data[j]= (i + 9) * (j + 10);
      jtag_sequence[i].tdo_data[j]= (i + 11) * (j + 12);
      result[i][j] = true;
    }
  }

  jtag_sequence[sequence_count - 1].tck_cycles = 64;
  result[254][7] = false;
  setCommand(sequence_count, jtag_sequence);
  setMock(sequence_count);
  commandCmsisDap();
  checkResponse(CmsisDap::kResponseStatusError, sequence_count, jtag_sequence);
}

}  // namespace dbger
