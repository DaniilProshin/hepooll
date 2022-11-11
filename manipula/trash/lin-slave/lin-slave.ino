#include <SoftwareSerial.h>


const byte ident = 0xA3; // Identification Byte
byte data_size=4; // length of byte array
byte data[4]; // byte array for received data
SoftwareSerial mySerial(2,3);

void setup() {
  // put your setup code here, to run once:
  //pinMode(2,INPUT);
  //pinMode(3,OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  //Serial.println("Start listening");
}


int LINread(byte data[], int data_size);
int LINwriteResponse(byte data[],int data_size);

void loop() {
  // put your main code here, to run repeatedly:
  if(LINread(data, data_size))
  {
    
    byte package[8] = {1, 2, 3, 4, 5, 6, 7, 8};
   byte package_size = 8;
  // byte ident = 0xA3;
   //delay(13);
   
   LINwriteResponse(package,package_size);
  //Serial.println(mySerial.overflow());
  }
    
 // Serial.write(data,data_size);
  /*
  byte package[4] = {31, 255, 0, 0};
  LINwriteResponse(package,4);
  Serial.println("Writed response");
  */
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
	byte rec[data_size+3];
  mySerial.begin(9600);
  for(int i = 0; i < data_size+3;++i)
  {
    rec[i] = 0;
  }
    //Serial.println("Check if there are data");
		if(mySerial.available()){ // Check if there is an event on LIN bus
    // Serial.println("Read something: ");
       Serial.println("Received data");
			mySerial.readBytes(rec,data_size+3);
			
				for(int j=0;j<data_size;j++){
				data[j] = rec[j+2];
				}
        for(int i = 0; i < data_size+3;++i)
        {
          int a = rec[i];
          Serial.println(a,DEC);
        }
        return true;
         mySerial.end();
		}
    else
    {
     return false;
    }
	
	
}

int LINwriteResponse(byte data[],byte data_size)
{
  byte suma = 0;
	for(int i=0;i<data_size;i++) suma = suma + data[i];
	//suma = suma + 1;
	byte checksum = 255 - suma;
	// Start interface
    Serial.println("transmiting data");
		mySerial.begin(9600); // config Serial
    //mySerial.write(0x55); // write Synch Byte to serial
    //byte ident = 0xA3;
    //mySerial.write(ident);
		mySerial.write(data, data_size); // write data to serial
		mySerial.write(checksum); // write data to serial
		mySerial.end(); // clear Serial config
    Serial.println("data transmited");
	return 1;
}
