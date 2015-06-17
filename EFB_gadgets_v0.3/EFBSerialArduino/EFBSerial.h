#ifndef LIB_EFBSERIAL_H_
#define LIB_EFBSERIAL_H_

#include <inttypes.h>
#include <Arduino.h>

#define EFB_ERROR_UNKNOWN              0
#define EFB_ERROR_UNRECOGNIZED_REQUEST 1
#define EFB_ERROR_INVAILD_CHANNEL      2
#define EFB_ERROR_READ_FAILURE         3
#define EFB_ERROR_WRITE_FAILURE        4

class EFBSerial {

public:
  EFBSerial();
  ~EFBSerial();

  void begin(long baudrate, const char* name, byte version, const char* tag);

public:	
	
  // send data responses to clients
  void sendDeviceInfo();
  void sendAnalogData(uint16_t channel, int value);
  void sendBinaryData(uint16_t channel, boolean value);
  void sendCharacters(uint16_t channel, const char* text, uint16_t length);
  void sendError(uint16_t error);
	
  void send(const char* data, uint16_t length);
				
  bool read();
	
  // set callbacks on receiving requests
  void setHandleChangeDeviceTag(void (*fptr)(const char* tag, uint16_t length));
  
  void setHandleWriteAnalogData(void (*fptr)(uint16_t channel, int value));
  void setHandleReadAnalogData(void (*fptr)(uint16_t channel));
  void setHandleWriteBinaryData(void (*fptr)(uint16_t channel, boolean value));
  void setHandleReadBinaryData(void (*fptr)(uint16_t channel));
  void setHandleWriteCharacters(void (*fptr)(uint16_t channel, const char* text, uint16_t length));
  void setHandleReadCharacters(void (*fptr)(uint16_t channel));
	
private:  

  const char* _devName;
  byte _devVersion;
  char _devTag[16];
  
  char _bufIn[64]; // buffer to receive bytes
  byte _lenIn;     // length of the received bytes
  
  void (*_changeDeviceTagCallback)(const char* tag, uint16_t length);
  
  void (*_writeAnalogDataCallback)(uint16_t channel, int value);
  void (*_readAnalogDataCallback)(uint16_t channel);
  void (*_writeBinaryDataCallback)(uint16_t channel, boolean value);
  void (*_readBinaryDataCallback)(uint16_t channel);
  void (*_writeCharactersCallback)(uint16_t channel, const char* text, uint16_t length);
  void (*_readCharactersCallback)(uint16_t channel);
  
  void parse(char* data, uint16_t length);
  
};

// extern EFBSerial EFB;

#endif // LIB_EFBSERIAL_H_
