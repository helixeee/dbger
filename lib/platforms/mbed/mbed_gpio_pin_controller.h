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

#ifndef LIB_PLATFORMS_MBED_MBED_GPIO_PIN_CONTROLLER_H_
#define LIB_PLATFORMS_MBED_MBED_GPIO_PIN_CONTROLLER_H_

#include "platforms/mbed/mbed_pin_controller.h"

namespace mbed {

class DigitalInOut;

}  // namespace mbed

namespace dbger {

namespace arm_mbed {

using mbed::DigitalInOut;

class MbedGpioPinControllerSwd : public MbedPinControllerSwd {
 public:
  MbedGpioPinControllerSwd(DigitalInOut *swclk, DigitalInOut *swdio);
  virtual ~MbedGpioPinControllerSwd();

  virtual bool setModeSwclkTck(PinMode mode);
  virtual int readSwclkTck();
  virtual void writeSwclkTck(int value);

  virtual bool setModeSwdioTms(PinMode mode);
  virtual int readSwdioTms();
  virtual void writeSwdioTms(int value);

 protected:
  MbedGpioPinControllerSwd();

  DigitalInOut *swclk_;
  DigitalInOut *swdio_;
};

class MbedGpioPinControllerJtag : public MbedPinControllerJtag {
 public:
  MbedGpioPinControllerJtag(DigitalInOut *tck, DigitalInOut *tms,
                            DigitalInOut *tdi, DigitalInOut *tdo);
  virtual ~MbedGpioPinControllerJtag();

  virtual bool setModeSwclkTck(PinMode mode);
  virtual int readSwclkTck();
  virtual void writeSwclkTck(int value);

  virtual bool setModeSwdioTms(PinMode mode);
  virtual int readSwdioTms();
  virtual void writeSwdioTms(int value);

  virtual bool setModeTdi(PinMode mode);
  virtual int readTdi();
  virtual void writeTdi(int value);

  virtual bool setModeTdo(PinMode mode);
  virtual int readTdo();
  virtual void writeTdo(int value);

 protected:
  MbedGpioPinControllerJtag();

  DigitalInOut *tck_;
  DigitalInOut *tms_;
  DigitalInOut *tdi_;
  DigitalInOut *tdo_;
};

}  // namespace arm_mbed

}  // namespace dbger

#endif  // LIB_PLATFORMS_MBED_MBED_GPIO_PIN_CONTROLLER_H_
