#include <SoftwareSerial.h>


#define BIT(data,shift) ((ident&(1<<shift))>>shift)

SoftwareSerial mySerial(2,3);

int LINwrite(byte ident, byte data[], byte data_size);
int LINwriteRequest(byte ident);
int LINreadResponse(char *buffer,byte buffer_size);
int LINread(byte data[], int data_size);
void syncbreak();

int baudrate = 9600;

void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  
  
  //LINwrite(0xA3, package, 4); // Writ
  
  byte package[8] = {22, 25, 0, 1, 0, 2, 0, 3}; 
  byte package_size = 8;
  byte inpackage[8];
  byte inpackage_size = 8;
  byte ident = 0xA3;
  LINwriteRequest(ident); 
  LINreadResponse(inpackage,inpackage_size);
  
  delay(3000);
}






 int LINwrite(byte ident, byte data[], byte data_size){
	// Calculate checksum
	byte suma = 0;
	for(int i=0;i<data_size;i++) 
		suma = suma + data[i];
	//suma = suma + 1;
	byte checksum = 255 - suma;
  byte res[data_size + 1];

    
		mySerial.begin(9600); // config Serial
		mySerial.write(0x55); // write Synch Byte to serial
		mySerial.write(ident); // write Identification Byte to serial
		for(int i=0;i<data_size;i++) mySerial.write(data[i]); // write data to serial
		mySerial.write(checksum); // write Checksum Byte to serial
		mySerial.end(); // clear Serial config
	
	return 1;
}

bool LINread(byte data[], byte data_size){
	byte rec[data_size+1];
  for(int i = 0; i < data_size+1;++i)
  {
    rec[i] = 0;
  }
    
		if(mySerial.available()){ // Check if there is an event on LIN bus
     Serial.println("Read something: ");
			mySerial.readBytes(rec,data_size+1);
			
				for(int j=0;j<data_size;j++){
				data[j] = rec[j];
				}
        for(int i = 0; i < data_size+1;++i)
        {
          int a = rec[i];
          Serial.println(a,DEC);
        }
        return true;
         
		}
    else
    {
     return false;
    }
	
	
}
 

byte calcIdentParity(byte ident)
{
  byte p0 = BIT(ident,0) ^ BIT(ident,1) ^ BIT(ident,2) ^ BIT(ident,4);
  byte p1 = ~(BIT(ident,1) ^ BIT(ident,3) ^ BIT(ident,4) ^ BIT(ident,5));
  return (p0 | (p1<<1))<<6;
}
int LINwriteRequest(byte ident)
{
  // Create Header
	byte identByte = (ident&0x3f) | calcIdentParity(ident);
	byte header[2]= {0x55, identByte};  
  //syncBreak();
  Serial.println("Starting request with ident bytes: ");
  //Serial.println(header[0],DEC);
  //Serial.println(header[1],DEC);
  syncbreak();
	//mySerial.begin(9600); // config Serial
	mySerial.write(header,2); // write data to serial
  //mySerial.end(); // clear Serial config

	
	return 1;
}
int LINreadResponse(byte *buffer,byte buffer_size)
{
  //Serial.println("Checking for response: ");
  //mySerial.begin(9600);
  pinMode(3, INPUT);
  digitalWrite(3, LOW);  // don't pull up

  do
  {

    if(mySerial.available())
    {
      Serial.println("Starting to read response: ");
      mySerial.readBytes(buffer,buffer_size);
       for(int i = 0; i < buffer_size;++i)
        {
          int a = buffer[i];
          Serial.println(a,DEC);
        }
      //Serial.write(buffer,buffer_size);
      //mySerial.end();
       pinMode(3, OUTPUT);
      return 1;
    }
  } while(!mySerial.available());
}

void syncbreak()
{
  mySerial.end();
  unsigned long int brkend = (1000000UL/(unsigned long int)baudrate);
  unsigned long int brkbegin =  brkend*13;
  pinMode(3,OUTPUT);
  digitalWrite(3,LOW);
  if (brkbegin > 16383) delay(brkbegin/1000);  // delayMicroseconds unreliable above 16383 see arduino man pages
  else delayMicroseconds(brkbegin);

  digitalWrite(3,HIGH);
  
  if (brkend > 16383) delay(brkend/1000);  // delayMicroseconds unreliable above 16383 see arduino man pages
  else delayMicroseconds(brkend);

  mySerial.begin(baudrate);
}

