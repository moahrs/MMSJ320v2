#ifndef   MONITOR_H
#define   MONITOR_H

#define __TMS9xxx__
#define __KEYPS2_EXT__
//#define __KEYPS2__
//#define __MOUSEPS2__
#define __MOUSEPS2__EXT__
//#define __MON_SERIAL_KBD__

#if !defined(NULL)
    #define NULL '\0'
#endif

extern unsigned long runMemory;
extern unsigned char kbdKeyPtrR; // Contador do ponteiro das teclas colocadas no buffer
extern unsigned char kbdKeyPtrW; // Contador do ponteiro das teclas colocadas no buffer
extern unsigned char kbdKeyBuffer[66];   // 16 buffer char
extern unsigned char kbdScanCodePtrR; // Contador do ponteiro das teclas colocadas no buffer
extern unsigned char kbdScanCodePtrW; // Contador do ponteiro das teclas colocadas no buffer
extern unsigned char kbdScanCodeBuf[66];   // 16 buffer char
extern unsigned char scanCode;
extern unsigned char vBufReceived; // Byte recebido pelo MFP
extern unsigned char vbuf[128]; // Buffer Linha Digitavel, maximo de 128 caracteres - 
extern unsigned char MseMovPtrR; // Contador do ponteiro das dados do mouse recebidos
extern unsigned char MseMovPtrW; // Contador do ponteiro das dados do mouse recebidos
extern unsigned char MseMovBuffer[66];   // 64 buffer mouse movimentos
extern unsigned long vSizeTotalRec;

#define kbdKeyBuffMax       65

extern void delayms(int pTimeMS);
extern void delayus(int pTimeUS);
extern unsigned char readChar(void);
extern unsigned char inputLine(unsigned int pQtdInput, unsigned char pTipo);
extern void writeSerial(unsigned char pchr);
extern void writeLongSerial(unsigned char *msg);
extern unsigned long lstmGetSize(void);
extern unsigned char loadSerialToMem(unsigned char *pEnder, unsigned char ptipo);
extern void pokeMem(unsigned char *pEnder, unsigned char *pByte);
extern void dumpMem (unsigned char *pEnder, unsigned char *pqtd, unsigned char *pCols);
extern void dumpMem2 (unsigned char *pEnder, unsigned char *pqtd);
extern void dumpMemWin (unsigned char *pEnder, unsigned char *pqtd, unsigned char *pCols);
extern unsigned long hexToLong(char *pHex);
extern unsigned long pow(int val, int pot);
extern int hex2int(char ch);
extern void asctohex(unsigned char a, unsigned char *s);
extern unsigned char readMouse(unsigned char *vStat, unsigned char *vMovX, unsigned char *vMovY);

#ifdef __KEYPS2__
void writeKbdPs2(unsigned char pData);
unsigned char readKbdPs2 (void);
#endif

#ifdef __MOUSEPS2__
void writeMsePs2(unsigned char pData);
unsigned char readMsePs2 (void);
void flushMsePs2 (void);
#endif

#define ALL_OK                0x00
#define FS_CMD      0
#define FS_DATA     1
#define FS_PAR      2

#ifdef __KEYPS2__
    unsigned char convertCode(unsigned char codeToFind,unsigned char *source, unsigned char *destination);
    void processCode(void);
#endif

#endif