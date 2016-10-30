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

#ifndef TEST_CORE_CASES_CMSIS_DAP_DAP_TRANSFER_BLOCK_TEST_TRANSFER_BLOCK_H_
#define TEST_CORE_CASES_CMSIS_DAP_DAP_TRANSFER_BLOCK_TEST_TRANSFER_BLOCK_H_

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "core/dap_port.h"
#include "test/core/cases/cmsis_dap/test_transfer_request.h"

namespace dbger {

struct DapTransferBlock : public TestCmsisDap {
  virtual ~DapTransferBlock() {}
  void setCommand(int index) {
    initCommand();
    command_length = 4;
    command[0] = CmsisDap::kCommandIdDapTransferBlock;
    command[1] = index;
    command[2] = 0;
    command[3] = 0;
  }
  void setCommand(int index, int count,
                  TransferRequest *transfer_request,
                  const uint32_t *transfer_data) {
    initCommand();
    command[0] = CmsisDap::kCommandIdDapTransferBlock;
    command[1] = index;
    command[2] = count & 0xff;
    command[3] = (count >> 8) & 0xff;
    command[4] = transfer_request->getTransferRequest();
    int command_index = 5;
    if (transfer_request->transfer_request_type ==
        TransferRequest::kTransferTypeNormalWrite) {
      for (int i = 0; i < count; i++) {
        command[command_index++] = transfer_data[i] & 0xff;
        command[command_index++] = (transfer_data[i] >> 8) & 0xff;
        command[command_index++] = (transfer_data[i] >> 16) & 0xff;
        command[command_index++] = (transfer_data[i] >> 24) & 0xff;
      }
    }
    command_length = command_index;
  }
  void checkResponse(int transfer_response) {
    CHECK_EQUAL(4, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapTransferBlock, response[0]);
    BYTES_EQUAL(0, response[1]);
    BYTES_EQUAL(0, response[2]);
    BYTES_EQUAL(transfer_response, response[3]);
  }
  void checkResponse(int count, int transfer_response,
                     const TransferRequest *transfer_request,
                     const uint32_t *response_data) {
    BYTES_EQUAL(CmsisDap::kCommandIdDapTransferBlock, response[0]);
    BYTES_EQUAL(count & 0xff, response[1]);
    BYTES_EQUAL((count >> 8) & 0xff, response[2]);
    BYTES_EQUAL(transfer_response, response[3]);
    int index = 4;
    if (transfer_request->transfer_request_type ==
        TransferRequest::kTransferTypeNormalRead) {
      for (int i = 0; i < count; i++) {
        BYTES_EQUAL(response_data[i] & 0xff, response[index++]);
        BYTES_EQUAL((response_data[i] >> 8) & 0xff, response[index++]);
        BYTES_EQUAL((response_data[i] >> 16) & 0xff, response[index++]);
        BYTES_EQUAL((response_data[i] >> 24) & 0xff, response[index++]);
      }
    }
    CHECK_EQUAL(index, response_length);
  }
  TransferRequest transfer_request;
};

struct DapTransferBlockMockSwd : public DapTransferBlock {
  virtual ~DapTransferBlockMockSwd() {
    mock().checkExpectations();
    mock().clear();
    delete[] transfer_data;
    delete[] response_data;
  }
  void setMockDummyReadApacc(int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortSwd::read")
        .withOutputParameterReturning("read_data", nullptr, 0)
        .withParameter("current_ir_register", DapPort::kIrRegisterApacc)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadRdbuff(uint32_t *response_data,
                         int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortSwd::read")
        .withOutputParameterReturning("read_data",
                                      response_data, sizeof(uint32_t))
        .withParameter("current_ir_register", DapPort::kIrRegisterDpacc)
        .withParameter("address", DapPort::kDpRegisterRdbuff)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadApacc(uint32_t *response_data,
                        int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortSwd::read")
        .withOutputParameterReturning("read_data",
                                      response_data, sizeof(uint32_t))
        .withParameter("current_ir_register", DapPort::kIrRegisterApacc)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadDpacc(uint32_t *response_data,
                        int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortSwd::read")
        .withOutputParameterReturning("read_data",
                                      response_data, sizeof(uint32_t))
        .withParameter("current_ir_register", DapPort::kIrRegisterDpacc)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockWrite(uint32_t transfer_data,
                    int ack = CmsisDap::kTransferAckOk) {
    int ir_register = transfer_request.APnDP ?
        DapPort::kIrRegisterApacc : DapPort::kIrRegisterDpacc;
    mock().expectOneCall("DapPortSwd::write")
        .withParameter("write_data", transfer_data)
        .withOutputParameterReturning("read_data", nullptr, 0)
        .withParameter("current_ir_register", ir_register)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  uint32_t *transfer_data;
  uint32_t *response_data;
};

struct DapTransferBlockMockJtag : public DapTransferBlock {
  virtual ~DapTransferBlockMockJtag() {
    mock().checkExpectations();
    mock().clear();
    delete[] transfer_data;
    delete[] response_data;
  }
  void setMockDummyReadApacc(int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortJtag::read")
        .withOutputParameterReturning("read_data", nullptr, 0)
        .withParameter("current_ir_register", DapPort::kIrRegisterApacc)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockDummyReadDpacc(int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortJtag::read")
        .withOutputParameterReturning("read_data", nullptr, 0)
        .withParameter("current_ir_register", DapPort::kIrRegisterDpacc)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadRdbuff(uint32_t *response_data,
                         int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortJtag::read")
        .withOutputParameterReturning("read_data",
                                      response_data, sizeof(uint32_t))
        .withParameter("current_ir_register", DapPort::kIrRegisterDpacc)
        .withParameter("address", DapPort::kDpRegisterRdbuff)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadApacc(uint32_t *response_data,
                        int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortJtag::read")
        .withOutputParameterReturning("read_data",
                                      response_data, sizeof(uint32_t))
        .withParameter("current_ir_register", DapPort::kIrRegisterApacc)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockReadDpacc(uint32_t *response_data,
                        int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortJtag::read")
        .withOutputParameterReturning("read_data",
                                      response_data, sizeof(uint32_t))
        .withParameter("current_ir_register", DapPort::kIrRegisterDpacc)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockWrite(uint32_t transfer_data,
                    int ack = CmsisDap::kTransferAckOk) {
    int ir_register = transfer_request.APnDP ?
        DapPort::kIrRegisterApacc : DapPort::kIrRegisterDpacc;
    mock().expectOneCall("DapPortJtag::write")
        .withParameter("write_data", transfer_data)
        .withOutputParameterReturning("read_data", nullptr, 0)
        .withParameter("current_ir_register", ir_register)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockWriteApacc(uint32_t transfer_data,
                         int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortJtag::write")
        .withParameter("write_data", transfer_data)
        .withOutputParameterReturning("read_data", nullptr, 0)
        .withParameter("current_ir_register", DapPort::kIrRegisterApacc)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  void setMockWriteDpacc(uint32_t transfer_data,
                         int ack = CmsisDap::kTransferAckOk) {
    mock().expectOneCall("DapPortJtag::write")
        .withParameter("write_data", transfer_data)
        .withOutputParameterReturning("read_data", nullptr, 0)
        .withParameter("current_ir_register", DapPort::kIrRegisterDpacc)
        .withParameter("address", transfer_request.address)
        .andReturnValue(ack)
        .ignoreOtherParameters();
  }
  uint32_t *transfer_data;
  uint32_t *response_data;
};

}  // namespace dbger

#endif  // TEST_CORE_CASES_CMSIS_DAP_DAP_TRANSFER_BLOCK_TEST_TRANSFER_BLOCK_H_
