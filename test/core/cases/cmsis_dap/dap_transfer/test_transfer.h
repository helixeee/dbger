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

#ifndef TEST_CORE_CASES_CMSIS_DAP_DAP_TRANSFER_TEST_TRANSFER_H_
#define TEST_CORE_CASES_CMSIS_DAP_DAP_TRANSFER_TEST_TRANSFER_H_

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "core/dap_port.h"
#include "test/core/cases/cmsis_dap/test_transfer_request.h"

namespace dbger {

struct Transfer : public TransferRequest {
  uint32_t transfer_data;
  uint32_t response_data;
  bool hasTransferData() {
    switch (transfer_request_type) {
      case kTransferTypeNormalWrite:
      case kTransferTypeValueMatch:
      case kTransferTypeMatchMask:
        return true;
      default:
        return false;
    }
  }
  bool hasResponseData() {
    switch (transfer_request_type) {
      case kTransferTypeNormalRead:
      case kTransferTypeValueMatch:
        return true;
      default:
        return false;
    }
  }
};

struct DapTransfer : public TestCmsisDap {
  void setCommand(int index) {
    initCommand();
    command_length = 3;
    command[0] = CmsisDap::kCommandIdDapTransfer;
    command[1] = index;
    command[2] = 0;
  }
  void setCommand(int index, int count, Transfer *transfer) {
    initCommand();
    command[0] = CmsisDap::kCommandIdDapTransfer;
    command[1] = index;
    command[2] = count;
    int command_index = 3;
    for (int i = 0; i < count; i++) {
      command[command_index++] = transfer[i].getTransferRequest();
      if (transfer[i].hasTransferData()) {
        command[command_index++] = transfer[i].transfer_data & 0xff;
        command[command_index++] = (transfer[i].transfer_data >> 8) & 0xff;
        command[command_index++] = (transfer[i].transfer_data >> 16) & 0xff;
        command[command_index++] = (transfer[i].transfer_data >> 24) & 0xff;
      }
    }
    command_length = command_index;
  }
  void checkResponse(int transfer_response) {
    CHECK_EQUAL(3, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapTransfer, response[0]);
    BYTES_EQUAL(0, response[1]);
    BYTES_EQUAL(transfer_response, response[2]);
  }
  void checkResponse(int count, int transfer_response, Transfer *transfer) {
    BYTES_EQUAL(CmsisDap::kCommandIdDapTransfer, response[0]);
    BYTES_EQUAL(count, response[1]);
    BYTES_EQUAL(transfer_response, response[2]);
    int index = 3;
    for (int i = 0; i < count; i++) {
      if (transfer[i].hasResponseData()) {
        BYTES_EQUAL(transfer[i].response_data & 0xff, response[index++]);
        BYTES_EQUAL((transfer[i].response_data >> 8) & 0xff, response[index++]);
        BYTES_EQUAL((transfer[i].response_data >> 16) & 0xff,
                    response[index++]);
        BYTES_EQUAL((transfer[i].response_data >> 24) & 0xff,
                    response[index++]);
      }
    }
    CHECK_EQUAL(index, response_length);
  }
};

struct DapTransferMockSwd {
  ~DapTransferMockSwd() {
    mock().checkExpectations();
    mock().clear();
    delete[] transfer;
  }
  void setMockDummyReadApacc(int transfer_index,
                             int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortSwd::read")
        .withOutputParameterReturning("read_data", nullptr, 0)
        .withParameter("current_ir_register", DapPort::kIrRegisterApacc)
        .withParameter("address", transfer[transfer_index].address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadRdbuff(int transfer_index,
                         int ack = CmsisDap::kTransferAckOk) {
    uint32_t *read_data = &transfer[transfer_index].response_data;
    mock().expectOneCall("DapPortSwd::read")
        .withOutputParameterReturning("read_data",
                                      read_data, sizeof(&read_data))
        .withParameter("current_ir_register", DapPort::kIrRegisterDpacc)
        .withParameter("address", DapPort::kDpRegisterRdbuff)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadApacc(int transfer_index,
                        int ack = CmsisDap::kTransferAckOk) {
    uint32_t *read_data = &transfer[transfer_index - 1].response_data;
    mock().expectOneCall("DapPortSwd::read")
        .withOutputParameterReturning("read_data",
                                      read_data, sizeof(*read_data))
        .withParameter("current_ir_register", DapPort::kIrRegisterApacc)
        .withParameter("address", transfer[transfer_index].address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadApacc(int transfer_index, int read_index, int ack) {
    uint32_t *read_data = &transfer[read_index].response_data;
    mock().expectOneCall("DapPortSwd::read")
        .withOutputParameterReturning("read_data",
                                      read_data, sizeof(*read_data))
        .withParameter("current_ir_register", DapPort::kIrRegisterApacc)
        .withParameter("address", transfer[transfer_index].address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadDpacc(int transfer_index,
                        int ack = CmsisDap::kTransferAckOk) {
    uint32_t *read_data = &transfer[transfer_index].response_data;
    mock().expectOneCall("DapPortSwd::read")
        .withOutputParameterReturning("read_data",
                                      read_data, sizeof(*read_data))
        .withParameter("current_ir_register", DapPort::kIrRegisterDpacc)
        .withParameter("address", transfer[transfer_index].address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockWrite(int transfer_index, int ack = CmsisDap::kTransferAckOk) {
    uint32_t *read_data = &transfer[transfer_index].response_data;
    int ir_register =
        transfer[transfer_index].APnDP ?
        DapPort::kIrRegisterApacc : DapPort::kIrRegisterDpacc;
    mock().expectOneCall("DapPortSwd::write")
        .withParameter("write_data", transfer[transfer_index].transfer_data)
        .withOutputParameterReturning("read_data",
                                      read_data, sizeof(*read_data))
        .withParameter("current_ir_register", ir_register)
        .withParameter("address", transfer[transfer_index].address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  Transfer *transfer;
};

struct DapTransferMockJtag {
  ~DapTransferMockJtag() {
    mock().checkExpectations();
    mock().clear();
    delete[] transfer;
  }
  void setMockDummyRead(int transfer_index,
                        int ack = CmsisDap::kTransferAckOk) {
    int ir_register =
        transfer[transfer_index].APnDP ?
        DapPort::kIrRegisterApacc : DapPort::kIrRegisterDpacc;
    mock().expectOneCall("DapPortJtag::read")
        .withOutputParameterReturning("read_data", nullptr, 0)
        .withParameter("current_ir_register", ir_register)
        .withParameter("address", transfer[transfer_index].address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadRdbuff(int transfer_index,
                         int ack = CmsisDap::kTransferAckOk) {
    uint32_t *read_data = &transfer[transfer_index].response_data;
    mock().expectOneCall("DapPortJtag::read")
        .withOutputParameterReturning("read_data",
                                      read_data, sizeof(*read_data))
        .withParameter("current_ir_register", DapPort::kIrRegisterDpacc)
        .withParameter("address", DapPort::kDpRegisterRdbuff)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockRead(int transfer_index, int ack = CmsisDap::kTransferAckOk) {
    uint32_t *read_data = &transfer[transfer_index - 1].response_data;
    int ir_register =
        transfer[transfer_index].APnDP ?
        DapPort::kIrRegisterApacc : DapPort::kIrRegisterDpacc;
    mock().expectOneCall("DapPortJtag::read")
        .withOutputParameterReturning("read_data",
                                      read_data, sizeof(*read_data))
        .withParameter("current_ir_register", ir_register)
        .withParameter("address", transfer[transfer_index].address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockRead(int transfer_index, int read_index, int ack) {
    uint32_t *read_data = &transfer[read_index].response_data;
    int ir_register =
        transfer[transfer_index].APnDP ?
        DapPort::kIrRegisterApacc : DapPort::kIrRegisterDpacc;
    mock().expectOneCall("DapPortJtag::read")
        .withOutputParameterReturning("read_data",
                                      read_data, sizeof(*read_data))
        .withParameter("current_ir_register", ir_register)
        .withParameter("address", transfer[transfer_index].address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockWrite(int transfer_index, int ack = CmsisDap::kTransferAckOk) {
    uint32_t *read_data = &transfer[transfer_index].response_data;
    int ir_register =
        transfer[transfer_index].APnDP ?
        DapPort::kIrRegisterApacc : DapPort::kIrRegisterDpacc;
    mock().expectOneCall("DapPortJtag::write")
        .withParameter("write_data", transfer[transfer_index].transfer_data)
        .withOutputParameterReturning("read_data",
                                      read_data, sizeof(*read_data))
        .withParameter("current_ir_register", ir_register)
        .withParameter("address", transfer[transfer_index].address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  Transfer *transfer;
};

}  // namespace dbger

#endif  // TEST_CORE_CASES_CMSIS_DAP_DAP_TRANSFER_TEST_TRANSFER_H_
