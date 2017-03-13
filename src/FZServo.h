#ifndef FZSERVO_h
#define FZSERVO_h

#include <Arduino.h>

#define BROADCAST_ADDR  0xFE

//the macros below is instructions
#define INSTRUCTION_PING        0x1
#define INSTRUCTION_READ_DATA   0x2
#define INSTRUCTION_WRITE_DATA  0x3
#define INSTRUCTION_REG_WRITE   0x4
#define INSTRUCTION_ACTION      0x5
#define INSTRUCTION_RESET       0x6
#define INSTRUCTION_SYNC_WRITE  0x83


//the macros below is storaged in EEPROM
#define ID_REG			0x5
#define MIN_ANGLE_LIMIT_H	0x9
#define MIN_ANGLE_LIMIT_L	0xA
#define MAX_ANGLE_LIMIT_H	0xB
#define MAX_ANGLE_LIMIT_L	0xC
#define MAX_TORQUE_H		0x10
#define MAX_TORQUE_L		0x11
#define DEFAULT_SPEED		0x12
#define MIDDLE_POSITION_H	0x14
#define MIDDLE_POSITION_L	0x15

//the macros below is storaged in RAM
#define TORQUE_SWITCH		0x28
#define TARGET_POSITION_H	0x2A
#define TARGET_POSITION_L	0x2B
#define CURRENT_POSITION_H  0x38
#define CURRENT_POSITION_L  0x39
#define TARGET_SPEED		0x41


class FZServo
{
  private:
    HardwareSerial *svSer;
    byte checkSum(byte *buf, byte len);

  public:
    FZServo(HardwareSerial *serial, long timeout);
    boolean ping(byte id, byte *data);
    boolean read(byte id, byte regStartAddr, byte *data, byte len);
    void write(byte id, byte regStartAddr, byte *buf, byte bufLen);
    void write(byte regStartAddr, byte *buf, byte svNum, byte perDataLen, boolean mode);
    void act();
    void reset(byte id);
};

#endif
