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

#include "test/platforms/mbed/mock/DigitalInOut.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

namespace mbed {

void DigitalInOut::write(int data) {
  mock().actualCall("DigitalInOut::write")
      .withParameter("data", data);
}

int DigitalInOut::read() {
  mock().actualCall("DigitalInOut::read");
  return mock().returnIntValueOrDefault(0);
}

void DigitalInOut::output() {
  mock().actualCall("DigitalInOut::output");
}

void DigitalInOut::input() {
  mock().actualCall("DigitalInOut::input");
}

}  // namespace mbed
