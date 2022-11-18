
#include <SoftwareSerial.h>
#include <Arduino.h>
#define BIT(data,shift) ((ident&(1<<shift))>>shift)

class LIN {
  public:
  SoftwareSerial linSerial;
  int txPin;
  int rxPin;
  int bdRate;
  bool serialOn;
  LIN(int rxPin, int txPin):linSerial(SoftwareSerial(rxPin,txPin)){
    this->txPin = txPin;
    this->rxPin = rxPin;
    this->serialOn = 0;
  }
  void begin( int baudRate){
    this->bdRate = baudRate;
    this->linSerial.begin(this->bdRate);
    this->serialOn = 1;
  }

  byte calcIdentParity(byte ident)
  {
    byte p0 = BIT(ident,0) ^ BIT(ident,1) ^ BIT(ident,2) ^ BIT(ident,4);
    byte p1 = ~(BIT(ident,1) ^ BIT(ident,3) ^ BIT(ident,4) ^ BIT(ident,5));
    return (p0 | (p1<<1))<<6;
  }

  int write(byte ident, byte data[], byte data_size){
	// Calculate checksum
	byte suma = 0;
	for(int i=0;i<data_size;i++) 
		suma = suma + data[i];
	byte checksum = 255 - suma;
  this->linSerial.flush();
  serialBreak();
	this->linSerial.write(0x55); // write Synch Byte to serial
	this->linSerial.write(ident); // write Identification Byte to serial
	for(int i=0;i<data_size;i++) this->linSerial.write(data[i]); // write data to serial
	this->linSerial.write(checksum); // write Checksum Byte to serial
	return 1;
  }

  int writeRequest(byte ident)
  {
	  byte identByte = (ident&0x3f) | calcIdentParity(ident);
	  byte header[2]= {0x55, identByte};  
    serialBreak();
	  this->linSerial.write(header,2);
  	return 1;
  }

int readResponse(byte *buffer,byte buffer_size)
{
  pinMode(this->txPin, INPUT);
  digitalWrite(this->txPin, LOW);
  do
  {
    if(this->linSerial.available() >= (buffer_size))
    {
      byte read[buffer_size+2];
      size_t n = this->linSerial.readBytes(read,buffer_size);
      for(int i = 0; i < buffer_size;++i)
      {
        buffer[i] = read[i];
      }
      pinMode(this->txPin, OUTPUT);
      return 1;
    }
  } while(this->linSerial.available() < (buffer_size));

  pinMode(this->txPin, OUTPUT);
  return 0;
}

  void serialBreak(){
    this->linSerial.end();
    this->serialOn = false;
    unsigned long int brkend = (1000000UL/(unsigned long int)this->bdRate);
    unsigned long int brkbegin =  brkend*13;
    pinMode(this->txPin,OUTPUT);
    digitalWrite(this->txPin,LOW);
    if (brkbegin > 16383) delay(brkbegin/1000);  // delayMicroseconds unreliable above 16383 see arduino man pages
    else delayMicroseconds(brkbegin);

    digitalWrite(this->txPin,HIGH);
  
    if (brkend > 16383) delay(brkend/1000);  // delayMicroseconds unreliable above 16383 see arduino man pages
    else delayMicroseconds(brkend);
    this->linSerial.begin(this->bdRate);
    this->serialOn = true;
  }

};