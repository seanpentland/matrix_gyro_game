#include <Arduino.h>
#include "LEDMatrixLibrary.h"

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

