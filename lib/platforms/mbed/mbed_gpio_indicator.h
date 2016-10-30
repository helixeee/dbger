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

#ifndef LIB_PLATFORMS_MBED_MBED_GPIO_INDICATOR_H_
#define LIB_PLATFORMS_MBED_MBED_GPIO_INDICATOR_H_

#include "core/indicator.h"

namespace mbed {

class DigitalOut;

}  // namespace mbed

namespace dbger {

namespace arm_mbed {

using mbed::DigitalOut;

class MbedGpioIndicator : public Indicator {
 public:
  MbedGpioIndicator(DigitalOut *connect, DigitalOut *running);
  virtual ~MbedGpioIndicator();

  virtual bool switchConnect(bool on);
  virtual bool switchRunning(bool on);

 private:
  MbedGpioIndicator();

  DigitalOut *connect_;
  DigitalOut *running_;
};

}  // namespace arm_mbed

}  // namespace dbger

#endif  // LIB_PLATFORMS_MBED_MBED_GPIO_INDICATOR_H_
