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

#include "core/cmsis_dap.h"

using dbger::CmsisDap;

TEST_GROUP(CmsisDap) {
};

TEST(CmsisDap, CommandId) {
  BYTES_EQUAL(0x00, CmsisDap::kCommandIdDapInfo);
  BYTES_EQUAL(0x01, CmsisDap::kCommandIdDapHostStatus);
  BYTES_EQUAL(0x02, CmsisDap::kCommandIdDapConnect);
  BYTES_EQUAL(0x03, CmsisDap::kCommandIdDapDisconnect);
  BYTES_EQUAL(0x08, CmsisDap::kCommandIdDapWriteABORT);
  BYTES_EQUAL(0x09, CmsisDap::kCommandIdDapDelay);
  BYTES_EQUAL(0x0a, CmsisDap::kCommandIdDapResetTarget);

  BYTES_EQUAL(0x10, CmsisDap::kCommandIdDapSwjPins);
  BYTES_EQUAL(0x11, CmsisDap::kCommandIdDapSwjClock);
  BYTES_EQUAL(0x12, CmsisDap::kCommandIdDapSwjSequence);

  BYTES_EQUAL(0x13, CmsisDap::kCommandIdDapSwdConfigure);

  BYTES_EQUAL(0x17, CmsisDap::kCommandIdDapSwoTransport);
  BYTES_EQUAL(0x18, CmsisDap::kCommandIdDapSwoMode);
  BYTES_EQUAL(0x19, CmsisDap::kCommandIdDapSwoBaudrate);
  BYTES_EQUAL(0x1a, CmsisDap::kCommandIdDapSwoControl);
  BYTES_EQUAL(0x1a, CmsisDap::kCommandIdDapSwoControl);
  BYTES_EQUAL(0x1b, CmsisDap::kCommandIdDapSwoStatus);
  BYTES_EQUAL(0x1c, CmsisDap::kCommandIdDapSwoData);

  BYTES_EQUAL(0x14, CmsisDap::kCommandIdDapJtagSequence);
  BYTES_EQUAL(0x15, CmsisDap::kCommandIdDapJtagConfigure);
  BYTES_EQUAL(0x16, CmsisDap::kCommandIdDapJtagIDCODE);

  BYTES_EQUAL(0x04, CmsisDap::kCommandIdDapTransferConfigure);
  BYTES_EQUAL(0x05, CmsisDap::kCommandIdDapTransfer);
  BYTES_EQUAL(0x06, CmsisDap::kCommandIdDapTransferBlock);
  BYTES_EQUAL(0x07, CmsisDap::kCommandIdDapTransferAbort);

  BYTES_EQUAL(0x7f, CmsisDap::kCommandIdDapExecuteCommands);
  BYTES_EQUAL(0x7e, CmsisDap::kCommandIdDapQueueCommands);
}

TEST(CmsisDap, ResponseStatus) {
  BYTES_EQUAL(0x00, CmsisDap::kResponseStatusOk);
  BYTES_EQUAL(0xff, CmsisDap::kResponseStatusError);
}

TEST(CmsisDap, TransferRequest) {
  BYTES_EQUAL(0x1 << 0, CmsisDap::kTransferRequestAPnDP);
  BYTES_EQUAL(0x1 << 1, CmsisDap::kTransferRequestRnW);
  BYTES_EQUAL(0x3 << 2, CmsisDap::kTransferRequestAddress);
  BYTES_EQUAL(0x1 << 4, CmsisDap::kTransferRequestValueMatch);
  BYTES_EQUAL(0x1 << 5, CmsisDap::kTransferRequestMatchMask);
}

TEST(CmsisDap, InfoId) {
  BYTES_EQUAL(0x01, CmsisDap::kInfoIdVendorId);
  BYTES_EQUAL(0x02, CmsisDap::kInfoIdProductId);
  BYTES_EQUAL(0x03, CmsisDap::kInfoIdSerialNumber);
  BYTES_EQUAL(0x04, CmsisDap::kInfoIdFirmwareVersion);
  BYTES_EQUAL(0x05, CmsisDap::kInfoIdTargetDeviceVendor);
  BYTES_EQUAL(0x06, CmsisDap::kInfoIdTargetDeviceName);
  BYTES_EQUAL(0xF0, CmsisDap::kInfoIdCapabilities);
  BYTES_EQUAL(0xFD, CmsisDap::kInfoIdSwoTraceBufferSize);
  BYTES_EQUAL(0xFE, CmsisDap::kInfoIdPacketCount);
  BYTES_EQUAL(0xFF, CmsisDap::kInfoIdPacketSize);
}

TEST(CmsisDap, HostStatusType) {
  BYTES_EQUAL(0x00, CmsisDap::kHostStatusTypeConnect);
  BYTES_EQUAL(0x01, CmsisDap::kHostStatusTypeRunning);
}

TEST(CmsisDap, HostStatus) {
  BYTES_EQUAL(0x00, CmsisDap::kHostStatusOff);
  BYTES_EQUAL(0x01, CmsisDap::kHostStatusOn);
}

TEST(CmsisDap, DapPortMode) {
  BYTES_EQUAL(0x00, CmsisDap::kDapPortModeDefault);
  BYTES_EQUAL(0x01, CmsisDap::kDapPortModeSwd);
  BYTES_EQUAL(0x02, CmsisDap::kDapPortModeJtag);
  BYTES_EQUAL(0x00, CmsisDap::kDapPortModeFailed);
  BYTES_EQUAL(0x00, CmsisDap::kDapPortModeNone);
}

TEST(CmsisDap, TransferAck) {
  BYTES_EQUAL(0x00, CmsisDap::kTransferAckError);
  BYTES_EQUAL(0x01, CmsisDap::kTransferAckOk);
  BYTES_EQUAL(0x01, CmsisDap::kTransferAckSwdOk);
  BYTES_EQUAL(0x01, CmsisDap::kTransferAckJtagOkFault);
  BYTES_EQUAL(0x02, CmsisDap::kTransferAckWait);
  BYTES_EQUAL(0x04, CmsisDap::kTransferAckFault);
  BYTES_EQUAL(0x07, CmsisDap::kTransferNoAck);
  BYTES_EQUAL(0x08, CmsisDap::kTransferSwdProtocolError);
  BYTES_EQUAL(0x10, CmsisDap::kTransferReadMismatch);
}

TEST(CmsisDap, ResetSequence) {
  BYTES_EQUAL(0x00, CmsisDap::kResetSequenceNotImplemented);
  BYTES_EQUAL(0x01, CmsisDap::kResetSequenceImplemented);
}

TEST(CmsisDap, DapInfoCapabilities) {
  BYTES_EQUAL(0x00, CmsisDap::kDapInfoCapabilitiesNone);
  BYTES_EQUAL(0x01 << 0, CmsisDap::kDapInfoCapabilitiesSwd);
  BYTES_EQUAL(0x01 << 1, CmsisDap::kDapInfoCapabilitiesJtag);
  BYTES_EQUAL(0x01 << 2, CmsisDap::kDapInfoCapabilitiesSwoUart);
  BYTES_EQUAL(0x01 << 3, CmsisDap::kDapInfoCapabilitiesSwoManchester);
}
