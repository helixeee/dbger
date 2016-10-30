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

#include "platforms/mbed/mbed_gpio_indicator.h"

#include "DigitalOut.h"
#include "wait_api.h"

#include "config/cxx.h"

namespace {

using mbed::DigitalOut;

bool switchGpio(DigitalOut **gpio, bool on) {
  (*gpio)->write(on ? 1 : 0);
  return true;
}

}  // namespace

namespace dbger {

namespace arm_mbed {

using mbed::DigitalOut;

MbedGpioIndicator::MbedGpioIndicator(DigitalOut *connect, DigitalOut *running)
    : connect_(connect), running_(running) {
  MBED_ASSERT(connect != nullptr);
  MBED_ASSERT(running != nullptr);
}

MbedGpioIndicator::~MbedGpioIndicator() {
}

bool MbedGpioIndicator::switchConnect(bool on) {
  return switchGpio(&connect_, on);
}

bool MbedGpioIndicator::switchRunning(bool on) {
  return switchGpio(&running_, on);
}

}  // namespace arm_mbed

}  // namespace dbger
