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

#include "platforms/mbed/mbed_pin_controller.h"

namespace dbger {

namespace arm_mbed {

MbedPinControllerSwd::~MbedPinControllerSwd() {
}

bool MbedPinControllerSwd::setModeSwclkTck(PinMode /*mode*/) {
  return false;
}

int MbedPinControllerSwd::readSwclkTck() {
  return 0;
}

void MbedPinControllerSwd::writeSwclkTck(int /*value*/) {
}

bool MbedPinControllerSwd::setModeSwdioTms(PinMode /*mode*/) {
  return false;
}

int MbedPinControllerSwd::readSwdioTms() {
  return 0;
}

void MbedPinControllerSwd::writeSwdioTms(int /*value*/) {
}

bool MbedPinControllerSwd::setModeNReset(PinMode /*mode*/) {
  return false;
}

int MbedPinControllerSwd::readNReset() {
  return 0;
}

void MbedPinControllerSwd::writeNReset(int /*value*/) {
}


MbedPinControllerJtag::~MbedPinControllerJtag() {
}

bool MbedPinControllerJtag::setModeTdi(PinMode /*mode*/) {
  return false;
}

int MbedPinControllerJtag::readTdi() {
  return 0;
}

void MbedPinControllerJtag::writeTdi(int /*value*/) {
}

bool MbedPinControllerJtag::setModeTdo(PinMode /*mode*/) {
  return false;
}

int MbedPinControllerJtag::readTdo() {
  return 0;
}

void MbedPinControllerJtag::writeTdo(int /*value*/) {
}

bool MbedPinControllerJtag::setModeNTrst(PinMode /*mode*/) {
  return false;
}

int MbedPinControllerJtag::readNTrst() {
  return 0;
}

void MbedPinControllerJtag::writeNTrst(int /*value*/) {
}

}  // namespace arm_mbed

}  // namespace dbger
