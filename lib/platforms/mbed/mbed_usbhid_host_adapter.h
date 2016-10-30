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

#ifndef LIB_PLATFORMS_MBED_MBED_USBHID_HOST_ADAPTER_H_
#define LIB_PLATFORMS_MBED_MBED_USBHID_HOST_ADAPTER_H_

#include "core/host_adapter.h"

#include "USBHID.h"

namespace dbger {

namespace arm_mbed {

template<int COMMAND_BUFFER_SIZE = 1>
class MbedUsbhidHostAdapter : public HostAdapter, public USBHID {
 public:
  MbedUsbhidHostAdapter(uint8_t output_report_length = 64,
                        uint8_t input_report_length = 64,
                        uint16_t vendor_id = 0x1234,
                        uint16_t product_id = 0x0006,
                        uint16_t product_release = 0x0001,
                        bool connect = true)
      : USBHID(output_report_length,
               input_report_length,
               vendor_id,
               product_id,
               product_release,
               false) {
    if (connect) {
      USBHID::connect();
    }
  }
  virtual ~MbedUsbhidHostAdapter() {}

  bool hasCommand() {
    return !command_buffer_.isEmpty();
  }
  bool hasResponse() {
    return !response_buffer_.isEmpty();
  }

  virtual int command() {
    if (response_buffer_.isFull()) {
      return kHostAdapterStatusResponseFull;
    } else if (command_buffer_.isEmpty()) {
      if (!response_buffer_.isEmpty()) {
        return kHostAdapterStatusHasResponse;
      } else {
        return kHostAdapterStatusCommandEmpty;
      }
    }

    HID_REPORT *command = command_buffer_.read();
    HID_REPORT *response = response_buffer_.write();

    cmsis_dap_->command(command->length, command->data,
                        &response->length, response->data);

    command_buffer_.pop();
    response_buffer_.push();

    return kHostAdapterStatusOk;
  }

  bool sendResponse() {
    if (response_buffer_.isEmpty()) {
      return false;
    }

    HID_REPORT *response = response_buffer_.read();
    if (USBDevice::write(EPINT_IN,
                         response->data,
                         response->length,
                         MAX_HID_REPORT_SIZE)) {
      response_buffer_.pop();
      return true;
    }

    return false;
  }

  virtual uint8_t getMaxPacketCount() {
    return COMMAND_BUFFER_SIZE;
  }
  virtual uint16_t getMaxPacketSize() {
    return MAX_HID_REPORT_SIZE;
  }

 protected:
  virtual uint8_t *stringIproductDesc() {
    static uint8_t i_product_desc[] = {
      0x20,
      STRING_DESCRIPTOR,
      'd', 0, 'b', 0, 'g', 0, 'e', 0, 'r', 0,
      ' ', 0,
      'C', 0, 'M', 0, 'S', 0, 'I', 0, 'S', 0, '-', 0, 'D', 0, 'A', 0, 'P', 0,
    };
    return i_product_desc;
  }
  virtual bool EPINT_OUT_callback() {
    if (command_buffer_.isFull()) {
      return false;
    }

    HID_REPORT *command = command_buffer_.write();

    USBDevice::readEP(EPINT_OUT,
                      command->data,
                      &command->length,
                      MAX_HID_REPORT_SIZE);

    if (!readStart(EPINT_OUT, MAX_HID_REPORT_SIZE)) {
      return false;
    }

    if (command->data[0] == CmsisDap::kCommandIdDapTransferAbort) {
      abortTransfer();
      return true;
    }

    command_buffer_.push();

    return true;
  }

 private:
  CommandBuffer<HID_REPORT, COMMAND_BUFFER_SIZE + 1> command_buffer_;
  CommandBuffer<HID_REPORT, COMMAND_BUFFER_SIZE> response_buffer_;
};

}  // namespace arm_mbed

}  // namespace dbger

#endif  // LIB_PLATFORMS_MBED_MBED_USBHID_HOST_ADAPTER_H_
