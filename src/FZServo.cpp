#include "FZServo.h"

//for the situation of only one enable pin.when power on,the host should be in recieve mode when tx enabled
//by low voltage level while rx enabled by high;otherwise you should set reverse true.
FZServo::FZServo(HardwareSerial *serial, long timeout)
{
  svSer = serial;
  svSer -> begin(115200);
  svSer -> setTimeout(timeout);
}


byte FZServo::checkSum(byte *buf, byte len)
{
  byte sum = 0;
  for(byte cnt=2; cnt<len; cnt++){
    sum += buf[cnt];
  }
  return ~sum;
}


boolean FZServo::ping(byte id, byte *data)
{
  byte buf[6] = {0xff, 0xff, id, 0x2, 0x1, ~(id+3)};

  while(svSer -> availableForWrite() < 6);
  svSer -> write(buf, 6);
  svSer -> flush();

  byte cnt = svSer -> readBytes(buf, 6);
  if (cnt == 6 && buf[0] == 0xff && buf[1] == 0xf5
      && buf[2] == id && buf[3] == 0x2 &&
      checkSum(buf, 5) == buf[5]){
    *data = buf[4];
    return true;
  }
  return false;
}


boolean FZServo::read(byte id, byte regStartAddr, byte *data, byte len)
{
  byte writeBuf[8] = {0xff, 0xff, id, 0x4, 0x2, regStartAddr,
                  len, ~(id+regStartAddr+len+6)};

  while(svSer -> availableForWrite() < 8);
  svSer -> write(writeBuf, 8);
  svSer -> flush();

  byte readBuf[len + 6];
  byte cnt = svSer -> readBytes(readBuf, len + 6);

  if (cnt == len + 6 && readBuf[0] == 0xff && readBuf[1] == 0xf5
      && readBuf[2] == id && readBuf[3] == len + 2 &&
      checkSum(readBuf, len + 5) == readBuf[len + 5]){

    memcpy(data, &readBuf[5], len);
    return true;
  }
  return false;
}


void FZServo::reset(byte id)
{
  byte buf[6] = {0xff, 0xff, id, 0x2, 0x6, ~(id+8)};

  while(svSer -> availableForWrite() < 6);
  svSer -> write(buf, 6);
  svSer -> flush();
}



void FZServo::write(byte id, byte regStartAddr, byte *buf, byte bufLen)
{
  byte writeBufLen = bufLen + 7;
  byte writeBuf[writeBufLen] = {0xff, 0xff, id, bufLen+3, INSTRUCTION_WRITE_DATA, regStartAddr};
  memcpy(&writeBuf[6], buf, bufLen);
  writeBuf[writeBufLen - 1] = checkSum(writeBuf, writeBufLen - 1);

  while(svSer -> availableForWrite() < writeBufLen);
  svSer -> write(writeBuf, writeBufLen);
  svSer -> flush();
}


//mode: true sync;  false async
void FZServo::write(byte regStartAddr, byte *buf, byte svNum, byte perDataLen, boolean mode)
{
  byte writeBufLen = perDataLen * svNum + 8;
  byte writeBuf[writeBufLen] = {0xff, 0xff, BROADCAST_ADDR, writeBufLen - 4,
                                mode ? INSTRUCTION_SYNC_WRITE : INSTRUCTION_REG_WRITE,
                                regStartAddr, perDataLen - 1};
  memcpy(&writeBuf[7], buf, writeBufLen - 8);
  writeBuf[writeBufLen - 1] = checkSum(writeBuf, writeBufLen - 1);

  while(svSer -> availableForWrite() < writeBufLen);
  svSer -> write(writeBuf, writeBufLen);
  svSer -> flush();
}


void FZServo::act()
{
  byte buf[6] = {0xff, 0xff, BROADCAST_ADDR, 0x2, 0x5, 0xfa};

  while(svSer -> availableForWrite() < 6);
  svSer -> write(buf, 6);
  svSer -> flush();
}
