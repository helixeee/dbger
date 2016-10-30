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

#ifndef LIB_CORE_CMSIS_DAP_H_
#define LIB_CORE_CMSIS_DAP_H_

#include <stdint.h>

namespace dbger {

class DapPort;
class HostAdapter;
class Indicator;

class CmsisDap {
 public:
  enum CmsisDapStatus {
    kCmsisDapStatusOk = 0,
    kCmsisDapStatusError = (-1),
    kCmsisDapStatusUndefinedCommandId = (-2),
    kCmsisDapStatusUndefinedInfoId = (-3),
    kCmsisDapStatusPortIsNotConnected = (-4),
    kCmsisDapStatusIndicatorIsNotConnected = (-5),
    kCmsisDapStatusIllegalCommandInformation = (-6),
    kCmsisDapStatusIllegalCommandField = (-7),
    kCmsisDapStatusTransferAbort = (-8),
  };
  enum CommandId {
    kCommandIdDapInfo = 0x00,
    kCommandIdDapHostStatus = 0x01,
    kCommandIdDapConnect = 0x02,
    kCommandIdDapDisconnect = 0x03,
    kCommandIdDapWriteABORT = 0x08,
    kCommandIdDapDelay = 0x09,
    kCommandIdDapResetTarget = 0x0a,

    kCommandIdDapSwjPins = 0x10,
    kCommandIdDapSwjClock = 0x11,
    kCommandIdDapSwjSequence = 0x12,

    kCommandIdDapSwdConfigure = 0x13,

    kCommandIdDapSwoTransport = 0x17,
    kCommandIdDapSwoMode = 0x18,
    kCommandIdDapSwoBaudrate = 0x19,
    kCommandIdDapSwoControl = 0x1a,
    kCommandIdDapSwoStatus = 0x1b,
    kCommandIdDapSwoData = 0x1c,

    kCommandIdDapJtagSequence = 0x14,
    kCommandIdDapJtagConfigure = 0x15,
    kCommandIdDapJtagIDCODE = 0x16,

    kCommandIdDapTransferConfigure = 0x04,
    kCommandIdDapTransfer = 0x05,
    kCommandIdDapTransferBlock = 0x06,
    kCommandIdDapTransferAbort = 0x07,

    kCommandIdDapExecuteCommands = 0x7f,
    kCommandIdDapQueueCommands = 0x7e,
  };
  enum ResponseStatus {
    kResponseStatusOk = 0,
    kResponseStatusError = (-1),
  };
  enum InfoId {
    kInfoIdVendorId = 0x01,
    kInfoIdProductId = 0x02,
    kInfoIdSerialNumber = 0x03,
    kInfoIdFirmwareVersion = 0x04,
    kInfoIdTargetDeviceVendor = 0x05,
    kInfoIdTargetDeviceName = 0x06,

    kInfoIdCapabilities = 0xf0,

    kInfoIdSwoTraceBufferSize = 0xfd,
    kInfoIdPacketCount = 0xfe,
    kInfoIdPacketSize = 0xff,
  };
  enum DapInfoCapabilities {
    kDapInfoCapabilitiesNone = 0,
    kDapInfoCapabilitiesSwd = (1 << 0),
    kDapInfoCapabilitiesJtag = (1 << 1),
    kDapInfoCapabilitiesSwoUart = (1 << 2),
    kDapInfoCapabilitiesSwoManchester = (1 << 3),
  };
  enum HostStatusType {
    kHostStatusTypeConnect = 0,
    kHostStatusTypeRunning = 1,
  };
  enum HostStatus {
    kHostStatusOff = 0,
    kHostStatusOn = 1,
  };
  enum DapSwdConfigure {
    kDapSwdConfigureTurnaroundClock = (0x3 << 0),
    kDapSwdConfigureGenerateDataPhase = (0x1 << 2),
  };
  enum JtagSequenceInfo {
    kJtagSequenceInfoTckCycles = 0x3f,
    kJtagSequenceInfoTmsValue = (0x1 << 6),
    kJtagSequenceInfoTmsValueBit = 6,
    kJtagSequenceInfoTdoCapture = (0x1 << 7),
  };
  enum TransferRequest {
    kTransferRequestAPnDP = 0x1,
    kTransferRequestRnW = (0x1 << 1),
    kTransferRequestAddress = (0x3 << 2),
    kTransferRequestValueMatch = (0x1 << 4),
    kTransferRequestMatchMask = (0x1 << 5),
  };
  enum DapPortMode {
    kDapPortModeDefault = 0,
    kDapPortModeSwd = 1,
    kDapPortModeJtag = 2,
    kDapPortModeFailed = 0,
    kDapPortModeNone = 0,
  };
  enum ResetSequence {
    kResetSequenceNotImplemented = 0,
    kResetSequenceImplemented = 1,
  };
  enum TransferAck {
    kTransferAckError = 0x0,
    kTransferAckOk = 0x1,
    kTransferAckSwdOk = kTransferAckOk,
    kTransferAckJtagOkFault = kTransferAckOk,
    kTransferAckWait = 0x2,
    kTransferAckFault = 0x4,
    kTransferNoAck = 0x7,
    kTransferSwdProtocolError = 0x8,
    kTransferReadMismatch = 0x10,
  };

  CmsisDap();
  virtual ~CmsisDap();

  int command(uint32_t command_length,
              const uint8_t command[],
              uint32_t *response_length,
              uint8_t response[]);

  bool abortTransfer();

  HostAdapter* attachHostAdapter(HostAdapter *host_adapter);
  Indicator* attachIndicator(Indicator *indicator);
  DapPort* attachSwdPort(DapPort *swd_port);
  DapPort* attachJtagPort(DapPort *jtag_port);

 protected:
  virtual int commandDapVendorCommand();

  uint32_t copyInfoString(const char *info_string);

  virtual uint32_t getInfoVendorId();
  virtual uint32_t getInfoProductId();
  virtual uint32_t getInfoSerialNumber();
  virtual uint32_t getInfoFirmwareVersion();
  virtual uint32_t getInfoTargetDeviceVendor();
  virtual uint32_t getInfoTargetDeviceName();

  virtual int connectDefaultPortOnDapConnect();
  int connectPortOnDapConnect(DapPort **dap_port,
                              DapPortMode response_port);

  virtual bool delay(uint16_t delay_usec);

  Indicator *indicator_;

  DapPort *swd_port_;
  DapPort *jtag_port_;
  DapPort *access_port_;

  uint32_t command_length_;
  const uint8_t *command_;
  uint32_t *response_length_;
  uint8_t *response_;

 private:
  int commandDapInfo();
  uint8_t getInfoCapabilities();
  uint32_t getInfoSwoTraceBufferSize();
  uint8_t getInfoPacketCount();
  uint16_t getInfoPacketSize();

  int commandDapHostStatus();

  int commandDapConnect();

  int commandDapDisconnect();
  int commandDapWriteABORT();
  int commandDapDelay();
  int commandDapResetTarget();

  int commandDapSwjPins();
  int commandDapSwjClock();
  int commandDapSwjSequence();

  int commandDapSwdConfigure();

  int commandDapJtagSequence();
  int commandDapJtagConfigure();
  int commandDapJtagIDCODE();

  int commandDapTransferConfigure();

  int commandDapTransfer();

  int commandDapTransferBlock();
  int terminateDapTransfer(int response_transfer_count,
                           int response_data_count,
                           uint8_t *response_count_lsb,
                           uint8_t *response_count_msb);

  HostAdapter *host_adapter_;

  int transfer_wait_retry_;
  int transfer_match_retry_;

  bool transfer_abort_;
};

}  // namespace dbger

#endif  // LIB_CORE_CMSIS_DAP_H_
