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

#ifndef LIB_PLATFORMS_MBED_MBED_PIN_CONTROLLER_H_
#define LIB_PLATFORMS_MBED_MBED_PIN_CONTROLLER_H_

namespace dbger {

namespace arm_mbed {

struct MbedPinController {
 public:
  enum PinMode {
    kPinModeInput,
    kPinModeOutput,
  };
};

class MbedPinControllerSwd : public MbedPinController {
 public:
  virtual ~MbedPinControllerSwd();

  virtual bool setModeSwclkTck(PinMode mode);
  virtual int readSwclkTck();
  virtual void writeSwclkTck(int value);

  virtual bool setModeSwdioTms(PinMode mode);
  virtual int readSwdioTms();
  virtual void writeSwdioTms(int value);

  virtual bool setModeNReset(PinMode mode);
  virtual int readNReset();
  virtual void writeNReset(int value);
};

class MbedPinControllerJtag : public MbedPinControllerSwd {
 public:
  virtual ~MbedPinControllerJtag();

  virtual bool setModeTdi(PinMode mode);
  virtual int readTdi();
  virtual void writeTdi(int value);

  virtual bool setModeTdo(PinMode mode);
  virtual int readTdo();
  virtual void writeTdo(int value);

  virtual bool setModeNTrst(PinMode mode);
  virtual int readNTrst();
  virtual void writeNTrst(int value);
};

}  // namespace arm_mbed

}  // namespace dbger

#endif  // LIB_PLATFORMS_MBED_MBED_PIN_CONTROLLER_H_
