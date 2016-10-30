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

#ifndef TEST_CORE_MOCK_CMSIS_DAP_MOCK_H_
#define TEST_CORE_MOCK_CMSIS_DAP_MOCK_H_

#include "core/cmsis_dap.h"

namespace dbger {

class Indicator;

class CmsisDapMock : public CmsisDap {
 public:
  CmsisDapMock();
  virtual ~CmsisDapMock();

  void attachPort();
  void attachPortSwj();
  void attachPortSwd();
  void attachPortJtag();
  void detachPortSwj();

  void connectPortSwd();
  void connectPortJtag();
  void disconnectPort();

  void attachIndicator(Indicator *indicator);

 protected:
  virtual bool delay(uint16_t delay);

  virtual uint32_t getInfoVendorId();
  virtual uint32_t getInfoProductId();
  virtual uint32_t getInfoSerialNumber();
  virtual uint32_t getInfoFirmwareVersion();
  virtual uint32_t getInfoTargetDeviceVendor();
  virtual uint32_t getInfoTargetDeviceName();
};

class CmsisDapMockDefaultJtag : public CmsisDapMock {
 public:
  CmsisDapMockDefaultJtag();
  virtual ~CmsisDapMockDefaultJtag();

 protected:
  virtual int connectDefaultPortOnDapConnect();
};

}  // namespace dbger

#endif  // TEST_CORE_MOCK_CMSIS_DAP_MOCK_H_
