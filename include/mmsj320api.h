/*------------------------------------------------------------------------------
* MMSJ320API.H - Arquivo de Header do MMSJ320
* Author: Moacir Silveira Junior (moacir.silveira@gmail.com)
* Date: 10/01/2025
*------------------------------------------------------------------------------*/
// Alternate definitions
//typedef void                    VOID;
//typedef char                    CHAR;
//typedef unsigned char           BYTE;                           /* 8-bit unsigned  */
//typedef unsigned short          WORD;                           /* 16-bit unsigned */
//typedef unsigned long           DWORD;                          /* 32-bit unsigned */

// Pointers to I/O devices
unsigned char *vdsk  = 0x00200000; // DISK Arduino Uno, r/w
unsigned char *vdskc = 0x00200001; // DISK Arduino Uno, r/w Command
unsigned char *vdskp = 0x00200005; // DISK Arduino Uno, r/w Command Param
unsigned char *vdskd = 0x00200003; // DISK Arduino Uno, r/w Data
unsigned char *vdest = 0x00000000; // Endereco destino + endre�o base

// Pointer to Variables
// Usando memoria ram de sistema (0x00600000 - 0x0063FFFF) - 256KB

unsigned short *errorBufferAddrBus   = 0x0060F208; // 64 words
                                 // to 0x0060F288
unsigned short *traceData            = 0x0060F28A; // 512 words
                                 // to 0x0060F68A
unsigned long *tracePointer          = 0x0060F68C;
unsigned long *traceA7               = 0x0060F692;
unsigned short *regA7                = 0x0060F696;

/*unsigned char *kbdvprim = 0x00609B3C;
unsigned char *kbdvmove = 0x00609B3E;
unsigned char *kbdvshift = 0x00609B40;
unsigned char *kbdvctrl = 0x00609B42;
unsigned char *kbdvalt = 0x00609B44;
unsigned char *kbdvcaps = 0x00609B46;
unsigned char *kbdvnum = 0x00609B48;
unsigned char *kbdvscr = 0x00609B4A;
unsigned char *kbdvreleased = 0x00609B4C;
unsigned char *kbdve0 = 0x00609B4E;
unsigned long *SysClockms = 0x00609B50;
unsigned char *kbdvreleasedTemp = 0x00609B60;
unsigned char *kbdClockCount   = 0x00609B64;  // Countador do clock para recebimento de tecla PS2

unsigned short *vxmaxold    = 0x00609B68;
unsigned short *vymaxold    = 0x00609B6A;
unsigned short *voverx      = 0x00609B6C; //  Overlay video texto para janelas
unsigned short *vovery      = 0x00609B6E; //  Overlay video texto para janelas
unsigned char *vparamstr   = 0x00609B70; //  255 Char Param string
unsigned short *vparam      = 0x00609C70; //  29 Parameters
unsigned char *vkeyopen    = 0x00609CAC;
unsigned char *vbytetec    = 0x00609CAE;
unsigned short *pposx       = 0x00609CB0;
unsigned short *pposy       = 0x00609CB2;
unsigned short *vbuttonwiny = 0x00609CB6;
unsigned char *vbuttonwin  = 0x00609CB8;
unsigned char  *next_pos   = 0x00609CCE;
//unsigned char  *mcfgfile   = 0x00609FF8; // onde eh carregado o arquivo de configuracao e outros arquivos 12K
unsigned short  *viconef    = 0x00609FF8; // onde eh carregado o arquivo de configuracao e outros arquivos 12K
unsigned short  *vcorf      = 0x0060CFFC; // cor padrao de frente
unsigned short  *vcorb      = 0x0060CFFE; // cor padrao de fundo
unsigned short  *vcol       = 0x0060D000;
unsigned short  *vlin       = 0x0060D002;
unsigned short  *voutput    = 0x0060D004; // 0 - LCD (16x2), 1 - LCD Grafico (320 x 240), 2 - VGA (somente modo texto)
//unsigned char  *vbuf       = 0x0060D006; // Buffer Linha Digitavel OS 32 bytes
unsigned short  *vxmax      = 0x0060D026;
unsigned short  *vymax      = 0x0060D028;
unsigned short  *xpos       = 0x0060D02A;
unsigned short  *ypos       = 0x0060D02C;
unsigned char  *vinip      = 0x0060D0C0; // Inicio da digitacao do prompt
unsigned long *vbufk      = 0x0060D0C2; // Ponteiro para o buffer do teclado (SO FUNCIONA NA RAM)
unsigned char  *vbufkptr   = 0x0060D0C2; // Ponteiro para o buffer do teclado (SO FUNCIONA NA RAM)
unsigned char  *vbufkmove  = 0x0060D0C2;
unsigned char  *vbufkatu   = 0x0060D0C2;
unsigned long *vbufkbios  = 0x0060D0E2; // Interface ponteiros entre BIOS e OS para o teclado
unsigned char  *inten      = 0x0060D0F0; // Usar interrupcoes para comunicacao com o PIC
unsigned short  *vygmax     = 0x0060D0F6;
unsigned long *vmtaskatu  = 0x0060DFA4; // Posicao atual no vmtask do prog sendo executado atualmente:
unsigned char  *vmtask     = 0x0060DFA4; // Lista de 8 posicoes com 6 Bytes cada posicao:
                                //   - Byte 0 a 1 - ordem de execu��o dos programas (2 Bytes)
                                //       - 0xAA: programa sendo usado atualmente, 10mS de tempo de execucao
                                //       - 0xFF: programas que estao em 2o.Plano, 1mS de tempo de execucao
                                //   - Bytes 2 a 5 - ordem dos SP's (A7) dos programas (4 Bytes)
unsigned char  *vmtaskup   = 0x0060DFA4; // Lista de 8 posicoes com 6 Bytes cada posicao (TOPO)
unsigned long *intpos     = 0x0060DFF8; // Posicao da rotina de controle de multitask - OS
unsigned short  *vtotmem    = 0x0060DFFC; // Quantidade de memoria total, vindo da bios
unsigned short  *v10ms      = 0x0060DFFE; // contador 10mS para programas principais na multitarefa
unsigned char  *vPS2                 = 0x0060E000; // Define se tem teclado PS2 (1) ou Touch (0)
unsigned char *vBufXmitEmpty         = 0x0060E002; // Mostra buffer envio serial vazio (1) ou nao (0)
unsigned char *vbuf                  = 0x0060E006; // Buffer Linha Digitavel, maximo de 512 caracteres - 
                                 // to 0x0060E206
unsigned short *startBasic           = 0x0060E69A;

unsigned char *vBufDataDisk = 0x0060E6A0; // 512 bytes
                                 // to 0x0060E89F

unsigned long *kbdtimeout    = 0x0060E8A2;
unsigned long *Msetimeout    = 0x0060E8AA;
unsigned char *MseClockCount = 0x0060E8B2;  // Countador do clock para recebimento de tecla PS/2
unsigned char *vUseMouse     = 0x0060E906;
unsigned char *vStatusMse    = 0x0060E908;
unsigned char *TypeMse       = 0x0060E90A;
unsigned char *kbdPs2Readtype = 0x0060E910;

unsigned char *tempDataMon    = 0x0060F000;*/

/*unsigned char *scanCodeMse   = 0x0060E8B4;
unsigned char *MseMovPntr    = 0x0060E8B6; // Contador do ponteiro das dados do mouse recebidos
unsigned char *MseMovBuffer  = 0x0060E8B8;   // 64 buffer mouse movimentos
unsigned char *Ps2PairChk    = 0x0060E904;*/

// Constantes
#define picSendMsg      0x00D0
#define picCarregaSO    0x00D1
#define picReadKbd      0x00D2
#define picDOScmd       0x00D3
#define picloadFile     0x00D4
#define picSectorRead   0x00D7
#define picSectorWrite  0x00D8

#define picIniIntr      0x00EA
#define picIniStatus    0x00EF

#define picErrorNoSO    0x00FD
#define picErrorIO      0x00FE
#define picOkSO         0x0060

#define picCommData     0x0030
#define picCommStop     0x0040

#define picDOSdir       0x00A0
#define picDOSdel       0x00A1
#define picDOSren       0x00A2
#define picDOSmd        0x00A3
#define picDOScd        0x00A4
#define picDOSrd        0x00A5
#define picDOSformat    0x00A6
#define picDOSdate      0x00A7
#define picDOStime      0x00A8
#define picDOSifconfig  0x00A9
#define picDOScopy      0x00AA

#define kbdMaxCharBuff      8

#define NUMBER_OF_DIGITS 32   /* space for NUMBER_OF_DIGITS + '\0' */

