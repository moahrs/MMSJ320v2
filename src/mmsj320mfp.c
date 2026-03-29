#include "mmsj320mfp.h"

unsigned char *vmfp  = 0x00400020; // MFP MC68901p     A8 to A12 => RS1 to RS5
unsigned char *mfpgpdr = 0x00400021;
unsigned char *mfpddr  = 0x00400221;

// MFP MC68901p Definitions
// ------------------------------
// AAAAAAAAAAAAA
// 1119876543210
// 210
// -------------
// RRRRR00000001
// SSSSS
// 54321
//
// A0 = Always 1 to activate LDS
// A1 - A7 = Always 0
// -----------------------------

// MFP MC68901p USART Registers
unsigned short Reg_UCR   =  0x1401;  // 0x29
unsigned short Reg_UDR   =  0x1701;  // 0x2F
unsigned short Reg_RSR   =  0x1501;  // 0x2B
unsigned short Reg_TSR   =  0x1601;  // 0x2D

// MFP MC68901p Interrupt Registers
unsigned short Reg_VR    =  0x0B01;  // 0x17
unsigned short Reg_IERA  =  0x0301;  // 0x07
unsigned short Reg_IERB  =  0x0401;  // 0x09
unsigned short Reg_IPRA  =  0x0501;  // 0x0B
unsigned short Reg_IPRB  =  0x0601;  // 0x0D
unsigned short Reg_IMRA  =  0x0901;  // 0x13
unsigned short Reg_IMRB  =  0x0A01;  // 0x15
unsigned short Reg_ISRA  =  0x0701;  // 0x0F
unsigned short Reg_ISRB  =  0x0801;  // 0x11

// MFP MC68901p Timers Registers
unsigned short Reg_TADR  =  0x0F01;  // 0x1F
unsigned short Reg_TBDR  =  0x1001;  // 0x21
unsigned short Reg_TCDR  =  0x1101;  // 0x23
unsigned short Reg_TDDR  =  0x1201;  // 0x25
unsigned short Reg_TACR  =  0x0C01;  // 0x19
unsigned short Reg_TBCR  =  0x0D01;  // 0x1B
unsigned short Reg_TCDCR =  0x0E01;  // 0x1D

// MFP MC68901p GPIO Registers
unsigned short Reg_GPDR  =  0x0001;  // 0x01
unsigned short Reg_AER   =  0x0101;  // 0x03
unsigned short Reg_DDR   =  0x0201;  // 0x05
