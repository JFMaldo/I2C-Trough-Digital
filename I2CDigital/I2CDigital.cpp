#include "Arduino.h"
#include "I2CDigital.h"

I2CDigital::I2CDigital(int SCLw,int SDAw){
  SCL = SCLw;
  SDA = SDAw;
   
  //Makes sure the pins wont supply 5V (Screwing with the external pullup V)
  pinMode(SCL,INPUT);
  pinMode(SDA,INPUT);
  //Digital write MUST stay low
  digitalWrite(SCL,LOW);
  digitalWrite(SDA,LOW);
}

void I2CDigital::pullHigh(int pin){
  pinMode(pin,INPUT);
}

void I2CDigital::pullLow(int pin){
  pinMode(pin,OUTPUT);
}

//Will send the byte and return the ACK bit
byte I2CDigital::sendByte(byte Byte){
  //Makes sure low from the start
  pullLow(SDA);
  pullLow(SCL);

  for(int i = 0;i <= 7;i++){
    //If the MSB is 1, else 0 (Sends the MSB)
    if(Byte&0b10000000){
      pullHigh(SDA);
      delayMicroseconds(1);
      pullHigh(SCL);
      delayMicroseconds(4);
      pullLow(SCL);
      delayMicroseconds(4);
    }else{
      pullLow(SDA);
      delayMicroseconds(1);
      pullHigh(SCL);
      delayMicroseconds(4);
      pullLow(SCL);
      delayMicroseconds(4);
    }

    //Shifts bits 1 to the right, eliminating the MSB
    Byte = Byte << 1;
  }

  //Reads AKW
  pinMode(SDA,INPUT);
  pullHigh(SCL);
  delayMicroseconds(4);
  //If SDA is pulled low, ACK will be true
  byte ACK = !digitalRead(SDA);
  pullLow(SCL);
  delayMicroseconds(4);

  return ACK;
}

byte I2CDigital::readByte(bool lastByte){
  byte Byte = 0b00000000;
  pinMode(SDA,INPUT);
    
  //Reads a bit after a CLK pulse
  for(int i = 0;i<=7;i++){
    pullHigh(SCL);
    delayMicroseconds(4);
    //Puts the bit on the LSB of Byte
    Byte = Byte|digitalRead(SDA);
    Byte = Byte << 1;
    pullLow(SCL);
    delayMicroseconds(4);
  }

  //If it´s the last byte, it will let SDA high, sending NACK (from pinMode at start)
  if(!lastByte){
    pullLow(SDA);
  }

  //Sends the ACK to slave acording to last "if" statement
  delayMicroseconds(1);
  pullHigh(SCL);
  delayMicroseconds(4);
  pullLow(SCL);
  delayMicroseconds(4);

  return Byte;
}
  
byte I2CDigital::startCom(byte address,byte RW){
  //Adding RW to LSB
  address = address<<1;
  address = address|RW;
    
  //Starting state(Not stop condition)
  pullHigh(SDA);
  pullHigh(SCL);
  delayMicroseconds(4);
    
  //I2C Start condition
  pullLow(SDA);
  delayMicroseconds(4);
  pullLow(SCL);
  delayMicroseconds(4);

  return sendByte(address);
}

byte I2CDigital::stopCom(){
  //Starting state
  pullLow(SCL);
  pullLow(SDA);
  delayMicroseconds(4);

  //Stop condition
  pullHigh(SCL);
  delayMicroseconds(4);
  pullHigh(SDA);
}
