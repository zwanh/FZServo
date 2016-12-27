/*




*/

#include <FZServo.h>

#define txEn  41
#define rxEn  38

FZServo armSrv(&Serial1, 115200, txEn, rxEn, true, true);

void setup(){

  Serial.begin(115200);

  delay(500);

  for(byte i=0; i<254; i++){
    byte id = armSrv.ping(i);
    if (id != 0xff) {
      Serial.print("found the servo:");
      Serial.println(id, DEC);
    }
  }
}


void loop(){

}
