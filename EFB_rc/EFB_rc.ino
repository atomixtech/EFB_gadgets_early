#include <EEPROM.h>
#include "EFBSerial.h"

#define PIN_RC_1  A0
#define PIN_RC_2  A1
#define PIN_RC_3  A2
#define PIN_RC_4  A3

int pin[] = {PIN_RC_1, PIN_RC_2, PIN_RC_3, PIN_RC_4};
unsigned long t[]={0, 0, 0, 0}; // pulse rising time (micoseconds)
unsigned long v[]={0, 0, 0, 0}; // pulse width 0~1000 (micoseconds)

EFBSerial EFB;

boolean changed = false;
unsigned long lastTime = micros();

void changeDeviceTagCallback(const char* tag, uint16_t length)
{
  // write device tag to eeprom
  for (int n=0 ; n<length ; n++) {
    EEPROM.write(n, tag[n]);
  }
  EEPROM.write(length, 0);
}

void readAnalogDataCallback(uint16_t channel)
{
  if (channel>=1 && channel<=4) {
    EFB.sendAnalogData(channel, v[channel-1]);
  }
}

void setup() {
  
  pinMode(PIN_RC_1, INPUT);
  pinMode(PIN_RC_2, INPUT);
  pinMode(PIN_RC_3, INPUT);
  pinMode(PIN_RC_4, INPUT);
  
  // set callbacks to process requests
  EFB.setHandleChangeDeviceTag(changeDeviceTagCallback);
  EFB.setHandleReadAnalogData(readAnalogDataCallback);
  
  // read device tag from eeprom
  char tag[16];
  for (int n=0 ; n<16 ; n++) {
    tag[n] = EEPROM.read(n);
    if (tag[n]=='\xff') tag[n] = 0; 
  }
  
  EFB.begin(
    9600,             // serial baud rate 
    "EFB_RC",         // device name
    0x01,             // device version
    tag);             // device tag    
}

void loop() {

  // check pulse width of each rc channel
  unsigned long now = micros();
  int busy = 0;
  for (int n=0 ; n<4 ; n++) {
    int pinValue = digitalRead(pin[n]);
    busy += pinValue;
    if (t[n]==0 && pinValue==HIGH) {
      // pulse rising
      t[n] = micros();
    } else if (t[n]>0 && pinValue==LOW) {
      // pulse falling
      if (now>t[n]) {
        unsigned long pulseWidth = constrain(now-t[n], 1000,2000)-1000;
        if (pulseWidth != v[n]) {
          v[n] = pulseWidth;
          changed = true;
        }
      }
      t[n] = 0;
    }
  }
  
  // send serial dataframe
  if (changed && !busy && now - lastTime > 2500) {
    for (int n=0 ; n<4 ; n++) {
      EFB.sendAnalogData(n+1, v[n]);
    }
    changed = false;
    lastTime = now;
  }
  
  EFB.read();
}

