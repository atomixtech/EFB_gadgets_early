/* EFB Serial Demo v0.2 */

#include <EEPROM.h>
#include "EFBSerial.h"

EFBSerial EFB;

void changeDeviceTagCallback(const char* tag, uint16_t length)
{
  // write device tag to eeprom
  for (int n=0 ; n<length ; n++) {
    EEPROM.write(n, tag[n]);
  }
  EEPROM.write(length, 0);
}

void write8bitDataCallback(uint16_t channel, byte value)
{
  // add your code here to process the request to write the 8bit data
  EFB.send8bitData(channel, value);
}

void read8bitDataCallback(uint16_t channel)
{
  // modify your code here to process the request to read the 8bit data
  byte value = 0xff;
  // send response to master
  EFB.send8bitData(channel, value);
}

void write1bitDataCallback(uint16_t channel, boolean value)
{
  // add your code here to process the request to write the 1bit data
  EFB.send1bitData(channel, value);
}

void read1bitDataCallback(uint16_t channel)
{
  // modify your code here to process the request to read the 1bit data
  boolean value = true;
  // send response to master
  EFB.send1bitData(channel, value);
}

void writeTextCallback(uint16_t channel, const char* text, uint16_t length)
{
  // add your code here to process the request to write the text data
  EFB.sendText(channel, text, length);
}

void readTextCallback(uint16_t channel)
{
  // modify your code here to process the request to read the text data
  char text[] = "MY_TEXT";
  int length = 7;
  // send response to master
  EFB.sendText(channel, text, length);
}


void setup() {
  
  // set callbacks to process requests
  EFB.setHandleChangeDeviceTag(changeDeviceTagCallback);

  EFB.setHandleWrite8bitData(write8bitDataCallback);
  EFB.setHandleRead8bitData(read8bitDataCallback);
  EFB.setHandleWrite1bitData(write1bitDataCallback);
  EFB.setHandleRead1bitData(read1bitDataCallback);
  EFB.setHandleWriteText(writeTextCallback);
  EFB.setHandleReadText(readTextCallback);
  
  // read device tag from eeprom
  char tag[16];
  for (int n=0 ; n<16 ; n++) {
    tag[n] = EEPROM.read(n);
    if (tag[n]=='\xff') tag[n] = 0; 
  }
  
  EFB.begin(
    9600,             // serial baud rate 
    "MY_EFB_DEVICE",  // device name
    0x01,             // device version
    tag);             // device tag
}

void loop() {
  EFB.read();
}

