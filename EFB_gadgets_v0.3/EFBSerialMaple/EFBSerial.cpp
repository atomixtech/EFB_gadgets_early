#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EFBSerial.h"

#define EFB_SERIAL Serial1

// EFBSerial EFB;

EFBSerial::EFBSerial()
{   
  // initialise device specification
  _devName = NULL;
  _devVersion = 0;
  _devTag[0] = 0;

  // clear buffer to receive bytes
  _lenIn = 0; 
  
  // initialise callbacks to NULL pointer
  _writeAnalogDataCallback = NULL;
  _readAnalogDataCallback  = NULL;
  _writeBinaryDataCallback = NULL;
  _readBinaryDataCallback  = NULL;
  _writeCharactersCallback = NULL;
  _readCharactersCallback  = NULL;
}


EFBSerial::~EFBSerial()
{

}

void EFBSerial::begin(long baudrate, const char* name, byte version, const char* tag)
{
  _devName = name;
  _devVersion = version;
  strncpy(_devTag, tag, sizeof(_devTag)-1);

  EFB_SERIAL.begin(baudrate);
}

void EFBSerial::sendDeviceInfo()
{
  char bufOut[128];
  sprintf(bufOut, "{DD#%s#%u#%s}", 
     _devName, _devVersion, _devTag);
  send(bufOut, strlen(bufOut));
}

void EFBSerial::sendAnalogData(uint16_t channel, int value)
{
    char bufOut[16];
    sprintf(bufOut, "{=A#%u#%d}", channel, value);
    send(bufOut, strlen(bufOut));
}

void EFBSerial::sendBinaryData(uint16_t channel, boolean value)
{
    char bufOut[16];
    sprintf(bufOut, "{=B#%u#%d}", channel, (value ? 1 : 0));
    send(bufOut, strlen(bufOut));
}

void EFBSerial::sendCharacters(uint16_t channel, const char* text, uint16_t length)
{
    char bufOut[16];
    sprintf(bufOut, "{=C#%u#", channel);
    send(bufOut, strlen(bufOut));
    send(text, length);
    send("}", 1);
}

void EFBSerial::sendError(uint16_t error)
{
    char bufOut[16];
    sprintf(bufOut, "{ER#%u}", error);
    send(bufOut, strlen(bufOut));
}

void EFBSerial::send(const char* data, uint16_t length)
{
  uint16_t n;
  for (n=0 ; n<length ; n++) EFB_SERIAL.write(data[n]); 
}

bool EFBSerial::read()
{  
  if (EFB_SERIAL.available() > 0) {
    char c = EFB_SERIAL.read();
      
    if (_lenIn >= sizeof(_bufIn)) _lenIn = 0; // buffer is full, then clear the buffer
    _bufIn[_lenIn] = c;
    _lenIn++;
      
    //Serial.println(c);
        
    if (c=='{') {
      // leading character found
      // drop all characters received before
      _bufIn[0] = c;
      _lenIn = 1;  
      
    } else if (c=='}') {
      // ending character found
      // parse received characters
      // and launch callbacks
      parse(_bufIn, _lenIn);
      _lenIn = 0;
      return true;
    }  
  }
  
  return false;
}



void EFBSerial::parse(char* data, uint16_t length)
{
  if (length<3 || data[0]!='{' || data[length-1]!='}') return;
  
  data[0] = 0;
  data[length-1] = 0;
  
  char* p[3]; // pointers to parameters
  int len[3]; // length of parameters
  
  int t = 0;
  p[t] = data+1;
  len[t] = 0;
  for (int n=1 ; n< length-1 ; n++) {
    if (data[n]!='#') {
      len[t]++;
    } else {
      data[n] = 0;
      if (t==2) break; // skip rest if more parameters exist
      t++;
      p[t] = data+n+1;
      len[t] = 0;
    }
  }
  
  if (strcmp(p[0], "DD")==0 && t==0) {
    sendDeviceInfo();
  } else if (strcmp(p[0], "DT")==0 && t==1) {
    strncpy(_devTag, p[1], sizeof(_devTag)-1);
    if (_changeDeviceTagCallback != NULL) _changeDeviceTagCallback(_devTag, strlen(_devTag));
  } else if (strcmp(p[0], "!A")==0 && t==2) {
    if (_writeAnalogDataCallback != NULL) _writeAnalogDataCallback(atoi(p[1]), atoi(p[2]));	
  } else if (strcmp(p[0], "?A")==0 && t==1) {
    if (_readAnalogDataCallback != NULL) _readAnalogDataCallback(atoi(p[1]));
  } else if (strcmp(p[0], "!B")==0 && t==2) {
    if (_writeBinaryDataCallback != NULL) _writeBinaryDataCallback(atoi(p[1]), atoi(p[2]));	
  } else if (strcmp(p[0], "?B")==0 && t==1) {
    if (_readBinaryDataCallback != NULL) _readBinaryDataCallback(atoi(p[1]));	
  } else if (strcmp(p[0], "!C")==0 && t==2) {
    if (_writeCharactersCallback != NULL) _writeCharactersCallback(atoi(p[1]), p[2], len[2]);	
  } else if (strcmp(p[0], "?C")==0 && t==1) {
    if (_readCharactersCallback != NULL) _readCharactersCallback(atoi(p[1]));	
  } else {
    sendError(EFB_ERROR_UNRECOGNIZED_REQUEST);  
  }
  
}

void EFBSerial::setHandleChangeDeviceTag(void (*fptr)(const char* tag, uint16_t length))
{
    _changeDeviceTagCallback = fptr;
}

void EFBSerial::setHandleWriteAnalogData(void (*fptr)(uint16_t channel, int value))
{
    _writeAnalogDataCallback = fptr;
}

void EFBSerial::setHandleReadAnalogData(void (*fptr)(uint16_t channel))
{
    _readAnalogDataCallback = fptr;
}

void EFBSerial::setHandleWriteBinaryData(void (*fptr)(uint16_t channel, boolean value))
{
    _writeBinaryDataCallback = fptr;
}

void EFBSerial::setHandleReadBinaryData(void (*fptr)(uint16_t channel))
{
    _readBinaryDataCallback = fptr;
}

void EFBSerial::setHandleWriteCharacters(void (*fptr)(uint16_t channel, const char* text, uint16_t length))
{
    _writeCharactersCallback = fptr;
}

void EFBSerial::setHandleReadCharacters(void (*fptr)(uint16_t channel))
{
    _readCharactersCallback = fptr;
}
