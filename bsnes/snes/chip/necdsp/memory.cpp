#ifdef NECDSP_CPP

uint8 NECDSP::read(unsigned addr) {
  if(!debugger_access()) cpu.synchronize_coprocessor();
  if((addr & srmask) == srtest) return sr_read();
  if((addr & drmask) == drtest) return dr_read();
  if((addr & dpmask) == dptest) return dp_read(addr);
  return 0x00;
}

void NECDSP::write(unsigned addr, uint8 data) {
  if(!debugger_access()) cpu.synchronize_coprocessor();
  if((addr & srmask) == srtest) return sr_write(data);
  if((addr & drmask) == drtest) return dr_write(data);
  if((addr & dpmask) == dptest) return dp_write(addr, data);
}

uint8 NECDSP::sr_read() {
  return regs.sr >> 8;
}

void NECDSP::sr_write(uint8 data) {
}

uint8 NECDSP::dr_read() {
  if(debugger_access()) return regs.dr >> 0;

  if(regs.sr.drc == 0) {
    //16-bit
    if(regs.sr.drs == 0) {
      regs.sr.drs = 1;
      return regs.dr >> 0;
    } else {
      regs.sr.rqm = 0;
      regs.sr.drs = 0;
      return regs.dr >> 8;
    }
  } else {
    //8-bit
    regs.sr.rqm = 0;
    return regs.dr >> 0;
  }
}

void NECDSP::dr_write(uint8 data) {
  if(regs.sr.drc == 0) {
    //16-bit
    if(regs.sr.drs == 0) {
      regs.sr.drs = 1;
      regs.dr = (regs.dr & 0xff00) | (data << 0);
    } else {
      regs.sr.rqm = 0;
      regs.sr.drs = 0;
      regs.dr = (data << 8) | (regs.dr & 0x00ff);
    }
  } else {
    //8-bit
    regs.sr.rqm = 0;
    regs.dr = (regs.dr & 0xff00) | (data << 0);
  }
}

uint8 NECDSP::dp_read(unsigned addr) {
  bool hi = addr & 1;
  addr = (addr >> 1) & 2047;

  if(hi == false) {
    return dataRAM[addr] >> 0;
  } else {
    return dataRAM[addr] >> 8;
  }
}

void NECDSP::dp_write(unsigned addr, uint8 data) {
  bool hi = addr & 1;
  addr = (addr >> 1) & 2047;

  if(hi == false) {
    dataRAM[addr] = (dataRAM[addr] & 0xff00) | (data << 0);
  } else {
    dataRAM[addr] = (dataRAM[addr] & 0x00ff) | (data << 8);
  }
}

#endif
