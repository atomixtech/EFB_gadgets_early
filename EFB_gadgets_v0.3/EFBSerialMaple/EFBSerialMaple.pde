/* EFB Serial Demo v0.3 */

#include "EFBSerial.h"

EFBSerial EFB;

char deviceTag[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  

void changeDeviceTagCallback(const char* tag, uint16_t length)
{
  // save device tag
  for (int n=0 ; n<length ; n++) {
    deviceTag[n] = tag[n];
  }
  deviceTag[length] = 0;
}

void writeAnalogDataCallback(uint16_t channel, int value)
{
  // add your code here to process the request to write the analog data
  EFB.sendAnalogData(channel, value);
}

void readAnalogDataCallback(uint16_t channel)
{
  // modify your code here to process the request to read the analog data
  int value = 1024;
  // send response to master
  EFB.sendAnalogData(channel, value);
}

void writeBinaryDataCallback(uint16_t channel, boolean value)
{
  // add your code here to process the request to write the binary data
  EFB.sendBinaryData(channel, value);
}

void readBinaryDataCallback(uint16_t channel)
{
  // modify your code here to process the request to read the binary data
  boolean value = true;
  // send response to master
  EFB.sendBinaryData(channel, value);
}

void writeCharactersCallback(uint16_t channel, const char* text, uint16_t length)
{
  // add your code here to process the request to write the text data
  EFB.sendCharacters(channel, text, length);
}

void readCharactersCallback(uint16_t channel)
{
  // modify your code here to process the request to read the text data
  char text[] = "MY_TEXT";
  int length = 7;
  // send response to master
  EFB.sendCharacters(channel, text, length);
}


void setup() {
  
  // set callbacks to process requests
  EFB.setHandleChangeDeviceTag(changeDeviceTagCallback);

  EFB.setHandleWriteAnalogData(writeAnalogDataCallback);
  EFB.setHandleReadAnalogData(readAnalogDataCallback);
  EFB.setHandleWriteBinaryData(writeBinaryDataCallback);
  EFB.setHandleReadBinaryData(readBinaryDataCallback);
  EFB.setHandleWriteCharacters(writeCharactersCallback);
  EFB.setHandleReadCharacters(readCharactersCallback);
  
  EFB.begin(
    9600,             // serial baud rate 
    "MY_EFB_DEVICE",  // device name
    0x01,             // device version
    deviceTag);       // device tag
}

void loop() {
  EFB.read();
}

