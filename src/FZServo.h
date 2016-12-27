#ifndef FZSERVO_h
#define FZSERVO_h

#include <Arduino.h>

class FZServo
{
  private:

    //for uart control
    bool twoPinsCtrl;
    bool _rxRvs;
    bool _txRvs;
    byte _rxEn;
    byte _txEn;

    HardwareSerial *svSer;

    void changeToWrite();
    void changeToRead();

    byte checkSum(byte *buf, byte len);

  public:
    FZServo(HardwareSerial *serial, word baudrate, byte enable, bool reverse=false);
    FZServo(HardwareSerial *serial, word baudrate, byte txEn, byte rxEn, bool txRvs=false, bool rxRvs=false);
    byte ping(byte id);
    void reset(byte id);
};

#endif
