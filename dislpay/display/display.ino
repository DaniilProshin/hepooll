#include <SoftwareSerial.h>



const int linRx = 2;
const int linTx = 3;
const int displayRx = 0;
const int displayTx = 0;

const byte ident = 0xA3; // Identification Byte
int baudrate = 9600;

const byte from_lin_data_size = 5; 
byte from_lin_data[from_lin_data_size]; 
//from lin data structure:
//2 bytes - rpm change values (1 byte: increase 2 byte: decrease)
//1 byte - reverse byte
//1 byte - pause
//1 byte - fault code
const byte from_display_data_size = 5; 
byte from_display_data[from_display_data_size]; 
//from display data structure:
//2 bytes - rpm value
//1 byte - reverse
//1 byte - pause
//1 byte - something
SoftwareSerial lin(linRx,linTx);
SoftwareSerial display(displayRx,displayTx);


int LINread(byte data[], int data_size);
int LINwriteResponse(byte data[],int data_size);

//
int rpmvalue = 400;

void setup() {

  lin.begin(baudrate);
  display.begin(baudrate);
  //from_display_data[1] = rpmvalue & 0xFF;
 //from_display_data[0] = rpmvalue >> 8;
  from_display_data[0] = 0;
  from_display_data[2] = 0;
  from_display_data[3] = 0;
  from_display_data[4] = 0;
}

void loop() {

  int number_of_bytes_readen = LINread(from_lin_data, from_lin_data_size);
  if(number_of_bytes_readen > 3) // receiving package from lin
  {
    display.write(from_lin_data, from_lin_data_size);
    rpmvalue += from_lin_data[0];
    rpmvalue -= from_lin_data[1];
    from_display_data[2] = from_lin_data[2];
    //from_display_data[1] = rpmvalue & 0xFF;
   // from_display_data[0] = rpmvalue >> 8;
    from_display_data[0] = rpmvalue;
  }
  else if(number_of_bytes_readen == 2) // receiving request from lin
  {
    LINwriteResponse(from_display_data,from_display_data_size);
  }
  if(display.available())
  {
    display.readBytes(from_display_data,from_display_data_size);
  }

}

int LINread(byte data[], byte data_size){
	byte rec[data_size+3];
  lin.begin(9600);

  int bytes_avbl = lin.available();
	if(bytes_avbl) // Check if there is an event on LIN bus
  { 
    lin.readBytes(rec,data_size+3);
		for(int j=0;j<data_size;j++)
    {
			data[j] = rec[j+2];
		}
	}
  
	return bytes_avbl;
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
