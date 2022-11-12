#include <SoftwareSerial.h>



const int linRx = 2;
const int linTx = 3;
const int displayRx = 0;
const int displayTx = 0;

const byte ident = 0xA3; // Identification Byte

byte lin_input_data_size=8; 
byte lin_input_data[8]; 

byte display_output_data_size=8; 
byte display_output_data[8]; 

SoftwareSerial lin(linRx,linTx);
SoftwareSerial display(displayRx,displayTx);


int LINread(byte data[], int data_size);
int LINwriteResponse(byte data[],int data_size);

void setup() {

  lin.begin(9600);
  display.begin(9600);

}

void loop() {

  if(LINread(data, data_size))
  {
    
    byte package[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    byte package_size = 8;
    LINwriteResponse(package,package_size);

  }
    

}

bool LINread(byte data[], byte data_size){
	byte rec[data_size+3];
  lin.begin(9600);
  for(int i = 0; i < data_size+3;++i)
  {
    rec[i] = 0;
  }
	if(lin.available()) // Check if there is an event on LIN bus
  { 
    lin.readBytes(rec,data_size+3);
		for(int j=0;j<data_size;j++)
    {
			data[j] = rec[j+2];
		}
    return true;
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
	byte checksum = 255 - suma;
	// Start interface
		lin.begin(9600); // config Serial
		lin.write(data, data_size); // write data to serial
		lin.write(checksum); // write data to serial
		lin.end(); // clear Serial config
	return 1;
}
