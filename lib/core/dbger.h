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

#ifndef LIB_CORE_DBGER_H_
#define LIB_CORE_DBGER_H_

#include "core/cmsis_dap.h"
#include "core/dap_port.h"
#include "core/host_adapter.h"
#include "core/indicator.h"

#ifdef __MBED__
#include "mbed/mbed_cmsis_dap.h"
#include "mbed/mbed_dap_port.h"
#include "mbed/mbed_gpio_indicator.h"
#include "mbed/mbed_gpio_pin_controller.h"
#include "mbed/mbed_pin_controller.h"
#include "mbed/mbed_usbhid_host_adapter.h"
#endif

#endif  // LIB_CORE_DBGER_H_
