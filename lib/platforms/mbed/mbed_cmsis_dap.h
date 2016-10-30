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

#ifndef LIB_PLATFORMS_MBED_MBED_CMSIS_DAP_H_
#define LIB_PLATFORMS_MBED_MBED_CMSIS_DAP_H_

#include "core/cmsis_dap.h"

namespace dbger {

namespace arm_mbed {

class MbedCmsisDap : public CmsisDap {
 public:
  MbedCmsisDap();
  virtual ~MbedCmsisDap();

 protected:
  virtual bool delay(uint16_t delay_usec);
};

}  // namespace arm_mbed

}  // namespace dbger

#endif  // LIB_PLATFORMS_MBED_MBED_CMSIS_DAP_H_
