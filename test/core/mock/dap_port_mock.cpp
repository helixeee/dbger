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

#include "test/core/mock/dap_port_mock.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "core/cmsis_dap.h"

namespace dbger {

DapPortMock::DapPortMock() {
}

DapPortMock::~DapPortMock() {
}

bool DapPortMock::connect() {
  mock().actualCall("DapPort::connect");
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::disconnect() {
  mock().actualCall("DapPort::disconnect");
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::writeAbort(uint8_t index, uint32_t abort) {
  mock().actualCall("DapPort::writeAbort")
      .withParameter("index", index)
      .withParameter("abort", abort);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::isImplementedResetSequence() const {
  mock().actualCall("DapPort::isImplementedResetSequence");
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::reset() {
  mock().actualCall("DapPort::reset");
  return mock().returnIntValueOrDefault(1) ? true : false;
}

uint8_t DapPortMock::setPins(uint8_t pin_output,
                             uint8_t pin_select,
                             uint32_t pin_wait) {
  mock().actualCall("DapPort::setPins")
      .withParameter("pin_output", pin_output)
      .withParameter("pin_select", pin_select)
      .withParameter("pin_wait", pin_wait);
  return mock().returnIntValueOrDefault(0);
}

bool DapPortMock::setClock(uint32_t clock) {
  mock().actualCall("DapPort::setClock")
      .withParameter("clock", clock);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::sequenceSwj(int bit_count,
                              uint8_t bit_data) {
  mock().actualCall("DapPort::sequenceSwj")
      .withParameter("bit_count", bit_count)
      .withParameter("bit_data", bit_data);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::configurateSwd(uint8_t turnaround_clock,
                                 bool generate_data_phase) {
  mock().actualCall("DapPort::configurateSwd")
      .withParameter("turnaround_clock", turnaround_clock)
      .withParameter("generate_data_phase", generate_data_phase);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::sequenceJtag(int tck_cycles,
                               int tms_value,
                               uint8_t tdi_data,
                               uint8_t *tdo_data) {
  mock().actualCall("DapPort::sequenceJtag")
      .withParameter("tck_cycles", tck_cycles)
      .withParameter("tms_value", tms_value)
      .withParameter("tdi_data", tdi_data)
      .withOutputParameter("tdo_data", tdo_data);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::setNumberOfJtagDevices(int count) {
  mock().actualCall("DapPort::setNumberOfJtagDevices")
      .withParameter("count", count);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::setJtagIrLength(int index, uint8_t ir_length) {
  mock().actualCall("DapPort::setJtagIrLength")
      .withParameter("index", index)
      .withParameter("ir_length", ir_length);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::readJtagIdcode(uint8_t jtag_index, uint32_t *idcode) {
  mock().actualCall("DapPort::readJtagIdcode")
      .withParameter("jtag_index", jtag_index)
      .withOutputParameter("idcode", idcode);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMock::checkJtagIndex(uint8_t index) {
  mock().actualCall("DapPort::checkJtagIndex")
      .withParameter("index", index);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

DapPortMockSwd::~DapPortMockSwd() {
}

bool DapPortMockSwd::connect() {
  mock().actualCall("DapPortSwd::connect");
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMockSwd::disconnect() {
  mock().actualCall("DapPortSwd::disconnect");
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMockSwd::setClock(uint32_t clock) {
  mock().actualCall("DapPortSwd::setClock")
      .withParameter("clock", clock);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

int DapPortMockSwd::readImpl(uint32_t write_data, uint32_t *read_data) {
  mock().actualCall("DapPortSwd::read")
      .withParameter("write_data", write_data)
      .withOutputParameter("read_data", read_data)
      .withParameter("previous_ir_register",
                     transfer_information.previous_ir_register)
      .withParameter("current_ir_register",
                     transfer_information.current_ir_register)
      .withParameter("address", transfer_information.address);
  return mock().returnIntValueOrDefault(CmsisDap::kTransferAckOk);
}

int DapPortMockSwd::writeImpl(uint32_t write_data, uint32_t *read_data) {
  mock().actualCall("DapPortSwd::write")
      .withParameter("write_data", write_data)
      .withOutputParameter("read_data", read_data)
      .withParameter("previous_ir_register",
                     transfer_information.previous_ir_register)
      .withParameter("current_ir_register",
                     transfer_information.current_ir_register)
      .withParameter("address", transfer_information.address);
  return mock().returnIntValueOrDefault(CmsisDap::kTransferAckOk);
}

DapPortMockJtag::~DapPortMockJtag() {
}

bool DapPortMockJtag::connect() {
  mock().actualCall("DapPortJtag::connect");
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMockJtag::disconnect() {
  mock().actualCall("DapPortJtag::disconnect");
  return mock().returnIntValueOrDefault(1) ? true : false;
}

bool DapPortMockJtag::setClock(uint32_t clock) {
  mock().actualCall("DapPortJtag::setClock")
      .withParameter("clock", clock);
  return mock().returnIntValueOrDefault(1) ? true : false;
}

int DapPortMockJtag::readImpl(uint32_t write_data, uint32_t *read_data) {
  mock().actualCall("DapPortJtag::read")
      .withParameter("write_data", write_data)
      .withOutputParameter("read_data", read_data)
      .withParameter("previous_ir_register",
                     transfer_information.previous_ir_register)
      .withParameter("current_ir_register",
                     transfer_information.current_ir_register)
      .withParameter("address", transfer_information.address);
  return mock().returnIntValueOrDefault(CmsisDap::kTransferAckOk);
}

int DapPortMockJtag::writeImpl(uint32_t write_data, uint32_t *read_data) {
  mock().actualCall("DapPortJtag::write")
      .withParameter("write_data", write_data)
      .withOutputParameter("read_data", read_data)
      .withParameter("previous_ir_register",
                     transfer_information.previous_ir_register)
      .withParameter("current_ir_register",
                     transfer_information.current_ir_register)
      .withParameter("address", transfer_information.address);
  return mock().returnIntValueOrDefault(CmsisDap::kTransferAckOk);
}

}  // namespace dbger
