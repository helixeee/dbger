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

#include "test/core/mock/cmsis_dap_mock.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "test/core/mock/dap_port_mock.h"
#include "test/core/mock/indicator_mock.h"

namespace dbger {

CmsisDapMock::CmsisDapMock() : CmsisDap() {
}

CmsisDapMock::~CmsisDapMock() {
}

void CmsisDapMock::attachPort() {
  static DapPortMock swd_port;
  swd_port_ = &swd_port;

  static DapPortMock jtag_port;
  jtag_port_ = &jtag_port;
}

void CmsisDapMock::attachPortSwj() {
  attachPortSwd();
  attachPortJtag();
}

void CmsisDapMock::attachPortSwd() {
  static DapPortMockSwd swd_port;
  swd_port_ = &swd_port;
}

void CmsisDapMock::attachPortJtag() {
  static DapPortMockJtag jtag_port;
  jtag_port_ = &jtag_port;
}

void CmsisDapMock::detachPortSwj() {
  swd_port_ = nullptr;
  jtag_port_ = nullptr;
}

void CmsisDapMock::connectPortSwd() {
  if (swd_port_ != nullptr) {
    access_port_ = swd_port_;
  }
}

void CmsisDapMock::connectPortJtag() {
  if (jtag_port_ != nullptr) {
    access_port_ = jtag_port_;
  }
}

void CmsisDapMock::disconnectPort() {
  if (access_port_ != nullptr) {
    access_port_ = nullptr;
  }
}

void CmsisDapMock::attachIndicator(Indicator *indicator) {
  indicator_ = indicator;
}

bool CmsisDapMock::delay(uint16_t delay) {
  mock().actualCall("CmsisDap::delay")
      .withParameter("delay", delay);
  bool return_val = false;
  if (mock().intReturnValue() != 0) {
    return_val = true;
  }
  return return_val;
}

uint32_t CmsisDapMock::getInfoVendorId() {
  char info[64];

  mock().actualCall("CmsisDap::getInfoVendorId")
      .withOutputParameter("vendor_id", info);

  return copyInfoString(info);
}

uint32_t CmsisDapMock::getInfoProductId() {
  char info[64];

  mock().actualCall("CmsisDap::getInfoProductId")
      .withOutputParameter("product_id", info);

  return copyInfoString(info);
}

uint32_t CmsisDapMock::getInfoSerialNumber() {
  char info[64];

  mock().actualCall("CmsisDap::getInfoSerialNumber")
      .withOutputParameter("serial_number", info);

  return copyInfoString(info);
}

uint32_t CmsisDapMock::getInfoFirmwareVersion() {
  char info[0xffff];

  mock().actualCall("CmsisDap::getInfoFirmwareVersion")
      .withOutputParameter("firmware_version", info);

  return copyInfoString(info);
}

uint32_t CmsisDapMock::getInfoTargetDeviceVendor() {
  char info[64];

  mock().actualCall("CmsisDap::getInfoTargetDeviceVendor")
      .withOutputParameter("target_device_vendor", info);

  return copyInfoString(info);
}

uint32_t CmsisDapMock::getInfoTargetDeviceName() {
  char info[64];

  mock().actualCall("CmsisDap::getInfoTargetDeviceName")
      .withOutputParameter("target_device_name", info);

  return copyInfoString(info);
}

CmsisDapMockDefaultJtag::CmsisDapMockDefaultJtag() {
}

CmsisDapMockDefaultJtag::~CmsisDapMockDefaultJtag() {
}

int CmsisDapMockDefaultJtag::connectDefaultPortOnDapConnect() {
  return connectPortOnDapConnect(&jtag_port_, kDapPortModeJtag);
}

}  // end namespace dbger
