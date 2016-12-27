#include "FZServo.h"

//for the situation of only one enable pin.when power on,the host should be in recieve mode when tx enabled
//by low voltage level while rx enabled by high;otherwise you should set reverse true.
FZServo::FZServo(HardwareSerial *serial, word baudrate, byte enable, bool reverse=false)
{
  svSer = serial;
  svSer->begin(baudrate);
  svSer->setTimeout(25);

  twoPinsCtrl = false;

  _rxEn = enable;
  _rxRvs = reverse;

  pinMode(_rxEn, OUTPUT);
  digitalWrite(_rxEn, !_rxRvs);
}


FZServo::FZServo(HardwareSerial *serial, word baudrate, byte txEn, byte rxEn, bool txRvs=false, bool rxRvs=false)
{
  svSer = serial;
  svSer->begin(baudrate);
  svSer->setTimeout(25);

  twoPinsCtrl = true;

  _txEn = txEn;
  _txRvs = txRvs;
  _rxEn = rxEn;
  _rxRvs = rxRvs;

  pinMode(_txEn, OUTPUT);
  digitalWrite(_txEn, _txRvs);
  pinMode(_rxEn, OUTPUT);
  digitalWrite(_rxEn, !_rxRvs);
}


void FZServo::changeToWrite()
{
  digitalWrite(_rxEn, _rxRvs);
  if (twoPinsCtrl)
    digitalWrite(_txEn, !_txRvs);
}


void FZServo::changeToRead()
{
  if (twoPinsCtrl)
    digitalWrite(_txEn, _txRvs);
  digitalWrite(_rxEn, !_rxRvs);
}


byte FZServo::checkSum(byte *buf, byte len)
{
  byte sum;
  for(byte cnt=2; cnt<len; cnt++){
    sum += buf[cnt];
  }
  return ~sum;
}


byte FZServo::ping(byte id)
{
  byte buf[6] = {0xff, 0xff};
  buf[2] = id;
  buf[3] = 0x02;
  buf[4] = 0x01;
  buf[5] = checkSum(buf, 5);

  changeToWrite();
  svSer -> write(buf, 6);
  svSer -> flush();

  changeToRead();
  byte cnt = svSer -> readBytes(buf, 6);
  if (cnt == 6 && buf[1] == 0xf5) {
    if (checkSum(buf, 5) == buf[5])
      return buf[4];
  }
  return 0xff;
}


void FZServo::reset(byte id)
{
  byte buf[6] = {0xff, 0xff};
  buf[2] = id;
  buf[3] = 0x02;
  buf[4] = 0x06;
  buf[5] = checkSum(buf, 5);

  changeToWrite();
  svSer -> write(buf, 6);
  svSer -> flush();

  changeToRead();
}
