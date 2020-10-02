#include <Arduino.h>
#include "LEDMatrixLibrary.h"

/*---------------------------------
This library was written to address two 8x16 LED Matrix Display Modules each using a TM1640 clone.
https://www.jaycar.com.au/8x16-matrix-led-display-module-for-arduino/p/XC3746

As the modules each have connectors attached to the same side, the most optimal way to connect them is end-to-end.
   ┌──╥──┐ 
  ═╡  ║  ╞═ 
   └──╨──┘
Doing it this way ensures a uniform 16x16 LEDs without any space in the middle, but it also means that one of the
modules needs to be addressed upside-down and back to front.

The Jaycar catalogue claims that these modules use I2C communication, but that is not the case. The TM1640s in the module
are not individually addressable, so addressing two of them needs to be done by bit-banging. 

Data communication is initialised by setting DIN from high to low while CLK is high, which is done in DisplayStart().
Each bit is then sent on CLK low in DisplaySend().
Communication is then ended by setting DIN from low to high on CLK high in DisplayEnd().

-------

The display modules can recieve Data Commands, Display Control Commands and Address Commands. 

The Data Command allows for either fixed address mode or address auto +1 mode, where address in this case refers to
the columns on the LED matrix. For the purpose of this project, only address auto +1 mode will be used. 
Example:
  DisplaySendCmd(0, 0x40); //sets the first display to Auto Address +1 mode

The Display Control Commands control the brightness of the displays, and can turn them on or off. For this project, only
the lowest brightness setting will be used.
Example:
  DisplaySendCmd(0, 0x88); //sets the first display to the lowest brightness setting

Address Commands will set the column in the display that needs to be addressed. Because we are using Auto Address +1,
only the first column will be addressed, followed by the data for each column.
Example:
  DisplaySendCmd(0,0xC0); //sets the address of the first column
  for(int i;i<16;i++){    
    DisplaySend(0, 0xFF); //send 0xFF to display 16 times, which will turn all LEDs on
  }

--------

All the on/off data for the LED Matrices are stored in xyTable[], and sent to both displays using DisplayTable().

----------------------------------*/

unsigned char nibRev[16] =   {0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
                              0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf,};

unsigned int xyTable[16] = {
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};

unsigned char dis[]      = {displayNum1, displayNum2}; 
unsigned char disClk[]   = {displayClk1, displayClk2};
unsigned char firstAddr  = 0xC0;
unsigned char fixedAddr  = 0x44; //fixed address
unsigned char autoAddr   = 0x40; //address auto +1
unsigned char brightness = 0x88; //lowest brightness setting
 

void RemoveXY(int x, int y){
  xyTable[x] &= ~(1<<y);
}

void AddXY(int x, int y){
  xyTable[x] |= (1<<y);
}

/* --- Send contents of xyTable[] to both displays --- */
void DisplayTable(){
  DisplaySendAddr(0, firstAddr);
  for(int i = 0; i < 16; i++){
    DisplaySend(0, (xyTable[i]));
  }
  DisplayEnd(0);
    
  DisplaySendAddr(1, firstAddr);
  for(int i = 15; i >= 0; i--){
    unsigned char temp = (xyTable[i]>>8); //takes the last two bytes of the value in table and assigns to temp
    temp = (nibRev[temp&0b1111] << 4) | nibRev[temp>>4]; //temp is reversed using lookup table
    DisplaySend(1, temp);
  }
  DisplayEnd(1);
}

/* --- Remove all items from table --- */
void ClearTable(){
  for(int i=0;i<16;i++){
    xyTable[i] = 0x0000;
  }
}


/* --- Start Communication with Display --- */
void DisplayStart(int display){
    digitalWrite(disClk[display], LOW);
    digitalWrite(dis[display], HIGH);
    digitalWrite(disClk[display], HIGH);
    digitalWrite(dis[display], LOW);
}

/* --- Send Data to Display --- */
void DisplaySend(int display, unsigned char send_data){
  for (char i = 0; i < 8; i++){
    digitalWrite(disClk[display], LOW);
    if (send_data & 0x01){
      digitalWrite(dis[display], HIGH);
    }else{
      digitalWrite(dis[display], LOW);
    }
    digitalWrite(disClk[display], HIGH);
    send_data = send_data >> 1;
  }
}

/* --- Send Command (Includes Start and Send) --- */
void DisplaySendCmd(int display, unsigned char send_cmd){
  DisplayStart(display);
  DisplaySend(display, send_cmd);
  DisplayEnd(display);
}

/* --- Send Address (Includes Start only) --- */
void DisplaySendAddr(int display, unsigned char send_addr){
  DisplayStart(display);
  DisplaySend(display, send_addr);
}

/* --- End Communnication with display --- */
void DisplayEnd(int display){
  digitalWrite(disClk[display], LOW);
  digitalWrite(dis[display], LOW);
  digitalWrite(disClk[display], HIGH);
  digitalWrite(dis[display], HIGH);
}

/* --- Clear the display --- */
void DisplayClear(int display){
  DisplaySendCmd(display,autoAddr);  
  DisplaySendAddr(display, firstAddr);
  for(int i = 0; i < 16; i++){
    DisplaySend(display, 0x00);
  }
  DisplayEnd(display);
  //DisplaySendCmd(display,fixedAddr);
}

/* --- Clear both displays --- */
void DisplayClearBoth(){
  DisplayClear(0);
  DisplayClear(1);
}

