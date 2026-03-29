#ifndef   MMSJ_320_MFP_H
#define   MMSJ_320_MFP_H 

extern unsigned char *vmfp; // MFP MC68901p     A8 to A12 => RS1 to RS5
extern unsigned char *mfpgpdr;
extern unsigned char *mfpddr;

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
extern unsigned short Reg_UCR ;  // 0x29
extern unsigned short Reg_UDR ;  // 0x2F
extern unsigned short Reg_RSR ;  // 0x2B
extern unsigned short Reg_TSR ;  // 0x2D

// MFP MC68901p Interrupt Registers
extern unsigned short Reg_VR  ;  // 0x17
extern unsigned short Reg_IERA;  // 0x07
extern unsigned short Reg_IERB;  // 0x09
extern unsigned short Reg_IPRA;  // 0x0B
extern unsigned short Reg_IPRB;  // 0x0D
extern unsigned short Reg_IMRA;  // 0x13
extern unsigned short Reg_IMRB;  // 0x15
extern unsigned short Reg_ISRA;  // 0x0F
extern unsigned short Reg_ISRB;  // 0x11

// MFP MC68901p Timers Registers
extern unsigned short Reg_TADR;  // 0x1F
extern unsigned short Reg_TBDR;  // 0x21
extern unsigned short Reg_TCDR;  // 0x23
extern unsigned short Reg_TDDR;  // 0x25
extern unsigned short Reg_TACR;  // 0x19
extern unsigned short Reg_TBCR;  // 0x1B
extern unsigned short Reg_TCDCR;  // 0x1D

// MFP MC68901p GPIO Registers
extern unsigned short Reg_GPDR;  // 0x01
extern unsigned short Reg_AER ;  // 0x03
extern unsigned short Reg_DDR ;  // 0x05
#endif
