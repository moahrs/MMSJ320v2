#ifndef   MMSJ_320_MFP_H
#define   MMSJ_320_MFP_H 

#define vmfp ((volatile unsigned char *)0x00400020UL) // MFP MC68901p     A8 to A12 => RS1 to RS5
#define mfpgpdr ((volatile unsigned char *)0x00400021UL)
#define mfpddr ((volatile unsigned char *)0x00400221UL)

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
#define REG_UCR   0x1401U  // 0x29
#define REG_UDR   0x1701U  // 0x2F
#define REG_RSR   0x1501U  // 0x2B
#define REG_TSR   0x1601U  // 0x2D

// MFP MC68901p Interrupt Registers
#define REG_VR    0x0B01U  // 0x17
#define REG_IERA  0x0301U  // 0x07
#define REG_IERB  0x0401U  // 0x09
#define REG_IPRA  0x0501U  // 0x0B
#define REG_IPRB  0x0601U  // 0x0D
#define REG_IMRA  0x0901U  // 0x13
#define REG_IMRB  0x0A01U  // 0x15
#define REG_ISRA  0x0701U  // 0x0F
#define REG_ISRB  0x0801U  // 0x11

// MFP MC68901p Timers Registers
#define REG_TADR  0x0F01U  // 0x1F
#define REG_TBDR  0x1001U  // 0x21
#define REG_TCDR  0x1101U  // 0x23
#define REG_TDDR  0x1201U  // 0x25
#define REG_TACR  0x0C01U  // 0x19
#define REG_TBCR  0x0D01U  // 0x1B
#define REG_TCDCR 0x0E01U  // 0x1D

// MFP MC68901p GPIO Registers
#define REG_GPDR  0x0001U  // 0x01
#define REG_AER   0x0101U  // 0x03
#define REG_DDR   0x0201U  // 0x05

// Backward compatibility aliases
#define Reg_UCR REG_UCR
#define Reg_UDR REG_UDR
#define Reg_RSR REG_RSR
#define Reg_TSR REG_TSR
#define Reg_VR REG_VR
#define Reg_IERA REG_IERA
#define Reg_IERB REG_IERB
#define Reg_IPRA REG_IPRA
#define Reg_IPRB REG_IPRB
#define Reg_IMRA REG_IMRA
#define Reg_IMRB REG_IMRB
#define Reg_ISRA REG_ISRA
#define Reg_ISRB REG_ISRB
#define Reg_TADR REG_TADR
#define Reg_TBDR REG_TBDR
#define Reg_TCDR REG_TCDR
#define Reg_TDDR REG_TDDR
#define Reg_TACR REG_TACR
#define Reg_TBCR REG_TBCR
#define Reg_TCDCR REG_TCDCR
#define Reg_GPDR REG_GPDR
#define Reg_AER REG_AER
#define Reg_DDR REG_DDR
#endif
