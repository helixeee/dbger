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

#include "test/core/mock/host_adapter_mock.h"

#include "CppUTestExt/MockSupport.h"

namespace dbger {

HostAdapterMock::HostAdapterMock()
    : max_packet_count_(0), max_packet_size_(0) {
}

HostAdapterMock::HostAdapterMock(uint8_t max_packet_count,
                                 uint16_t max_packet_size)
    : max_packet_count_(max_packet_count), max_packet_size_(max_packet_size) {
}

void HostAdapterMock::setMaxPacketCount(uint8_t max_packet_count) {
  max_packet_count_ = max_packet_count;
}

void HostAdapterMock::setMaxPacketSize(uint16_t max_packet_size) {
  max_packet_size_ = max_packet_size;
}

HostAdapterMock::~HostAdapterMock() {
}

int HostAdapterMock::command() {
  mock().actualCall("HostAdapter::command");
  return mock().returnIntValueOrDefault(kHostAdapterStatusOk);
}

uint8_t HostAdapterMock::getMaxPacketCount() {
  mock().actualCall("HostAdapter::getMaxPacketCount");
  return mock().returnIntValueOrDefault(0);
}

uint16_t HostAdapterMock::getMaxPacketSize() {
  mock().actualCall("HostAdapter::getMaxPacketSize");
  return mock().returnIntValueOrDefault(0);
}

}  // namespace dbger
