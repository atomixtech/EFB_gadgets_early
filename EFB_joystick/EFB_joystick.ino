#include <EEPROM.h>
#include "EFBSerial.h"

#define PIN_AXIS_X  A0
#define PIN_AXIS_Y  A1
#define PIN_BUTTON  12

#define CH_AXIS_X  1
#define CH_AXIS_Y  2
#define CH_BUTTON  1

int valAxisX = 0;
int valAxisY = 0;
boolean valButton = false;

EFBSerial EFB;

boolean changed = false;
unsigned long lastTime = millis();

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
  if (channel==CH_AXIS_X) {
    EFB.sendAnalogData(channel, valAxisX);
  } else if (channel==CH_AXIS_Y) {
    EFB.sendAnalogData(channel, valAxisY);
  }
}

void readBinaryDataCallback(uint16_t channel)
{
  if (channel==CH_BUTTON) {
    EFB.sendBinaryData(channel, valButton);
  } 
}

void setup() {
  
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  
  // set callbacks to process requests
  EFB.setHandleChangeDeviceTag(changeDeviceTagCallback);
  EFB.setHandleReadAnalogData(readAnalogDataCallback);
  EFB.setHandleReadBinaryData(readBinaryDataCallback);
  
  // read device tag from eeprom
  char tag[16];
  for (int n=0 ; n<16 ; n++) {
    tag[n] = EEPROM.read(n);
    if (tag[n]=='\xff') tag[n] = 0; 
  }
  
  EFB.begin(
    9600,             // serial baud rate 
    "EFB_JOYSTICK",   // device name
    0x01,             // device version
    tag);             // device tag    
}

void loop() {
  
  int valAxisX1 = map(analogRead(PIN_AXIS_X), 0, 1024, 0, 1000);
  int valAxisY1 = map(analogRead(PIN_AXIS_Y), 0, 1024, 0, 1000);
  boolean valButton1 = (digitalRead(PIN_BUTTON)==LOW ? true : false);

  if (valAxisX != valAxisX1) {
    valAxisX = valAxisX1;
    changed = true;
  }
  if (valAxisY != valAxisY1) {
    valAxisY = valAxisY1;
    changed = true;
  }
  if (valButton != valButton1) {
    valButton = valButton1;
    EFB.sendBinaryData(CH_BUTTON, valButton);
  }
  
  unsigned long now = millis();
  if (changed && now - lastTime > 2) {
    EFB.sendAnalogData(CH_AXIS_X, valAxisX);
    EFB.sendAnalogData(CH_AXIS_Y, valAxisY);
    changed = false;
    lastTime = now;
  }
  
  EFB.read();
}

