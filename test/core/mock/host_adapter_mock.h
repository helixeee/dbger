/** * Copyright (C) 2016 helixeee
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

#ifndef TEST_CORE_MOCK_HOST_ADAPTER_MOCK_H_
#define TEST_CORE_MOCK_HOST_ADAPTER_MOCK_H_

#include "core/host_adapter.h"

namespace dbger {

class HostAdapterMock : public HostAdapter {
 public:
  HostAdapterMock();
  HostAdapterMock(uint8_t max_packet_count, uint16_t max_packet_size);

  void setMaxPacketCount(uint8_t max_packet_count);
  void setMaxPacketSize(uint16_t max_packet_size);

  virtual ~HostAdapterMock();
  virtual int command();
  virtual uint8_t getMaxPacketCount();
  virtual uint16_t getMaxPacketSize();

 private:
  uint8_t max_packet_count_;
  uint16_t max_packet_size_;
};

}  // namespace dbger

#endif  // TEST_CORE_MOCK_HOST_ADAPTER_MOCK_H_
