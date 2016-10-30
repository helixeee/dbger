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

#include <cstdint>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "platforms/mbed/mbed_cmsis_dap.h"

namespace dbger {

TEST_GROUP(Mbed_CMSIS_DAP_Delay) {
  uint32_t command_length;
  uint8_t command[3];

  uint32_t response_length;
  uint8_t response[2];

  dbger::arm_mbed::MbedCmsisDap cmsis_dap;

  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }
  void setCommand(uint16_t delay) {
    for (int i = 0; i < 3; i++) {
      command[i] = 0xff;
    }
    command_length = 3;
    command[0] = CmsisDap::kCommandIdDapDelay;
    command[1] = delay & 0xff;
    command[2] = (delay >> 8) & 0xff;

    for (int i = 0; i < 2; i++) {
      response[i] = 0xff;
    }
    response_length = 0;
  }
  void checkResponse(int status) {
    CHECK_EQUAL(2, response_length);
    BYTES_EQUAL(CmsisDap::kCommandIdDapDelay, response[0]);
    BYTES_EQUAL(status, response[1]);
  }
  void setMock(uint16_t delay) {
    mock().expectOneCall("wait_us")
        .withParameter("delay_usec", delay);
  }
};

TEST(Mbed_CMSIS_DAP_Delay, Default) {
  setCommand(0x0000);
  setMock(0x0000);
  cmsis_dap.command(command_length, command, &response_length, response);
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0x00ff);
  setMock(0x00ff);
  cmsis_dap.command(command_length, command, &response_length, response);
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xff00);
  setMock(0xff00);
  cmsis_dap.command(command_length, command, &response_length, response);
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xffff);
  setMock(0xffff);
  cmsis_dap.command(command_length, command, &response_length, response);
  checkResponse(CmsisDap::kResponseStatusOk);

  setCommand(0xa5f0);
  setMock(0xa5f0);
  cmsis_dap.command(command_length, command, &response_length, response);
  checkResponse(CmsisDap::kResponseStatusOk);
}

}  // namespace dbger
