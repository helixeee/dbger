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

#include "CppUTest/TestHarness.h"

#include "core/host_adapter.h"

#include "core/cmsis_dap.h"

namespace dbger {

TEST_GROUP(HostAdapter) {
  HostAdapter host_adapter;
};

TEST(HostAdapter, MaxPacketCount) {
  CHECK_EQUAL(0, host_adapter.getMaxPacketCount());
}

TEST(HostAdapter, MaxPacketSize) {
  CHECK_EQUAL(0, host_adapter.getMaxPacketSize());
}

TEST(HostAdapter, attach) {
  CmsisDap cmsis_dap_0;
  CmsisDap cmsis_dap_1;
  CmsisDap cmsis_dap_2;

  POINTERS_EQUAL(nullptr, host_adapter.attach(&cmsis_dap_0));
  POINTERS_EQUAL(&cmsis_dap_0, host_adapter.attach(&cmsis_dap_1));
  POINTERS_EQUAL(&cmsis_dap_1, host_adapter.attach(&cmsis_dap_2));
  POINTERS_EQUAL(&cmsis_dap_2, host_adapter.attach(&cmsis_dap_2));
}

TEST(HostAdapter, command) {
  CHECK_EQUAL(HostAdapter::kHostAdapterStatusError, host_adapter.command());
}

TEST_GROUP(CommandBuffer) {
  static const int size = 1;
  CommandBuffer<int, size> buffer;
};

TEST(CommandBuffer, push_pop) {
  CHECK_FALSE(buffer.isFull());
  CHECK(buffer.isEmpty());

  for (int i = 0; i < size; i++) {
    CHECK(buffer.push());
  }

  CHECK(buffer.isFull());
  CHECK_FALSE(buffer.isEmpty());

  CHECK_FALSE(buffer.push());

  for (int i = 0; i < size; i++) {
    CHECK(buffer.pop());
  }

  CHECK_FALSE(buffer.isFull());
  CHECK(buffer.isEmpty());

  CHECK_FALSE(buffer.pop());
}

TEST(CommandBuffer, pointer) {
  int data = 0x12345678;

  int *write_data = nullptr;
  int *read_data = nullptr;

  POINTERS_EQUAL(nullptr, buffer.read());

  write_data = buffer.write();
  *write_data = data;

  for (int i = 0; i < size; i++) {
    buffer.push();
  }

  POINTERS_EQUAL(nullptr, buffer.write());

  read_data = buffer.read();
  CHECK_EQUAL(data, *read_data);
}

}  // namespace dbger
