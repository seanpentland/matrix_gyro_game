#ifndef LEDMATRIXLIBRARY_H
#define LEDMATRIXLIBRARY_H

#pragma once

#define displayNum1 6
#define displayClk1 7
#define displayNum2 8
#define displayClk2 9

void DisplayStart(int display);
void DisplayEnd(int display);
void DisplaySend(int display, unsigned char send_data);
void DisplaySendCmd(int display, unsigned char send_cmd);
void DisplaySendAddr(int display, unsigned char send_addr);
void DisplayClear(int display);
void DisplayClearBoth();
void DisplayTest(int x, int y);
void DisplayTable();
void ClearTable();
void AddXY(int x, int y);
void RemoveXY(int x, int y);

extern unsigned char nibRev[16];
extern unsigned int xyTable[16];

extern unsigned char dis[];
extern unsigned char disClk[];
extern unsigned char firstAddr;
extern unsigned char fixedAddr; //fixed address
extern unsigned char autoAddr; //address auto +1
extern unsigned char brightness; //lowest brightness setting

#endif
