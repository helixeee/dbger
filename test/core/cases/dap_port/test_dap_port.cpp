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

#include "core/dap_port.h"

#include "core/cmsis_dap.h"

namespace dbger {

TEST_GROUP(DapPortConstant) {
};

TEST(DapPortConstant, IrRegister) {
  BYTES_EQUAL(0xff, DapPort::kIrRegisterNone);

  BYTES_EQUAL(0x00, DapPort::kIrRegisterExtest);
  BYTES_EQUAL(0x01, DapPort::kIrRegisterSample);
  BYTES_EQUAL(0x02, DapPort::kIrRegisterPreload);
  BYTES_EQUAL(0x03, DapPort::kIrRegisterReservedJtag);
  BYTES_EQUAL(0x04, DapPort::kIrRegisterIntest);
  BYTES_EQUAL(0x05, DapPort::kIrRegisterClamp);
  BYTES_EQUAL(0x06, DapPort::kIrRegisterHighz);
  BYTES_EQUAL(0x07, DapPort::kIrRegisterClampz);
  BYTES_EQUAL(0x08, DapPort::kIrRegisterAbort);
  BYTES_EQUAL(0x09, DapPort::kIrRegisterReserved9);
  BYTES_EQUAL(0x0A, DapPort::kIrRegisterDpacc);
  BYTES_EQUAL(0x0B, DapPort::kIrRegisterApacc);
  BYTES_EQUAL(0x0C, DapPort::kIrRegisterReservedC);
  BYTES_EQUAL(0x0D, DapPort::kIrRegisterReservedD);
  BYTES_EQUAL(0x0E, DapPort::kIrRegisterIdcode);
  BYTES_EQUAL(0x0F, DapPort::kIrRegisterBypass);
}

TEST(DapPortConstant, DpRegister) {
  BYTES_EQUAL(0x00, DapPort::kDpRegisterReserved);
  BYTES_EQUAL(0x00, DapPort::kDpRegisterSwAbort);

  BYTES_EQUAL(0x04, DapPort::kDpRegisterCtrlStat);

  BYTES_EQUAL(0x08, DapPort::kDpRegisterSelect);
  BYTES_EQUAL(0x08, DapPort::kDpRegisterSwResend);

  BYTES_EQUAL(0x0C, DapPort::kDpRegisterRdbuff);
  BYTES_EQUAL(0x0C, DapPort::kDpRegisterSwTargetsel);
}

TEST(DapPortConstant, PinMap) {
  BYTES_EQUAL(0x01, DapPort::kPinMapSwclkTck);
  BYTES_EQUAL(0x02, DapPort::kPinMapSwdioTms);
  BYTES_EQUAL(0x04, DapPort::kPinMapTdi);
  BYTES_EQUAL(0x08, DapPort::kPinMapTdo);
  BYTES_EQUAL(0x20, DapPort::kPinMapNTrst);
  BYTES_EQUAL(0x80, DapPort::kPinMapNReset);
}

TEST(DapPortConstant, TransferRequest) {
  BYTES_EQUAL(0x01, DapPort::kTransferRequestAPnDP);
  BYTES_EQUAL(0x02, DapPort::kTransferRequestRnW);
  BYTES_EQUAL(0x04, DapPort::kTransferRequestA2);
  BYTES_EQUAL(0x08, DapPort::kTransferRequestA3);
  BYTES_EQUAL(0x0C, DapPort::kTransferRequestAddress);
  BYTES_EQUAL(0x10, DapPort::kTransferRequestValueMatch);
  BYTES_EQUAL(0x20, DapPort::kTransferRequestMatchMask);
}

TEST(DapPortConstant, SwjSequence) {
  CHECK_EQUAL(8, DapPort::kSwjSequenceMaxCycle);
}

TEST(DapPortConstant, DataSize) {
  CHECK_EQUAL(32, DapPort::kDataSize);
}

TEST_GROUP(DapPort) {
  DapPort dap_port;
};

TEST(DapPort, function) {
  CHECK_FALSE(dap_port.connect());
  CHECK(dap_port.disconnect());

  CHECK(dap_port.writeAbort(0, 0));

  CHECK_FALSE(dap_port.isImplementedResetSequence());
  CHECK(dap_port.reset());

  BYTES_EQUAL(0, dap_port.setPins(0, 0, 0));

  CHECK(dap_port.setClock(0));
  CHECK_FALSE(dap_port.sequenceSwj(0, 0));

  CHECK_FALSE(dap_port.configurateSwd(0, false));

  CHECK_FALSE(dap_port.sequenceJtag(0, 0, 0, nullptr));

  CHECK_FALSE(dap_port.setNumberOfJtagDevices(0));
  CHECK_FALSE(dap_port.setJtagIrLength(0, 0));

  CHECK_FALSE(dap_port.readJtagIdcode(0, nullptr));

  CHECK(dap_port.checkJtagIndex(0));

  CHECK(dap_port.setIdleCycles(0));

  CHECK_EQUAL(CmsisDap::kTransferAckOk, dap_port.read(0, nullptr));
  CHECK_EQUAL(CmsisDap::kTransferAckOk, dap_port.write(0, nullptr));
  CHECK_EQUAL(CmsisDap::kTransferAckOk, dap_port.nop(0, nullptr));

  CHECK(dap_port.initStateMachine(0));

  CHECK_FALSE(dap_port.isFlagWrite());
  CHECK_FALSE(dap_port.isFlagRead());
  CHECK_FALSE(dap_port.isFlagRewind());
  CHECK_FALSE(dap_port.isFlagMask());
  CHECK_FALSE(dap_port.isFlagMatch());
  CHECK_FALSE(dap_port.isFlagResponse());
}

TEST_GROUP(DapPortSwdConstant) {
};

TEST(DapPortSwdConstant, SwdProtocol) {
  BYTES_EQUAL(1, DapPortSwd::kSwdProtocolStart);
  BYTES_EQUAL(1, DapPortSwd::kSwdProtocolAPnDPApacc);
  BYTES_EQUAL(0, DapPortSwd::kSwdProtocolAPnDPDpacc);
  BYTES_EQUAL(1, DapPortSwd::kSwdProtocolRnWRead);
  BYTES_EQUAL(0, DapPortSwd::kSwdProtocolRnWWrite);
  BYTES_EQUAL(0, DapPortSwd::kSwdProtocolStop);
  BYTES_EQUAL(1, DapPortSwd::kSwdProtocolPark);
}

TEST(DapPortSwdConstant, SwdCycle) {
  CHECK_EQUAL(3, DapPortSwd::kSwdCycleAck);
}

TEST_GROUP(DapPortSwd) {
  DapPortSwd dap_port;
};

TEST(DapPortSwd, function) {
  CHECK(dap_port.configurateSwd(0, false));
}

TEST_GROUP(DapPortJtagConstant) {
};

TEST(DapPortJtagConstant, JtagChain) {
  BYTES_EQUAL(4, DapPortJtag::kJtagChainIrLength);
}

}  // namespace dbger
