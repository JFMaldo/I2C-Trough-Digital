#ifndef Morse_h
#define Morse_h

#include "Arduino.h"

class I2CDigital{
  public:
    I2CDigital(int SCLw,int SDAw);
    void pullHigh(int pin);
    void pullLow(int pin);
    byte sendByte(byte Byte);
    byte readByte(bool lastByte);
    byte startCom(byte address,byte RW);
    byte stopCom();

    int SCL;
    int SDA;
};

#endif
