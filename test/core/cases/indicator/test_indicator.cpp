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

#include "core/indicator.h"
#include "test/core/mock/indicator_mock.h"

namespace dbger {

TEST_GROUP(Indicator) {
  Indicator indicator;
};

TEST(Indicator, switchConnect) {
  CHECK_EQUAL(true, indicator.switchConnect(true));
  CHECK_EQUAL(true, indicator.switchConnect(false));
}

TEST(Indicator, switchRunning) {
  CHECK_EQUAL(true, indicator.switchRunning(true));
  CHECK_EQUAL(true, indicator.switchRunning(false));
}

TEST_GROUP(IndicatorMock) {
  IndicatorMock indicator;

  void teardown() {
    mock().checkExpectations();
    mock().clear();
  }
};

TEST(IndicatorMock, switchConnectOn_Ok) {
  const bool on = true;

  mock().expectOneCall("Indicator::switchConnect")
      .withParameter("on", on)
      .andReturnValue(1);
  CHECK_EQUAL(true, indicator.switchConnect(on));
}

TEST(IndicatorMock, switchConnectOn_Error) {
  const bool on = true;

  mock().expectOneCall("Indicator::switchConnect")
      .withParameter("on", on)
      .andReturnValue(0);
  CHECK_EQUAL(false, indicator.switchConnect(on));
}

TEST(IndicatorMock, switchConnectOff_Ok) {
  const bool on = false;

  mock().expectOneCall("Indicator::switchConnect")
      .withParameter("on", on)
      .andReturnValue(1);
  CHECK_EQUAL(true, indicator.switchConnect(on));
}

TEST(IndicatorMock, switchConnectOff_Error) {
  const bool on = false;

  mock().expectOneCall("Indicator::switchConnect")
      .withParameter("on", on)
      .andReturnValue(0);
  CHECK_EQUAL(false, indicator.switchConnect(on));
}

TEST(IndicatorMock, switchRunningOn_Ok) {
  const bool on = true;

  mock().expectOneCall("Indicator::switchRunning")
      .withParameter("on", on)
      .andReturnValue(1);
  CHECK_EQUAL(true, indicator.switchRunning(on));
}

TEST(IndicatorMock, switchRunningOn_Error) {
  const bool on = true;

  mock().expectOneCall("Indicator::switchRunning")
      .withParameter("on", on)
      .andReturnValue(0);
  CHECK_EQUAL(false, indicator.switchRunning(on));
}

TEST(IndicatorMock, switchRunningOff_Ok) {
  const bool on = false;

  mock().expectOneCall("Indicator::switchRunning")
      .withParameter("on", on)
      .andReturnValue(1);
  CHECK_EQUAL(true, indicator.switchRunning(on));
}

TEST(IndicatorMock, switchRunningOff_Error) {
  const bool on = false;

  mock().expectOneCall("Indicator::switchRunning")
      .withParameter("on", on)
      .andReturnValue(0);
  CHECK_EQUAL(false, indicator.switchRunning(on));
}

}  // namespace dbger
