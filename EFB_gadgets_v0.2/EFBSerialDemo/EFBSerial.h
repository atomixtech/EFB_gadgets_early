#ifndef LIB_EFBSERIAL_H_
#define LIB_EFBSERIAL_H_

#include <inttypes.h>
#include <Arduino.h>

class EFBSerial {

public:
  EFBSerial();
  ~EFBSerial();

  void begin(long baudrate, const char* name, byte version, const char* tag);

public:	
	
  // send data responses to clients
  void sendDeviceInfo();
  void send8bitData(uint16_t channel, byte value);
  void send1bitData(uint16_t channel, boolean value);
  void sendText(uint16_t channel, const char* text, uint16_t length);
  void sendError(uint16_t error);
	
  void send(const char* data, uint16_t length);
				
  bool read();
	
  // set callbacks on receiving requests
  void setHandleChangeDeviceTag(void (*fptr)(const char* tag, uint16_t length));
  
  void setHandleWrite8bitData(void (*fptr)(uint16_t channel, byte value));
  void setHandleRead8bitData(void (*fptr)(uint16_t channel));
  void setHandleWrite1bitData(void (*fptr)(uint16_t channel, boolean value));
  void setHandleRead1bitData(void (*fptr)(uint16_t channel));
  void setHandleWriteText(void (*fptr)(uint16_t channel, const char* text, uint16_t length));
  void setHandleReadText(void (*fptr)(uint16_t channel));
	
private:  

  const char* _devName;
  byte _devVersion;
  char _devTag[16];
  
  char _bufIn[64]; // buffer to receive bytes
  byte _lenIn;     // length of the received bytes
  
  void (*_changeDeviceTagCallback)(const char* tag, uint16_t length);
  
  void (*_write8bitDataCallback)(uint16_t channel, byte value);
  void (*_read8bitDataCallback)(uint16_t channel);
  void (*_write1bitDataCallback)(uint16_t channel, boolean value);
  void (*_read1bitDataCallback)(uint16_t channel);
  void (*_writeTextCallback)(uint16_t channel, const char* text, uint16_t length);
  void (*_readTextCallback)(uint16_t channel);
  
  void parse(char* data, uint16_t length);
  
};

// extern EFBSerial EFB;

#endif // LIB_EFBSERIAL_H_
