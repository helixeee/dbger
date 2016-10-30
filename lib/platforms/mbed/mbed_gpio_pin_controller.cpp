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

#include "platforms/mbed/mbed_gpio_pin_controller.h"

#include "DigitalInOut.h"
#include "mbed_assert.h"

#include "config/cxx.h"

namespace {

using dbger::arm_mbed::MbedPinController;
using mbed::DigitalInOut;

bool setMode(DigitalInOut *pin, MbedPinController::PinMode mode) {
  switch (mode) {
    case MbedPinController::kPinModeInput:
      pin->input();
      break;
    case MbedPinController::kPinModeOutput:
      pin->output();
      break;
  }
  return true;
}

}  // namespace

namespace dbger {

namespace arm_mbed {


MbedGpioPinControllerSwd::MbedGpioPinControllerSwd(DigitalInOut *swclk,
                                                   DigitalInOut *swdio)
    : swclk_(swclk), swdio_(swdio) {
  MBED_ASSERT(swclk != nullptr);
  MBED_ASSERT(swdio != nullptr);
}

MbedGpioPinControllerSwd::~MbedGpioPinControllerSwd() {
}

bool MbedGpioPinControllerSwd::setModeSwclkTck(PinMode mode) {
  return setMode(swclk_, mode);
}

int MbedGpioPinControllerSwd::readSwclkTck() {
  return swclk_->read();
}

void MbedGpioPinControllerSwd::writeSwclkTck(int value) {
  swclk_->write(value);
}

bool MbedGpioPinControllerSwd::setModeSwdioTms(PinMode mode) {
  return setMode(swdio_, mode);
}

int MbedGpioPinControllerSwd::readSwdioTms() {
  return swdio_->read();
}

void MbedGpioPinControllerSwd::writeSwdioTms(int value) {
  swdio_->write(value);
}

MbedGpioPinControllerJtag::MbedGpioPinControllerJtag(DigitalInOut *tck,
                                                     DigitalInOut *tms,
                                                     DigitalInOut *tdi,
                                                     DigitalInOut *tdo)
    : tck_(tck), tms_(tms), tdi_(tdi), tdo_(tdo) {
  MBED_ASSERT(tck != nullptr);
  MBED_ASSERT(tms != nullptr);
  MBED_ASSERT(tdi != nullptr);
  MBED_ASSERT(tdo != nullptr);
}

MbedGpioPinControllerJtag::~MbedGpioPinControllerJtag() {
}

bool MbedGpioPinControllerJtag::setModeSwclkTck(PinMode mode) {
  return setMode(tck_, mode);
}

int MbedGpioPinControllerJtag::readSwclkTck() {
  return tck_->read();
}

void MbedGpioPinControllerJtag::writeSwclkTck(int value) {
  tck_->write(value);
}

bool MbedGpioPinControllerJtag::setModeSwdioTms(PinMode mode) {
  return setMode(tms_, mode);
}

int MbedGpioPinControllerJtag::readSwdioTms() {
  return tms_->read();
}

void MbedGpioPinControllerJtag::writeSwdioTms(int value) {
  tms_->write(value);
}

bool MbedGpioPinControllerJtag::setModeTdi(PinMode mode) {
  return setMode(tdi_, mode);
}

int MbedGpioPinControllerJtag::readTdi() {
  return tdi_->read();
}

void MbedGpioPinControllerJtag::writeTdi(int value) {
  tdi_->write(value);
}

bool MbedGpioPinControllerJtag::setModeTdo(PinMode mode) {
  return setMode(tdo_, mode);
}

int MbedGpioPinControllerJtag::readTdo() {
  return tdo_->read();
}

void MbedGpioPinControllerJtag::writeTdo(int value) {
  tdo_->write(value);
}

}  // namespace arm_mbed

}  // namespace dbger
