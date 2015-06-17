#include "EFBSerial.h"

EFBSerial EFB;

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
  EFB.setHandleWrite8bitData(write8bitDataCallback);
  EFB.setHandleRead8bitData(read8bitDataCallback);
  EFB.setHandleWrite1bitData(write1bitDataCallback);
  EFB.setHandleRead1bitData(read1bitDataCallback);
  EFB.setHandleWriteText(writeTextCallback);
  EFB.setHandleReadText(readTextCallback);
  
  byte uid[6] = { 0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6};
  EFB.begin(
    9600,             // serial baud rate 
    "MY_EFB_DEVICE",  // device name
    0x01,             // device version
    uid);             // device uid
}

void loop() {
  EFB.read();
}

