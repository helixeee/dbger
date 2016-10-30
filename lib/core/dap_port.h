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

#ifndef LIB_CORE_DAP_PORT_H_
#define LIB_CORE_DAP_PORT_H_

#include <stdint.h>

namespace dbger {

class DapPort {
 public:
  enum IrRegister {
    kIrRegisterNone = (-1),

    kIrRegisterExtest = 0x00,
    kIrRegisterSample = 0x01,
    kIrRegisterPreload = 0x02,
    kIrRegisterReservedJtag = 0x03,
    kIrRegisterIntest = 0x04,
    kIrRegisterClamp = 0x05,
    kIrRegisterHighz = 0x06,
    kIrRegisterClampz = 0x07,
    kIrRegisterAbort = 0x08,
    kIrRegisterReserved9 = 0x09,
    kIrRegisterDpacc = 0x0a,
    kIrRegisterApacc = 0x0b,
    kIrRegisterReservedC = 0x0c,
    kIrRegisterReservedD = 0x0d,
    kIrRegisterIdcode = 0x0e,
    kIrRegisterBypass = 0x0f,
  };
  enum DpRegister {
    kDpRegisterReserved = 0x0,
    kDpRegisterSwAbort = 0x0,  // SW-DP, DPv1,2(WO)

    kDpRegisterCtrlStat = 0x4,  // DPv0,1,2(RW)

    kDpRegisterSelect = 0x8,  // DPv0(RW) DPv1,2(WO)
    kDpRegisterSwResend = 0x8,  // SW-DP, DPv1,2(RO)

    kDpRegisterRdbuff = 0xC,  // DPv0,1,2(RO)
    kDpRegisterSwTargetsel = 0xC,  // SW-DP, DPv2(WO)
  };

  enum PinMap {
    kPinMapSwclkTck = (1 << 0),
    kPinMapSwdioTms = (1 << 1),
    kPinMapTdi = (1 << 2),
    kPinMapTdo = (1 << 3),
    kPinMapNTrst = (1 << 5),
    kPinMapNReset = (1 << 7),
  };

  enum TransferRequest {
    kTransferRequestAPnDP = (1 << 0),
    kTransferRequestRnW = (1 << 1),
    kTransferRequestA2 = (1 << 2),
    kTransferRequestA3 = (1 << 3),
    kTransferRequestAddress = (kTransferRequestA2 | kTransferRequestA3),
    kTransferRequestValueMatch = (1 << 4),
    kTransferRequestMatchMask = (1 << 5),
  };

  enum SwjSequence {
    kSwjSequenceMaxCycle = 8,
  };
  enum DataSize {
    kDataSize = 32,
  };

  DapPort();
  virtual ~DapPort();

  virtual bool connect();
  virtual bool disconnect();

  virtual bool writeAbort(uint8_t index, uint32_t abort);

  virtual bool isImplementedResetSequence() const;
  virtual bool reset();

  virtual uint8_t setPins(uint8_t pin_output,
                          uint8_t pin_select,
                          uint32_t wait_usec);

  virtual bool setClock(uint32_t clock);
  virtual bool sequenceSwj(int bit_count,
                           uint8_t bit_data);

  virtual bool configurateSwd(uint8_t turnaround_clock,
                              bool generate_data_phase);

  virtual bool sequenceJtag(int tck_cycles,
                            int tms_value,
                            uint8_t tdi_data,
                            uint8_t *tdo_data);

  virtual bool setNumberOfJtagDevices(int count);
  virtual bool setJtagIrLength(int index, uint8_t ir_length);

  virtual bool readJtagIdcode(uint8_t jtag_index, uint32_t *idcode);

  virtual bool checkJtagIndex(uint8_t index);

  virtual bool setIdleCycles(int idle_cycles);

  typedef int (DapPort::*transfer_func_ptr)(uint32_t write_data,
                                            uint32_t *read_data);
  virtual transfer_func_ptr transitionStateMachine(uint8_t transfer_requst);
  virtual transfer_func_ptr terminateStateMachine();

  int read(uint32_t write_data, uint32_t *read_data);
  int write(uint32_t write_data, uint32_t *read_data);
  int nop(uint32_t write_data, uint32_t *read_data);

  bool initStateMachine(uint8_t device_index);

  bool isFlagWrite() const;
  bool isFlagRead() const;
  bool isFlagRewind() const;
  bool isFlagMask() const;
  bool isFlagMatch() const;
  bool isFlagResponse() const;

 protected:
  void initState();

  void clearNextState();
  void clearStateFlag();

  virtual int readImpl(uint32_t write_data, uint32_t *read_data);
  virtual int writeImpl(uint32_t write_data, uint32_t *read_data);
  virtual int nopImpl(uint32_t write_data, uint32_t *read_data);

  struct TransferStateInformation {
    int next_state;

    int current_ir_register;
    int previous_ir_register;

    int address;

    bool flag_write_data;
    bool flag_read_data;
    bool flag_rewind_command;
    bool flag_mask;
    bool flag_match;
    bool flag_response;
  } transfer_information;

  int idle_cycles_;

 private:
  void updateTransfer();
};

class DapPortSwd : public DapPort {
 public:
  enum SwdProtocol {
    kSwdProtocolStart = 1,
    kSwdProtocolAPnDPApacc = 1,
    kSwdProtocolAPnDPDpacc = 0,
    kSwdProtocolRnWRead = 1,
    kSwdProtocolRnWWrite = 0,
    kSwdProtocolStop = 0,
    kSwdProtocolPark = 1,
  };
  enum SwdCycle {
    kSwdCycleAck = 3,
  };

  DapPortSwd();
  virtual ~DapPortSwd();

  virtual bool writeAbort(uint8_t index, uint32_t abort);

  virtual bool configurateSwd(uint8_t turnaround_clock,
                              bool generate_data_phase);
  virtual transfer_func_ptr transitionStateMachine(uint8_t transfer_reuest);

 protected:
  uint8_t turnaround_clock_;
  bool generate_data_phase_;
};

class DapPortJtag : public DapPort {
 public:
  enum JtagChain {
    kJtagChainIrLength = 4,
  };

  virtual ~DapPortJtag();

  virtual transfer_func_ptr transitionStateMachine(uint8_t transfer_request);
};

}  // namespace dbger

#endif  // LIB_CORE_DAP_PORT_H_
