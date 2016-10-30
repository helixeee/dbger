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

#include "core/host_adapter.h"

#include "core/cmsis_dap.h"

#include "config/cxx.h"

namespace dbger {

HostAdapter::HostAdapter() : cmsis_dap_(nullptr) {
}

HostAdapter::~HostAdapter() {
}

CmsisDap *HostAdapter::attach(CmsisDap *cmsis_dap) {
  CmsisDap *old_cmsis_dap = cmsis_dap_;
  cmsis_dap_ = cmsis_dap;
  return old_cmsis_dap;
}

int HostAdapter::command() {
  return kHostAdapterStatusError;
}

uint8_t HostAdapter::getMaxPacketCount() {
  return 0;
}

uint16_t HostAdapter::getMaxPacketSize() {
  return 0;
}

bool HostAdapter::abortTransfer() {
  if (cmsis_dap_ == nullptr) {
    return false;
  }
  return cmsis_dap_->abortTransfer();
}

}  // end namespace dbger
