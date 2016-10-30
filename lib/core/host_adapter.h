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

#ifndef LIB_CORE_HOST_ADAPTER_H_
#define LIB_CORE_HOST_ADAPTER_H_

#include <stdint.h>

#include "config/cxx.h"

namespace dbger {

template<typename T, int SIZE>
class CommandBuffer {
 public:
  CommandBuffer()
      : write_pointer_(0),
        read_pointer_(0),
        amount_(0) {
  }

  bool isFull() const {
    return (amount_ == SIZE);
  }
  bool isEmpty() const {
    return (amount_ == 0);
  }

  T *read() {
    if (isEmpty()) {
      return nullptr;
    }
    return &buffer_[read_pointer_];
  }
  T *write() {
    if (isFull()) {
      return nullptr;
    }
    return &buffer_[write_pointer_];
  }

  bool pop() {
    if (isEmpty()) {
      return false;
    }

    ++read_pointer_ %= SIZE;
    amount_--;

    return true;
  }
  bool push() {
    if (isFull()) {
      return false;
    }

    ++write_pointer_ %= SIZE;
    amount_++;

    return true;
  }

 private:
  int write_pointer_;
  int read_pointer_;
  int amount_;
  T buffer_[SIZE];
};

class CmsisDap;

class HostAdapter {
 public:
  enum HostAdapterStatus {
    kHostAdapterStatusOk = 0,
    kHostAdapterStatusError = (-1),
    kHostAdapterStatusBufferError = (-2),
    kHostAdapterStatusCommandEmpty = (-3),
    kHostAdapterStatusHasCommand = (-4),
    kHostAdapterStatusResponseFull = (-5),
    kHostAdapterStatusHasResponse = (-6),
  };
  HostAdapter();
  virtual ~HostAdapter();

  CmsisDap *attach(CmsisDap *cmsis_dap);

  virtual int command();

  virtual uint8_t getMaxPacketCount();
  virtual uint16_t getMaxPacketSize();

 protected:
  bool abortTransfer();

  CmsisDap *cmsis_dap_;
};

}  // namespace dbger

#endif  // LIB_CORE_HOST_ADAPTER_H_
