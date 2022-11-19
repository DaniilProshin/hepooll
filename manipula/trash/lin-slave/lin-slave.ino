#include <SoftwareSerial.h>


const byte ident = 0xA3; // Identification Byte
byte data_size=5; // length of byte array
byte data[5]; // byte array for received data
int linrx = 2;
int lintx = 4;
SoftwareSerial mySerial(2,4);

void setup() {
  // put your setup code here, to run once:
  //pinMode(2,INPUT);
  //pinMode(3,OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);

}


int LINread(byte data[], int data_size);
int LINwriteResponse(byte data[],int data_size);

void loop() {

  int bytesToread = LINread(data, data_size); 
  //Serial.print("received bytes: ");
  //Serial.println(bytesToread);
  if(bytesToread == 2)
  {
    Serial.println("Getting request");
    byte package[5] = {1, 2, 3, 4, 5};
    byte package_size = 5;
    LINwriteResponse(package,package_size);
    //Serial.println("Writed response");
  }
  else if(bytesToread >= data_size)
  {
   // Serial.print("Getting message:");
    for(int i = 0; i < data_size;++i)
    {
      Serial.print(data[i],DEC);
      Serial.print("  ");
    }
    Serial.println();
  }
  

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

int LINread(byte data[], byte data_size){
  //pinMode(lintx, INPUT);
  //digitalWrite(lintx, LOW);
	byte rec[data_size+3];
  mySerial.begin(9600);
  for(int i = 0; i < data_size+3;++i)
  {
    rec[i] = 0;
  }
  int bytesToread = mySerial.available();
  
	if(bytesToread == 2){ // Check if there is an event on LIN bus
    Serial.println("Received request package:");
		mySerial.readBytes(rec,data_size+3); 
	}
  else if(bytesToread > 2)
  {
    Serial.println("Received ordinary package:");
		mySerial.readBytes(rec,data_size+3);
		for(int j=0;j<data_size;j++)
    {
			data[j] = rec[j+2];
		}
  }
	// pinMode(lintx, OUTPUT);
	 return bytesToread;
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
		mySerial.write(data, data_size); // write data to serial
		mySerial.write(checksum); // write data to serial
		mySerial.end(); // clear Serial config
    Serial.println("data transmited");
	return 1;
}
