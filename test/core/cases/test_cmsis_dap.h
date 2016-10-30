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

#ifndef TEST_CORE_CASES_TEST_CMSIS_DAP_H_
#define TEST_CORE_CASES_TEST_CMSIS_DAP_H_

#include "CppUTest/TestHarness.h"

#include "core/cmsis_dap.h"
#include "core/host_adapter.h"
#include "core/indicator.h"

#include "test/core/mock/cmsis_dap_mock.h"
#include "test/core/mock/host_adapter_mock.h"
#include "test/core/mock/indicator_mock.h"

using dbger::CmsisDap;
using dbger::HostAdapter;
using dbger::Indicator;

using dbger::CmsisDapMock;
using dbger::CmsisDapMockDefaultJtag;
using dbger::HostAdapterMock;

TEST_BASE(TestCmsisDap) {
  enum CommandParameter {
    kDefaultMaxCommandSize = 64,
  };

  CmsisDap *cmsis_dap;
  HostAdapter *host_adapter;
  Indicator *indicator;

  int command_size;

  uint32_t command_length;
  uint8_t *command;

  uint32_t response_length;
  uint8_t *response;

  TestCmsisDap()
      : cmsis_dap(nullptr),
        host_adapter(nullptr),
        indicator(nullptr),
        command_size(0),
        command_length(0),
        command(nullptr),
        response_length(0),
        response(nullptr) {
  }
  virtual ~TestCmsisDap() {
    delete cmsis_dap;
    deallocateCommand();
    delete host_adapter;
    delete indicator;
  }

  void newCmsisDap() {
    delete cmsis_dap;
    cmsis_dap = new CmsisDap;
  }
  void newCmsisDapMock(int default_port) {
    delete cmsis_dap;
    if (default_port == CmsisDap::kDapPortModeJtag) {
      cmsis_dap = new CmsisDapMockDefaultJtag;
    } else {
      cmsis_dap = new CmsisDapMock;
    }
  }
  void newCmsisDapMock() {
    delete cmsis_dap;
    cmsis_dap = new CmsisDapMock;
  }

  void attachPort() {
    reinterpret_cast<CmsisDapMock*>(cmsis_dap)->attachPort();
  }
  void attachPortSwd() {
    reinterpret_cast<CmsisDapMock*>(cmsis_dap)->attachPortSwd();
  }
  void attachPortJtag() {
    reinterpret_cast<CmsisDapMock*>(cmsis_dap)->attachPortJtag();
  }
  void attachPortSwj() {
    attachPortSwd();
    attachPortJtag();
  }

  void connectPortSwd() {
    reinterpret_cast<CmsisDapMock*>(cmsis_dap)->connectPortSwd();
  }
  void connectPortJtag() {
    reinterpret_cast<CmsisDapMock*>(cmsis_dap)->connectPortJtag();
  }

  void newHostAdapterMock() {
    host_adapter = new HostAdapterMock;
  }

  void allocateCommand(int size = kDefaultMaxCommandSize) {
    command = new uint8_t[size];
    response = new uint8_t[size];
    command_size = size;
  }
  void deallocateCommand() {
    delete[] command;
    delete[] response;
    command_size = 0;
  }

  void initCommand() {
    command_length = 0;
    response_length = 0;
    for (int i = 0; i < command_size; i++) {
      command[i] = 0xff;
      response[i] = 0xff;
    }
  }
  int commandCmsisDap() {
    return cmsis_dap->command(command_length, command,
                              &response_length, response);
  }
};

#endif  // TEST_CORE_CASES_TEST_CMSIS_DAP_H_
