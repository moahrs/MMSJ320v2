/********************************************************************************
*    Programa    : monitor.c
*    Objetivo    : BIOS do modulo MMSJ300 - Versao vintage compatible
*    Criado em   : 17/09/2022
*    Programador : Moacir Jr.
*--------------------------------------------------------------------------------
* Data        Versao  Responsavel  Motivo
* 17/09/2022  0.1     Moacir Jr.   Criacao Versao Beta
*                                  512KB EEPROM + 256KB RAM BUFFER + 8MB RAM USU
* 12/11/2023  0.2     Moacir Jr.   Adaptacao do MC68901 p/ serial e interrupcoes
* 01/06/2023  0.3     Moacir Jr.   Adaptacao do teclado PS/2 via arduino nano
*                                  Simulando um FPGA de epoca
* 22/06/2023  0.4     Moacir Jr.   Adaptacao do TMS9118 VDP
* 23/06/2023  0.4a    Moacir Jr.   Adaptacao lib arduino tms9918 pro mmsj300
* 15/07/2023  0.4b    Moacir Jr.   Colocar tela vermelha de erros
* 15/07/2023  0.4c    Moacir Jr.   Colocar rotina de trace
* 18/07/2023  0.4d    Moacir Jr.   Verificar e Ajustar Problema no G2 do VDP
* 19/07/2023  1.0     Moacir Jr.   Versao para publicacao
* 20/07/2023  1.0a    Moacir Jr.   Ajuste de bugs
* 21/07/2023  1.1     Moacir Jr.   Adaptar Basic ao monitor
* 25/07/2023  1.1a    Moacir Jr.   Ajustes no inputLine, aceitar tipo '@'
* 20/01/2024  1.1b    Moacir Jr.   Iniciar direto no basic... Ai com QUIT, volta pro monitor
* 06/03/2024  1.1b    Moacir Jr.   Colocar dumpw (dump em forma de janela texto)
* 06/03/2024  1.1c    Moacir Jr.   Ajuste write char no modo grafico G2
* 09/03/2024  1.1d    Moacir Jr.   Adaptar floppy disk via arduino como controlador (FAT será aqui)
*                                  ***** A B O R T A D O *****
* 17/11/2024  1.1e    Moacir Jr.   Colocar o PS/2 direto no MFP->IRQ->CPU
* 25/12/2024  1.1f    Moacir Jr.   Ajustes e colocar tamanho total recebido
*                                  no carregamento seria pra memoria
* 10/01/2025  1.2     Moacir Jr.   Integrar com uC/CosII para o MMSJ320
* 16/01/2025  1.3     Moacir Jr.   Voltar para teclado e mouse com arduino nano
* 28/01/2025  1.3a    Moacir Jr.   Ajustes na leitura dos dados recebidos do Mouse
* 28/03/2026  1.4     Moacir Jr.   Troca de compilador, do ide68k para m68k-elf-gcc
*--------------------------------------------------------------------------------
*
* Mapa de Memoria
* ---------------
*
*     SLOT 0                          SLOT 1
* +-------------+ 000000h
* |   EEPROM    |
* |   512KB     |
* |   (BIOS)    | 07FFFFh
* +-------------+ 080000h
* |    LIVRE    | 1FFFFFh
* +-------------+ 200000h
* |             |
* |  EXPANSAO   |
* |             | 3FFFFFh
* +-------------+ 400000h
* |             |
* | PERIFERICOS |
* |             | 5FFFFFh
* +-------------+ 600000h
* |  RAM 256KB  |
* |  BUFFER E   |
* |  SISTEMA    | 63FFFFh
* +-------------+ 640000h
* |    LIVRE    | 7FFFFFh
* +-------------+ 800000h
* |             |
* |   ATUAL     |
* |    RAM      |
* |  USUARIO    |
* |    1MB      | 8FFFFFh
* +-------------+ 900000h
* |             |
* |             |
* |    RAM      |
* |  USUARIO    |
* |    7MB      |
* |             |
* |             |
* |             |
* |             |
* |             |
* |             |
* |             |
* |             |
* +-------------+ FFFFFFh
*--------------------------------------------------------------------------------
*
* Enderecos de Perifericos
*
* 00200001h e 00200003 - DISK Arduino UNO (Temp)
*                        - A1 = 0: r/w 4 bits LSB
*                        - A1 = 1: r/w 4 bits MSB
* 00400020h a 0040003F - MFP MC68901p - Cristal de 2.4576MHz
*                        - SERIAL 9600, 8, 1, n
*                        - TECLADO (PC-AT - PS/2)
*                        - Controle de Interrupcoes e PS/2
* 00400040h a 00400043 - VIDEO TMS9118 (16KB VRAM):
*             00400041 - Data Mode
*             00400043 - Register / Adress Mode
********************************************************************************/
#define VDP_EXT extern
#define MFP_EXT extern

#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "mmsj320api.h"
#include "mmsj320vdp.h"
#include "mmsj320mfp.h"
#include "monitor.h"

#define versionBios "1.4"

void *__allocp;

static unsigned char upper_ascii(unsigned char ch)
{
    if (ch >= 'a' && ch <= 'z')
        return (unsigned char)(ch - ('a' - 'A'));
    return ch;
}

unsigned long runMemory;
unsigned char kbdKeyPtrR; // Contador do ponteiro das teclas colocadas no buffer
unsigned char kbdKeyPtrW; // Contador do ponteiro das teclas colocadas no buffer
unsigned char kbdKeyBuffer[66];   // 16 buffer char

#ifdef __KEYPS2__
    unsigned char kbdScanCodePtrR; // Contador do ponteiro das teclas colocadas no buffer
    unsigned char kbdScanCodePtrW; // Contador do ponteiro das teclas colocadas no buffer
    unsigned char kbdScanCodeBuf[66];   // 16 buffer char
#endif
unsigned char scanCode;
unsigned char vBufReceived; // Byte recebido pelo MFP
unsigned char vbuf[128]; // Buffer Linha Digitavel, maximo de 128 caracteres -
unsigned char MseMovPtrR; // Contador do ponteiro das dados do mouse recebidos
unsigned char MseMovPtrW; // Contador do ponteiro das dados do mouse recebidos
unsigned char MseMovBuffer[66];   // 64 buffer mouse movimentos
unsigned long vSizeTotalRec;

unsigned char vBufXmitEmpty;
unsigned char vtotmem;
unsigned long SysClockms;
unsigned short startBasic;
unsigned char debugMessages;

void delayms(int pTimeMS);
void delayus(int pTimeUS);
unsigned char readChar(void);
unsigned char inputLine(unsigned int pQtdInput, unsigned char pTipo);
int processCmd(void);
void writeSerial(unsigned char pchr);
void writeLongSerial(unsigned char *msg);
unsigned long lstmGetSize(void);
unsigned char loadSerialToMem(unsigned char *pEnder, unsigned char ptipo);
void runMem(unsigned long pEnder);
void runBasic(unsigned long pEnder);
void pokeMem(unsigned char *pEnder, unsigned char *pByte);
void dumpMem (unsigned char *pEnder, unsigned char *pqtd, unsigned char *pCols);
void dumpMem2 (unsigned char *pEnder, unsigned char *pqtd);
void dumpMemWin (unsigned char *pEnder, unsigned char *pqtd, unsigned char *pCols);
void diskCmd (unsigned char *pCmd, unsigned char *pParam);
void basicFuncBios(void);
unsigned long hexToLong(char *pHex);
unsigned long pow(int val, int pot);
int hex2int(char ch);
void asctohex(unsigned char a, unsigned char *s);
void runCmd(void);
void runBas(void);
void runOSCmd(void);

void runSO(void);
unsigned int carregaSO(void);
void carregaOSDisk(void);
void runSystemOper(void);

void hideCursor(void);
void showCursor(void);
void modeVideo(unsigned char *pMode);
void printCharBuffer(unsigned char *pCharMade);
unsigned char readMouse(unsigned char *vStat, unsigned char *vMovX, unsigned char *vMovY);

void inputTask(void);

#ifdef __KEYPS2__
    void scanCodeTask(void *);
#endif

// ASCII character set
unsigned char ascii[]  = "abcdefghijklmnopqrstuvwxyz0123456789;=.,/'[]`- "; // Sem Caps sem Shift
unsigned char ascii2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ)!@#$%^&*(:+><?\"{}~_ "; // Sem Caps com Shift
unsigned char ascii3[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789;=.,/'[]`- "; // Com Caps sem Shift
unsigned char ascii4[] = "abcdefghijklmnopqrstuvwxyz)!@#$%^&*(:+><?\"{}~_ "; // Com Caps com Shift

// KeyCode set

// Querty keycode set: uncomment to activate this code
unsigned char keyCode[]={0x1C,0x32,0x21,0x23,0x24,0x2B,0x34,0x33,0x43,0x3B,0x42,
                         0x4B,0x3A,0x31,0x44,0x4D,0x15,0x2D,0x1B,0x2C,0x3C,0x2A,
                         0x1D,0x22,0x35,0x1A,0x45,0x16,0x1E,0x26,0x25,0x2E,0x36,
                         0x3D,0x3E,0x46,0x4C,0x55,0x49,0x41,0x4A,0x52,0x54,0x5B,
                         0x0E,0x4E,0x29,0x00};

//-----------------------------------------------------------------------------
// Principal
//-----------------------------------------------------------------------------
void main(void)
{
    unsigned short *xaddr = (unsigned short *) 0x00600000;
    unsigned short vbytepic = 0, xdado;
    volatile unsigned int ix = 0;
    unsigned int xcounter = 0;
    unsigned int free_kbytes = 0;
    unsigned char sqtdtam[20];
    unsigned char vRamSyst1st = 1, vRamUser1st = 1;

    // Inicia com Basic
    startBasic = 0;
    debugMessages = 0;

    // Tempo para Inicializar a Memoria DRAM (se tiver), Perifericos e etc...
    for(ix = 0; ix <= 12000; ix++);

    //---------------------------------------------
    // Enviar setup para o MFP 68901
    //---------------------------------------------
    vBufXmitEmpty = 1;

    // Setup Timers
    *(vmfp + REG_TACR)  = 0x10; // Stop Counter Timer A
    *(vmfp + REG_TBCR)  = 0x10; // Stop Counter Timer A

    while(*(vmfp + REG_TADR) != 0x9A)
        *(vmfp + REG_TADR)  = 0x9A; // Valor para 1 ms

    *(vmfp + REG_TACR)  = 0x13; // Start Counter Timer A Com Delay por 16

    while(*(vmfp + REG_TBDR) != 0xF6)
        *(vmfp + REG_TBDR)  = 0xF6; // Valor para 10 ms

    *(vmfp + REG_TBCR)  = 0x16; // Start Counter Timer B Com Delay por 100

    *(vmfp + REG_TCDR)  = 0x02;
    *(vmfp + REG_TDDR)  = 0x02;
    *(vmfp + REG_TCDCR) = 0x11;

    // Setup Interruptions
    *(vmfp + REG_VR)    = 0xA0; // vector = 0xA msb = 0x1010 and lsb = 0x0000 auto end session interrupt ///// antigo = lsb = 0x1000 software end session interrupt
    *(vmfp + REG_IERA)  = 0x00; // disable all at start
    *(vmfp + REG_IERB)  = 0x00; // disable all at start
    *(vmfp + REG_IMRA)  = 0x00; // disable all at start
    *(vmfp + REG_IMRB)  = 0x00; // disable all at start
    *(vmfp + REG_ISRA)  = 0x00; // disable all at start
    *(vmfp + REG_ISRB)  = 0x00; // disable all at start

    // Setup Serial = 9600, 8, 1, n
    *(vmfp + REG_UCR)   = 0x88;
    *(vmfp + REG_RSR)   = 0x01;
    *(vmfp + REG_TSR)   = 0x21;

    // Setup GPIO
    #ifdef __KEYPS2__
        *(vmfp + REG_DDR)   = 0x10; // I4 as Output, I7 - I5 e I3 - I0 as Input
    #endif

    #ifdef __KEYPS2_EXT__
        *(vmfp + REG_DDR)   = 0x10; // I4 as Output, I7 - I5 e I3 - I0 as Input
    #endif

    *(vmfp + REG_AER)   = 0x00; // All Interrupts transction 1 to 0

    // Setup Interruptions
    *(vmfp + REG_IERA)  = 0x00; // 0xCE; // serial interrupt (buffer full and empty) i7 = Clock PS2 Mouse, I6 = Clock PS2 KeyBoard (clk pin OR DTRDY pin)
    *(vmfp + REG_IERB)  = 0x00;
    *(vmfp + REG_IMRA)  = 0x00; // 0xCE; // serial interrupt (buffer full and empty) i7 = Clock PS2 Mouse, I6 = Clock PS2 KeyBoard (clk pin OR DTRDY pin)
    *(vmfp + REG_IMRB)  = 0x00;
    //---------------------------------------------

    #ifdef __KEYPS2_EXT__
        *(vmfp + REG_GPDR) |= 0x10;  // Seta CS = 1 (I4) do controlador
    #endif

    //---------------------------------------------
    // Enviar setup para o VDP TMS9118
    //---------------------------------------------
    // Definindo variaveis de video
    videoCursorPosColX = 0;
    videoCursorPosRowY = 0;
    videoScroll = 1;       // Ativo
    videoScrollDir = 1;    // Pra Cima
    videoCursorBlink = 0;
    videoCursorShow = 0;
    vdpMaxCols = 39;
    vdpMaxRows = 23;

    vdp_init_textmode(VDP_WHITE, VDP_BLACK);
    //---------------------------------------------

    // Zera Tudo (se tiver o que zerar), sem verificar, antes de testar
    xaddr = 0x00600000;
    while (xaddr <= 0x00FFFFFE)
    {
        *xaddr = 0x0000;
        xaddr += 32768;
    }

    // Testando memoria RAM de 64 em 64K Word pra saber quando tem
    xaddr = 0x00600000;
    xcounter = 0;
    while (xaddr <= 0x00FFFFFE) {
        // Se ja passou por esse endereco, cai fora - (caso de usar memoria de sistema como principal)
        xdado = *xaddr;

        if (xaddr < 0x00800000 && xdado == 0x5A4C && !vRamSyst1st)
        {
            xaddr = 0x00800000;
            continue;
        }
        else
        {
            if (xaddr >= 0x00800000 && xdado == 0x5A4C && !vRamUser1st)
                break;
        }

        // Testa Gravacao de 0000h
        *xaddr = 0x0000;
        for(ix = 0; ix <= 100; ix++);
        xdado = *xaddr;
        if (xdado != 0x0000)
        {
            if (xaddr < 0x00800000)
            {
                xaddr = 0x00800000;
                continue;
            }

            break;
        }

        // Testa Gravacao de FFFFh
        *xaddr = 0xFFFF;
        for(ix = 0; ix <= 100; ix++);
        xdado = *xaddr;
        if (xdado != 0xFFFF)
        {
            if (xaddr < 0x00800000)
            {
                xaddr = 0x00800000;
                continue;
            }

            break;
        }

        // Se tudo ok, deixa gravado 0x5A4C para nao ler novamente - (caso de usar memoria de sistema como principal)
        *xaddr = 0x5A4C;

        if (xaddr < 0x00800000)
            vRamSyst1st = 0;
        else
            vRamUser1st = 0;

        xcounter += 64; // dobrar a soma para aparecer em bytes e nao em words

        // Limite maximo de contagem, 8MB
        if (xcounter >= 8448)
            break;

        xaddr += 32768;
    }

    vtotmem = xcounter;

    clearScr();
    printText("MMSJ-320 BIOS v"versionBios);
    printText("\r\n\0");

    printText("Utility (c) 2014-2026\r\n\0");

    itoa(xcounter, sqtdtam, 10);
    printText(sqtdtam);
    printText("K Bytes Found. ");
    if (xcounter > 256)
        free_kbytes = xcounter - 256;
    else
        free_kbytes = 0;
    itoa(free_kbytes, sqtdtam, 10);
    printText(sqtdtam);
    printText("K Bytes Free.\r\n\0");

    if (!startBasic)
    {
        printText("OK\r\n\0");
        printText(">");
    }

    showCursor();

    vBufReceived = 0x00;
    vbuf[0] = '\0';
    SysClockms = 0;

    #if defined(__KEYPS2__) || defined(__KEYPS2_EXT__)
/*        *kbdvprim = 1;
        *kbdvshift = 0;
        *kbdvctrl = 0;
        *kbdvalt = 0;
        *kbdvcaps = 0;
        *kbdvnum = 0;
        *kbdvscr = 0;
        *kbdvreleased = 0x00;
        *kbdve0 = 0;
        *kbdClockCount = 0;*/
        kbdKeyBuffer[0] = 0x00;
        scanCode = 0;
        kbdKeyPtrR = 0;
        kbdKeyPtrW = 0;
        #ifdef __KEYPS2__
            kbdScanCodePtrR = 0;
            kbdScanCodePtrW = 0;
            kbdScanCodeBuf[0] = 0x00;
        #endif
/*        *kbdtimeout = 0;
        *kbdPs2Readtype = 1;*/
        MseMovPtrR = 0;
        MseMovPtrW = 0;
        MseMovBuffer[0] = 0x00;

        // Ativando Interrupcao do Kbd/Mse PS/2
        *(vmfp + REG_IERA) = 0xC0; // GPI6 and 7 will be KBD/MSE PS2 interrupt (clk pin OR DTRDYK/M pin)
        *(vmfp + REG_IMRA) = 0xC0; // GPI6 and 7 will be KBD/MSE PS2 interrupt (clk pin OR DTRDYK/M pin)
    #endif

    #ifdef __MOUSEPS2__
        *MseMovPntr = 0;
        *MseMovBuffer = 0;
        *MseClockCount = 0;
        *Msetimeout = 0;
        scanCodeMse = 0xFF;
        *vUseMouse = 0;

        ix = 0;
        do
        {
            // Send Reset
            writeMsePs2(0xFF);

            // Read 3 bytes Response
            *MseMovBuffer = readMsePs2();
            *(MseMovBuffer + 1) = readMsePs2();
            *(MseMovBuffer + 2) = readMsePs2();

            ix++;
        } while (*MseMovBuffer == 0xFE && ix++ < 4);

        if (*MseMovBuffer == 0xFA && *(MseMovBuffer + 1) == 0xAA)
        {
            flushMsePs2();

            // Send To know the ID
            writeMsePs2(0xF2);

            // Read 1 byte response
            *(MseMovBuffer + 4) = readMsePs2();
            *TypeMse = readMsePs2();

            flushMsePs2();

            ix = 3;
            do
            {
                if (ix > 3)
                    delayus(100);

                // Send Enable
                writeMsePs2(0xF4);

                // Read 1 byte response
                *(MseMovBuffer + 3) = readMsePs2();
                ix++;
            } while (*(MseMovBuffer + 3) == 0xFE && ix < 7);

            if (*(MseMovBuffer + 3) == 0xFA)
                *vUseMouse = 0x01;
        }
    #endif

    inputTask();
}

//-----------------------------------------------------------------------------
void inputTask(void)
{
    unsigned char vtec, vtecant = 0;
    int vRetProcCmd, countCursor = 0;
    unsigned char vbufptr = 0;

    while (1)
    {
        // Piscar Cursor
        if (debugMessages) //(videoCursorBlink)
        {
            switch (countCursor)
            {
                case 6000:  //20
                    hideCursor();
                    break;
                case 12000: //40
                    showCursor();
                    countCursor = 0;
                    break;
            }
            countCursor++;
        }

        vtec = readChar();

        if (vtec)
        {
            hideCursor();

            if (vtec >= 0x20 && vtec != 0x7F)   // Caracter Printavel menos o DeLete
            {
                // Digitcao Normal
                if (vbufptr > 127)
                {
                    vbufptr--;

                    printChar(0x08, 1);
                }

                printChar(vtec, 1);

                vbuf[vbufptr++] = vtec;
                vbuf[vbufptr] = '\0';
            }
            else if (vtec == 0x08)  // Backspace
            {
                if (vbufptr > 0)
                {
                    vbuf[vbufptr] = 0x00;
                    vbufptr--;

                    printChar(0x08, 1);
                }
            }
            else if (vtec == 0x0D || vtec == 0x0A)
            {
                vRetProcCmd = 1;

                printText("\r\n\0");

                vRetProcCmd = processCmd();

                vBufReceived = 0x00;
                vbuf[0] = '\0';
                vbufptr = 0x00;

                if (vRetProcCmd)
                    printText("\r\n\0");

                printChar('>', 1);
            }

            showCursor();
        }

        vtecant = vtec;
    }
}

//-----------------------------------------------------------------------------
#ifdef __KEYPS2__
void scanCodeTask(void *pdata)
{
    unsigned int error_code = OS_ERR_NONE;

    while (1)
    {
        if (kbdScanCodePtrR != kbdScanCodePtrW)
        {
            // Pega proximo Scan Code disponivel
            scanCode = kbdScanCodeBuf[kbdScanCodePtrR];

            // Processa Codigo
            processCode();

            // Adiciona 1 no ponteiro de leitura do buffer circular
            kbdScanCodePtrR++;

            // Se chegar em 16, volta pra 0
            if (kbdScanCodePtrR > kbdKeyBuffMax)
                kbdScanCodePtrR = 0;
        }
    }
}
#endif

//-----------------------------------------------------------------------------
// pQtdInput - Quantidade a ser digitada, min 1 max 255
// pTipo - Tipo de entrada:
//                  input : $ - String, % - Inteiro (sem ponto), # - Real (com ponto), @ - Sem Cursor e Qualquer Coisa e sem enter
//                   edit : S - String, I - Inteiro (sem ponto), R - Real (com ponto)
//-----------------------------------------------------------------------------
unsigned char inputLine(unsigned int pQtdInput, unsigned char pTipo)
{
    unsigned char *vbufptr = vbuf;
    unsigned char vtec, vtecant;
    int vRetProcCmd, iw, ix;
    int countCursor = 0;
    char pEdit = 0, pIns = 0, vbuftemp, vbuftemp2;
    int iPos, iz;
    unsigned short vantX, vantY;

    if (pQtdInput == 0)
        pQtdInput = 512;

    vtecant = 0x00;
    vbufptr = vbuf;

    // Se for Linha editavel apresenta a linha na tela
    if (pTipo == 'S' || pTipo == 'I' || pTipo == 'R')
    {
        // Apresenta a linha na tela, e posiciona o cursor na tela na primeira posicao valida
        iw = strlen(vbuf) / 40;

        printText(vbuf);

        videoCursorPosRowY -= iw;
        videoCursorPosColX = 0;
        pEdit = 1;
        iPos = 0;
        pIns = 0xFF;

        vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY);
    }

    if (pTipo != '@')
        showCursor();

    while (1)
    {
        // Piscar Cursor
        if (videoCursorBlink && pTipo != '@')
        {
            switch (countCursor)
            {
                case 6000:
                    hideCursor();
                    if (pEdit)
                        printChar(vbuf[iPos],0);
                    break;
                case 12000:
                    showCursor();
                    countCursor = 0;
                    break;
            }
            countCursor++;
        }

        // Inicia leitura
        vtec = readChar();

        if (pTipo == '@')
            return vtec;

        // Se nao for string ($ e S) ou Tudo (@), só aceita numeros
        if (pTipo != '$' && pTipo != 'S' && pTipo != '@' && vtec != '.' && vtec > 0x1F && (vtec < 0x30 || vtec > 0x39))
            vtec = 0;

        // So aceita ponto de for numero real (# ou R) ou string ($ ou S) ou tudo (@)
        if (vtec == '.' && pTipo != '#' && pTipo != '$' &&  pTipo != 'R' && pTipo != 'S' && pTipo != '@')
            vtec = 0;

        if (vtec)
        {
            // Prevenir sujeira no buffer ou repeticao
            if (vtec == vtecant)
            {
                if (countCursor % 300 != 0)
                    continue;
            }

            if (pTipo != '@')
            {
                hideCursor();

                if (pEdit)
                    printChar(vbuf[iPos],0);
            }

            vtecant = vtec;

            if (vtec >= 0x20 && vtec != 0x7F)   // Caracter Printavel menos o DELete
            {
                if (!pEdit)
                {
                    // Digitcao Normal
                    if (vbufptr > vbuf + pQtdInput)
                    {
                        *vbufptr--;

                        if (pTipo != '@')
                            printChar(0x08, 1);
                    }

                    if (pTipo != '@')
                        printChar(vtec, 1);

                    *vbufptr++ = vtec;
                    *vbufptr = '\0';
                }
                else
                {
                    iw = strlen(vbuf);

                    // Edicao de Linha
                    if (!pIns)
                    {
                        // Sem insercao de caracteres
                        if (iw < pQtdInput)
                        {
                            if (vbuf[iPos] == 0x00)
                                vbuf[iPos + 1] = 0x00;

                            vbuf[iPos] = vtec;

                            printChar(vbuf[iPos],0);
                        }
                    }
                    else
                    {
                        // Com insercao de caracteres
                        if ((iw + 1) <= pQtdInput)
                        {
                            // Copia todos os caracteres mais 1 pro final
                            vbuftemp2 = vbuf[iPos];
                            vbuftemp = vbuf[iPos + 1];
                            vantX = videoCursorPosColX;
                            vantY = videoCursorPosRowY;

                            printChar(vtec,1);

                            for (ix = iPos; ix <= iw ; ix++)
                            {
                                vbuf[ix + 1] = vbuftemp2;
                                vbuftemp2 = vbuftemp;
                                vbuftemp = vbuf[ix + 2];
                                printChar(vbuf[ix + 1],1);
                            }

                            vbuf[iw + 1] = 0x00;
                            vbuf[iPos] = vtec;

                            videoCursorPosColX = vantX;
                            videoCursorPosRowY = vantY;
                            vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY);
                        }
                    }

                    if (iw <= pQtdInput)
                    {
                        iPos++;
                        videoCursorPosColX = videoCursorPosColX + 1;
                        vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY);
                    }
                }
            }
            /*else if (pEdit && vtec == 0x11)    // UpArrow (17)
            {
                // TBD
            }
            else if (pEdit && vtec == 0x13)    // DownArrow (19)
            {
                // TBD
            }*/
            else if (pEdit && vtec == 0x12)    // LeftArrow (18)
            {
                if (iPos > 0)
                {
                    printChar(vbuf[iPos],0);
                    iPos--;
                    if (videoCursorPosColX == 0)
                        videoCursorPosColX = 255;
                    else
                        videoCursorPosColX = videoCursorPosColX - 1;
                    vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY);
                }
            }
            else if (pEdit && vtec == 0x14)    // RightArrow (20)
            {
                if (iPos < strlen(vbuf))
                {
                    printChar(vbuf[iPos],0);
                    iPos++;
                    videoCursorPosColX = videoCursorPosColX + 1;
                    vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY);
                }
            }
            else if (vtec == 0x15)  // Insert
            {
                pIns = ~pIns;
            }
            else if (vtec == 0x08 && !pEdit)  // Backspace
            {
                // Digitcao Normal
                if (vbufptr > vbuf)
                {
                    *vbufptr--;
                    *vbufptr = 0x00;

                    if (pTipo != '@')
                        printChar(0x08, 1);
                }
            }
            else if ((vtec == 0x08 || vtec == 0x7F) && pEdit)  // Backspace
            {
                iw = strlen(vbuf);

                if ((vtec == 0x08 && iPos > 0) || vtec == 0x7F)
                {
                    if (vtec == 0x08)
                    {
                        iPos--;

                        if (videoCursorPosColX == 0)
                            videoCursorPosColX = 255;
                        else
                            videoCursorPosColX = videoCursorPosColX - 1;
                        vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY);
                    }

                    vantX = videoCursorPosColX;
                    vantY = videoCursorPosRowY;

                    for (ix = iPos; ix < iw ; ix++)
                    {
                        vbuf[ix] = vbuf[ix + 1];
                        printChar(vbuf[ix],1);
                    }

                    vbuf[ix] = 0x00;

                    videoCursorPosColX = vantX;
                    videoCursorPosRowY = vantY;
                    vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY);
                }
            }
            else if (vtec == 0x1B)   // ESC
            {
                // Limpa a linha, esvazia o buffer e retorna tecla
                while (vbufptr > vbuf)
                {
                    *vbufptr--;
                    *vbufptr = 0x00;

                    if (pTipo != '@')
                        hideCursor();

                    if (pTipo != '@')
                        printChar(0x08, 1);

                    if (pTipo != '@')
                        showCursor();
                }
                hideCursor();

                return vtec;
            }
            else if (vtec == 0x0D || vtec == 0x0A ) // CR ou LF
            {
                return vtec;
            }

            if (pTipo != '@')
                showCursor();
        }
        else
        {
            vtecant = 0x00;
        }
    }

    return 0x00;
}

//-----------------------------------------------------------------------------
int processCmd(void)
{
    unsigned char linhacomando[32], linhaarg[32], vloop;
    unsigned char *blin = vbuf;
    unsigned char *vEndLoad = 0x00000000;
    unsigned short varg = 0;
    unsigned short ix, iy, iz, ikk, izz;
    unsigned short vbytepic = 0, vrecfim;
    unsigned char sqtdtam[10], cuntam, vparam[32], vparam2[16], vparam3[16], vpicret, vresp;
    int vRet = 1;

    // Separar linha entre comando e argumento
    linhacomando[0] = '\0';
    linhaarg[0] = '\0';
    ix = 0;
    iy = 0;
    while (*blin)
    {
        if (!varg && *blin == 0x20)
        {
            varg = 0x01;
            linhacomando[ix] = '\0';
            iy = ix;
            ix = 0;
        }
        else
        {
            if (!varg)
                linhacomando[ix] = upper_ascii(*blin);
            else
                linhaarg[ix] = upper_ascii(*blin);
            ix++;
        }

        *blin++;
    }

    if (!varg)
    {
        linhacomando[ix] = '\0';
        iy = ix;
    }
    else
    {
        linhaarg[ix] = '\0';

        ikk = 0;
        iz = 0;
        izz = 0;
        varg = 0;
        while (ikk < ix)
        {
            if (linhaarg[ikk] == 0x20)
                varg++;
            else
            {
                if (!varg)
                    vparam[ikk] = linhaarg[ikk];
                else if (varg == 1)
                {
                    vparam2[iz] = linhaarg[ikk];
                    iz++;
                }
                else if (varg == 2)
                {
                    vparam3[izz] = linhaarg[ikk];
                    izz++;
                }
            }

            ikk++;
        }
    }

    vparam[ikk] = '\0';
    vparam2[iz] = '\0';
    vparam3[izz] = '\0';

    vpicret = 0;

    // Processar e definir o que fazer
    if (linhacomando[0] != 0)
    {
        if (!strcmp(linhacomando,"CLS") && iy == 3)
        {
            clearScr();
            vRet = 0;
        }
        else if (!strcmp(linhacomando,"CLEAR") && iy == 5)
        {
            clearScr();
            vRet = 0;
        }
        else if (!strcmp(linhacomando,"VER") && iy == 3)
        {
            printText("MMSJ-320 BIOS v"versionBios);
        }
        else if (!strcmp(linhacomando,"LOAD") && iy == 4)
        {
            printText("Wait...\r\n\0");
            if (linhaarg[0] != 0x00)
                vEndLoad = hexToLong(linhaarg);

            loadSerialToMem(vEndLoad, 1);
        }
        else if (!strcmp(linhacomando,"RUN") && iy == 3)
        {
            if (linhaarg[0] != 0x00)
                vEndLoad = hexToLong(linhaarg);
            else
                vEndLoad = 0x00810000;

            runMem(vEndLoad);
        }
        else if (!strcmp(linhacomando,"BASIC") && iy == 5)
        {
            runBasic(linhaarg);
        }
        else if (!strcmp(linhacomando,"MODE") && iy == 4)
        {
            modeVideo(vparam);
        }
        else if (!strcmp(linhacomando,"POKE") && iy == 4)
        {
            pokeMem(vparam, vparam2);
        }
        else if (!strcmp(linhacomando,"LOADSO") && iy == 6)
        {
            carregaOSDisk();
        }
        else if (!strcmp(linhacomando,"RUNSO") && iy == 5)
        {
            runSystemOper();
        }
        else if (!strcmp(linhacomando,"DEBUG") && iy == 5)
        {
            if (debugMessages)
            {
                debugMessages = 0;
                printText("Debug Messages Off\r\n\0");
            }
            else
            {
                debugMessages = 1;
                printText("Debug Messages On\r\n\0");
            }
        }
        else if (strcmp(linhacomando,"DUMP") == 0 && iy == 4)
        {
            dumpMem(vparam, vparam2, vparam3);
        }
        else if (strcmp(linhacomando,"DDUUMMPP") == 0 && iy == 8)
        {
            dumpMem("6020A0\0", "128\0", "\0");
        }
        else if (strcmp(linhacomando,"DUMPS") == 0 && linhacomando[4] == 'S' && iy == 5)
        {
            dumpMem2(vparam, vparam2);
        }
        else if (strcmp(linhacomando,"DUMPW") == 0 && linhacomando[4] == 'W' && iy == 5)
        {
            dumpMemWin(vparam, vparam2, vparam3);
        }
        else
        {
            vresp = 0;

            if (!vresp)
                printText("Unknown Command !!!\r\n\0");
        }
    }

    return vRet;
}

//-----------------------------------------------------------------------------
unsigned char readMouse(unsigned char *vStat, unsigned char *vMovX, unsigned char *vMovY)
{
    unsigned char ix = 0;

    *vStat = 0;
    *vMovX = 0;
    *vMovY = 0;

    if (MseMovPtrR != MseMovPtrW)
    {
        // Pega proximos 3 status do mouse disponivel
        for(ix=0; ix<3; ix++)
        {
            switch (ix)
            {
                case 0:
                    *vStat = MseMovBuffer[MseMovPtrR];
                    break;
                case 1:
                    *vMovX = MseMovBuffer[MseMovPtrR];
                    break;
                case 2:
                    *vMovY = MseMovBuffer[MseMovPtrR];
                    break;
            }

            MseMovPtrR++;

            if (MseMovPtrR > kbdKeyBuffMax)
                MseMovPtrR = 0;

            // Erro de leitura ou gravacao vinda do mouse. Ignora essa leitura
            if (MseMovPtrR == MseMovPtrW && ix < 2)
            {
                *vStat = 0;
                *vMovX = 0;
                *vMovY = 0;
                ix = 0;
                break;
            }
        }
    }

    return ix;
}

//-----------------------------------------------------------------------------
unsigned char readChar(void)
{
    unsigned char ix = 0, vmove;

    vBufReceived = 0;

    if (kbdKeyPtrR != kbdKeyPtrW)
    {
        // Pega proxima tecla disponivel
        vBufReceived = kbdKeyBuffer[kbdKeyPtrR];

        kbdKeyPtrR++;

        if (kbdKeyPtrR > kbdKeyBuffMax)
            kbdKeyPtrR = 0;
    }

    #ifdef __MON_SERIAL_KBD__
        if (vBufReceived == 0x00)
        {
            if ((*(vmfp + REG_RSR) & 0x80))  // Se buffer de recepcao cheio
            {
                vBufReceived = *(vmfp + REG_UDR);
            }
        }
    #endif

    return vBufReceived;
}

//-----------------------------------------------------------------------------
void hideCursor(void)
{
    if (!videoCursorShow)  // Cursor já esta escondido, nao faz nada
        return;

    videoCursorShow = 0;

    printChar(0xFF, 1);
}

//-----------------------------------------------------------------------------
void showCursor(void)
{
    if (videoCursorShow)   // Cursor já esta aparecendo, nao faz nada
        return;

    videoCursorShow = 1;

    printChar(0xFF, 1);
}

//-----------------------------------------------------------------------------
void modeVideo(unsigned char *pMode)
{
    unsigned long vMode = 0;

    if (pMode[0] != 0x00)
    {
        vMode = atol(pMode);

        if (vMode <= 3)
        {
            switch(vMode)
            {
                case 0:
                    vdp_init_textmode(VDP_WHITE, VDP_BLACK);
                    break;
                case 1:
                    vdp_init_g1(VDP_WHITE, VDP_BLACK);
                    break;
                case 2:
                    vdp_init_g2(1, 0);
                    break;
                case 3:
                    vdp_init_multicolor();
                    break;
            }

            clearScr();
        }
        else
            vMode = 0xFF;
    }
    else
        vMode = 0xFF;

    if (vMode == 0xFF && vdp_mode == VDP_MODE_TEXT)
    {
        printText("usage: mode [code]\r\n\0");
        printText("   code: 0 = Text Mode 40x24\r\n\0");
        printText("         1 = Graphic Text Mode 32x24\r\n\0");
        printText("         2 = Graphic 256x192\r\n\0");
        printText("         3 = Graphic 64x48\r\n\0");
    }
}

//-----------------------------------------------------------------------------
void asctohex(unsigned char a, unsigned char *s)
{
     unsigned char c;
     c = (a >> 4) & 0x0f;
     if (c <= 9) c+= '0'; else c += 'a' - 10;
     *s++ = c;
     c = a & 0x0f;
     if (c <= 9) c+= '0'; else c += 'a' - 10;
     *s++ = c;
     *s = 0;
}

//-----------------------------------------------------------------------------
int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

//-----------------------------------------------------------------------------
unsigned long pow(int val, int pot)
{
    int ix;
    int base = val;

    if (val != 0)
    {
        if (pot == 0)
            val = 1;
        else if (pot == 1)
            val = base;
        else
        {
            for (ix = 0; ix <= pot; ix++)
            {
                if (ix >= 2)
                    val *= base;
            }
        }
    }

    return val;
}

//-----------------------------------------------------------------------------
unsigned long hexToLong(char *pHex)
{
    int ix;
    unsigned char ilen = strlen(pHex) - 1;
    unsigned long pVal = 0;

    for (ix = ilen; ix >= 0; ix--)
    {
        pVal += hex2int(pHex[ilen - ix]) * pow(16, ix);
    }

    return pVal;
}


//-----------------------------------------------------------------------------
void pokeMem(unsigned char *pEnder, unsigned char *pByte)
{
    unsigned char *vEnder = hexToLong(pEnder);
    unsigned long tByte = hexToLong(pByte);
    unsigned char vByte = 0;

    if (pEnder[0] != 0x00 && pByte[0] != 0x00)
    {
        vByte = (unsigned char)tByte;

        *vEnder = vByte;
    }
    else
    {
        if (vdp_mode == VDP_MODE_TEXT)
            printText("usage: poke <ender> <byte>\r\n\0");
    }
}

//-----------------------------------------------------------------------------
// dump <ender> [qtd (default 64)] [cols (default 8 (42cols) or 4 (32cols))]
//-----------------------------------------------------------------------------
void dumpMem (unsigned char *pEnder, unsigned char *pqtd, unsigned char *pCols)
{
    unsigned char ptype = 0x00;
    unsigned char *pender = hexToLong(pEnder);
    unsigned long vqtd = 64, ix;
    unsigned long vcols = 8;
    int iy;
    unsigned char shex[4], vchr[2];
    unsigned char pbytes[16];
    char vbuffer [sizeof(long)*8+1];
    char buffer[10];
    int i=0;
    int j=0;

    if (pEnder[0] == 0)
    {
        if (vdp_mode == VDP_MODE_TEXT)
        {
            printText("usage: dump <ender> [qtd] [cols]\r\n\0");
            printText("    qtd: default 64\r\n\0");
            printText("   cols: default 8\r\n\0");
        }
        return;
    }

    if (vdpMaxCols == 32)
        vcols = 4;

    if (pqtd[0] != 0x00)
        vqtd = atol(pqtd);

    if (pCols[0] != 0x00)
        vcols = atol(pCols);

    for (ix = 0; ix < vqtd; ix += vcols)
    {
        ltoa (pender,vbuffer,16);
        for (i=0; i<(6-strlen(vbuffer));i++) {
            buffer[i]='0';
        }
        for(j=0;j<strlen(vbuffer);j++){
            buffer[i] = vbuffer[j];
            i++;
            buffer[i] = 0x00;
        }

        printText(buffer);
        printChar(':', 1);

        for (iy = 0; iy < vcols; iy++)
            pbytes[iy] = *pender++;

        for (iy = 0; iy < vcols; iy++)
        {
            asctohex(pbytes[iy], shex);
            printText(shex);

            if ((vcols - iy) >= 2)
                printChar(' ', 1);
        }

        printText("|\0");

        for (iy = 0; iy < vcols; iy++)
        {
            if (pbytes[iy] >= 0x20)
            {
                vchr[0] = pbytes[iy];
                vchr[1] = 0x00;
                printText(vchr);
            }
            else
                printChar('.', 1);
        }

        printText("\r\n\0");
    }
}

//-----------------------------------------------------------------------------
// dumps <ender> [qtd (default 256)]
// Joga direto pra serial
//-----------------------------------------------------------------------------
void dumpMem2 (unsigned char *pEnder, unsigned char *pqtd)
{
    unsigned char ptype = 0x00;
    unsigned char *pender = hexToLong(pEnder);;
    unsigned long vqtd = 256, ix;
    int iy;
    unsigned char shex[4];
    unsigned char pbytes[16];
    char vbuffer [sizeof(long)*8+1];
    char buffer[10];
    int i=0;
    int j=0;

    if (pEnder[0] == 0)
    {
        if (vdp_mode == VDP_MODE_TEXT)
            writeLongSerial("usage: dump <ender initial> [qtd (default 256)]\r\n\0");
        return;
    }

    if (pqtd[0] != 0x00)
        vqtd = atol(pqtd);

    for (ix = 0; ix < vqtd; ix += 16)
    {
        ltoa (pender,vbuffer,16);
        for (i=0; i<(6-strlen(vbuffer));i++) {
            buffer[i]='0';
        }
        for(j=0;j<strlen(vbuffer);j++){
            buffer[i] = vbuffer[j];
            i++;
            buffer[i] = 0x00;
        }

        writeLongSerial(buffer);
        writeLongSerial("h : ");

        for (iy = 0; iy < 16; iy++)
        {
            pbytes[iy] = *pender;
            pender = pender + vdpAddCol;
        }

        for (iy = 0; iy < 16; iy++)
        {
            asctohex(pbytes[iy], shex);
            writeLongSerial(shex);
            writeSerial(' ');
        }

        writeLongSerial(" | \0");

        for (iy = 0; iy < 16; iy++)
        {
            if (pbytes[iy] >= 0x20)
                writeSerial(pbytes[iy]);
            else
                writeSerial('.');
        }

        writeLongSerial("\r\n\0");
    }
}

//-----------------------------------------------------------------------------
// dumpw <ender> [qtd (default 64)] [cols (default 8 (42cols) or 4 (32cols))]
//-----------------------------------------------------------------------------
void dumpMemWin (unsigned char *pEnder, unsigned char *pqtd, unsigned char *pCols)
{
    unsigned char ptype = 0x00;
    unsigned char *pender = hexToLong(pEnder);
    unsigned char *blin = vbuf;
    unsigned long vqtd = 128, ix;
    unsigned long vcols = 8;
    int iy;
    unsigned char shex[4], vchr[2];
    unsigned char pbytes[16];
    char vbuffer [sizeof(long)*8+1];
    char buffer[10];
    int i=0;
    int j=0;
    unsigned char vRetInput;

    if (pEnder[0] == 0)
    {
        if (vdp_mode == VDP_MODE_TEXT)
        {
            printText("usage: dumpw <ender> [qtd] [cols]\r\n\0");
            printText("    qtd: default 128\r\n\0");
            printText("   cols: default 8\r\n\0");
        }
        return;
    }

    if (vdpMaxCols == 32)
    {
        if (vdp_mode == VDP_MODE_TEXT)
            printText("dumpw Only Works in 40 cols\r\n\0");
        return;
    }

    if (pqtd[0] != 0x00)
        vqtd = atol(pqtd);

    if (pCols[0] != 0x00)
        vcols = atol(pCols);

    clearScr();
    printText("             DUMPW v0.1                \r\n\0");
    printChar(218,1);
    for (ix = 0; ix < 5; ix++)
        printChar(196,1);
    printChar(194,1);
    for (ix = 0; ix < 23; ix++)
        printChar(196,1);
    printChar(194,1);
    for (ix = 0; ix < 7; ix++)
        printChar(196,1);
    printChar(191,1);
    printText("\r\n");
    printChar(179,1);
    printText("Addr ");
    printChar(179,1);
    printText("         Bytes         ");
    printChar(179,1);
    printText(" ASCII ");
    printChar(179,1);
    printText("\r\n");
    printChar(192,1);
    for (ix = 0; ix < 5; ix++)
        printChar(196,1);
    printChar(193,1);
    for (ix = 0; ix < 23; ix++)
        printChar(196,1);
    printChar(193,1);
    for (ix = 0; ix < 7; ix++)
        printChar(196,1);
    printChar(217,1);
    printText("\r\n");

    vdp_set_cursor(0, 20);
    printChar(218,1);
    for (ix = 0; ix < 37; ix++)
        printChar(196,1);
    printChar(191,1);
    printText("\r\n");
    printChar(179,1);
    printText(" <-:Prev  ->:Next  <");
        printChar(217,1);
    printText(":Addr  ESC:Exit ");
    printChar(179,1);
    printText("\r\n");
    printChar(192,1);
    for (ix = 0; ix < 37; ix++)
        printChar(196,1);
    printChar(217,1);
    printText("\r\n");

    if (vdpMaxCols == 32)
        vcols = 4;

    while (1)
    {
        vdp_set_cursor(0, 4);

        for (ix = 0; ix < vqtd; ix += vcols)
        {
            ltoa (pender,vbuffer,16);
            for (i=0; i<(6-strlen(vbuffer));i++) {
                buffer[i]='0';
            }
            for(j=0;j<strlen(vbuffer);j++){
                buffer[i] = vbuffer[j];
                i++;
                buffer[i] = 0x00;
            }

            printText(buffer);
            printChar(':', 1);

            for (iy = 0; iy < vcols; iy++)
                pbytes[iy] = *pender++;

            for (iy = 0; iy < vcols; iy++)
            {
                asctohex(pbytes[iy], shex);
                printText(shex);

                if ((vcols - iy) >= 2)
                    printChar(' ', 1);
            }

            printText("|\0");

            for (iy = 0; iy < vcols; iy++)
            {
                if (pbytes[iy] >= 0x20)
                {
                    vchr[0] = pbytes[iy];
                    vchr[1] = 0x00;
                    printText(vchr);
                }
                else
                    printChar('.', 1);
            }

            printText("\r\n\0");
        }

        while (1)
        {
            vRetInput = inputLine(1,'@');

            if (vRetInput == 0x12)  // LeftArrow (18)
            {
                pender = pender - 256;

                if (pender > 0x00FFFFFF)
                    pender = 0x00FFFF00;

                break;
            }
            else if (vRetInput == 0x14)  // RightArrow (20)
            {
                if (pender > 0x00FFFFFF)
                    pender = 0x00000000;

                break;
            }
            else if (vRetInput == 0x0D)  // Enter (13)
            {
                vdp_set_cursor(1, 21);
                printText(" Address(HEX):                       \0");

                vdp_set_cursor(16, 21);

                vRetInput = inputLine(6,'$');

                vdp_set_cursor(1, 21);
                printText(" <-:Prev  ->:Next  <");
                    printChar(217,1);
                printText(":Addr  ESC:Exit ");

                if (vRetInput != 0x1B)
                {
                    blin = vbuf;
                    pender = hexToLong(blin);

                    if (pender > 0x00FFFF00)
                        pender = 0x00FFFF00;

                    break;
                }
            }
            else if (vRetInput == 0x1B)  // ESC
            {
                break;
            }
        }

        if (vRetInput == 0x1B)  // ESC
        {
            break;
        }
    }

    clearScr();
}

//-----------------------------------------------------------------------------
void writeSerial(unsigned char pchr)
{
    while(!(*(vmfp + REG_TSR) & 0x80));  // Aguarda buffer de transmissao estar vazio
    *(vmfp + REG_UDR) = pchr;
    vBufXmitEmpty = 0;     // Indica que o buffer de transmissao esta cheio
}

//-----------------------------------------------------------------------------
void writeLongSerial(unsigned char *msg)
{
    while (*msg)
    {
        writeSerial(*msg++);
    }
}

//-----------------------------------------------------------------------------
unsigned long lstmGetSize(void)
{
    return vSizeTotalRec;
}

//-----------------------------------------------------------------------------
// load <ender initial to save>
//-----------------------------------------------------------------------------
//         Uses XMODEM Protocol
//-----------------------------------------------------------------------------
// ptipo : 1 = mostra mensagens 0 = nao mostra e apenas retorna os erros ou 0x00 carregado com sucesso
//-----------------------------------------------------------------------------
unsigned char loadSerialToMem(unsigned char *pEndStart, unsigned char ptipo)
{
    unsigned long vTamanho;
    unsigned char vHeader[3];
    unsigned int vchecksum = 0;
    unsigned char inputBuffer, verro = 0;
    unsigned char *vEndSave = 0x00000000;
    unsigned char *vEndOld  = 0x00000000;
    unsigned char *pEnder  = pEndStart;
    unsigned long vTimeout = 0, vchecksumcalc = 0;
    unsigned char sqtdtam[20];
    unsigned char vinicio = 0x00;
    unsigned char vStart = 0x00;
    unsigned char vBlockOld = 0x00;
    unsigned int vAnim = 1000;

    if (ptipo)
        printText("Receiving. <Esc> to Cancel... \r\n\0");

    // Desabilita Timers and Mouse Interruption
//    *(vmfp + REG_IERA) &= 0x5E;

    vSizeTotalRec = 0;
    kbdKeyBuffer[kbdKeyPtrR] = 0x00;

    if (pEnder == 0)
        pEndStart = malloc(1024);

    printText("Address loading: 0x");
    itoa(pEndStart, sqtdtam, 16);
    printText(sqtdtam);
    printText(" \0");

    vEndSave = pEndStart;
    vEndOld = pEndStart;

    while(1)
    {
        inputBuffer = 0;
        vTimeout = 0;

        if (ptipo)
        {
            switch (vAnim)
            {
                case 800:
                    vdp_write(0x2F);    // Show "/"
                    break;
                case 1600:
                    vdp_write(0x2D);    // Show "-"
                    break;
                case 2400:
                    vdp_write(0x5C);    // Show "\"
                    break;
                case 3200:
                    vdp_write(0x7C);    // Show "|"
                    vAnim = 0;
                    break;
            }

            vAnim++;
        }

        while(!(*(vmfp + REG_RSR) & 0x80))
        {
            if (kbdKeyBuffer[kbdKeyPtrR] == 0x1B)  // ESC
                break;

            if(vinicio == 0x00 && vStart == 0x00)
            {
                if ((vTimeout % 100000) == 0) // +/- 10s
                {
                    //*(vmfp + REG_GPDR) = 0x01;

                    writeSerial(0x15);    // Send NACK to start
                }
                /*else
                {
                    *(vmfp + REG_GPDR) = 0x01;
                }*/
            }

            vTimeout++;
            if (vTimeout > 3000000) // +/- 5 min
                break;
        };

        if (kbdKeyBuffer[kbdKeyPtrR] == 0x1B)  // ESC
        {
            verro = 99;
            break;
        }

        if (vTimeout > 3000000)
            break;

        inputBuffer = *(vmfp + REG_UDR);

        if (vinicio == 0 && inputBuffer == 0x04)    // Primeiro byte eh EOT
        {
            writeSerial(0x06);    // Send ACK
            break;
        }
        else if (vinicio < 3)
        {
            //*(vmfp + REG_GPDR) = 0x04;

            vHeader[vinicio] = inputBuffer;

            if (vinicio == 1)
            {
                if (vBlockOld == inputBuffer)
                    vEndSave = vEndOld;
                else
                {
                    vEndOld = vEndSave;
                    vBlockOld = inputBuffer;
                }
            }

            vinicio++;
            vchecksumcalc = 0;
            verro = 0;
            vStart = 0x01;
        }
        else if (vinicio == 131)
        {
            //*(vmfp + REG_GPDR) = 0x05;

            vinicio = 0;
            vchecksum = inputBuffer;
            if ((vchecksumcalc % 256) != vchecksum)
            {
                //*(vmfp + REG_GPDR) = 0x00;
                verro = 1;
                vEndSave = vEndOld;
                writeSerial(0x15);    // Send NACK
            }
            else
            {
                //*(vmfp + REG_GPDR) = 0x01;
                writeSerial(0x06);    // Send ACK
            }
        }
        else
        {
            *vEndSave++ = inputBuffer;
            vchecksumcalc += inputBuffer;
            vinicio++;
            vSizeTotalRec = vSizeTotalRec + 1;
            if (pEnder == 0)
            {
                if (vSizeTotalRec >= vSizeTotalRec + 1024)
                    realloc(pEndStart, (vSizeTotalRec + 1024));
            }
        }
    }

    vdp_write(' ');

    printText("\r\n\0");

    // Habilita Timers Interruption
//    *(vmfp + REG_IERA) |= 0x61;

    if (vTimeout > 3000000)
    {
        if (pEnder == 0)
            free(pEndStart);

        if (ptipo)
            printText("Timeout. Process Aborted.\r\n\0");

        return 0xFE;
    }
    else
    {
        if (!verro)
        {
            if (ptipo)
            {
                printText("File loaded in to memory successfuly.\r\n\0");

                printText("Address loaded: 0x");
                itoa(pEndStart, sqtdtam, 16);
                printText(sqtdtam);
                printText("\r\n\0");
            }

            return 0x00;
        }
        else
        {
            if (pEnder == 0)
                free(pEndStart);

            if (ptipo)
            {
                if (verro == 99)
                    printText("Loading aborted.\r\n\0");
                else
                    printText("File loaded in to memory with checksum errors.\r\n\0");
            }

            return 0xFD;
        }
    }

    return 0xF0;
}

//-----------------------------------------------------------------------------
void runMem(unsigned long pEnder)
{
    runMemory = pEnder;
    runCmd();
}

//-----------------------------------------------------------------------------
void runBasic(unsigned long pEnder)
{
    runBas();
}

//-----------------------------------------------------------------------------
void runSystemOper(void)
{
    runSO();
}

//-----------------------------------------------------------------------------
int fsSendByte(unsigned char vByte, unsigned char pType)
{
    if (pType == 0)
        *vdskc = vByte;
    else if (pType == 1)
        *vdskd = vByte;
    else if (pType == 2)
        *vdskp = vByte;

    return 1;
}

//-----------------------------------------------------------------------------
unsigned char fsRecByte(unsigned char pType)
{
    unsigned char vByte;

    if (pType == 0)
        vByte = *vdskc;
    else if (pType == 1)
        vByte = *vdskd;

    return vByte;
}

//-----------------------------------------------------------------------------
unsigned int carregaSO(void)
{
    unsigned char *xaddress = 0x00800000;
    unsigned char vbyteprog[128], vbytes[4], dd, vByte = 0;
    unsigned int ix, cc;
    unsigned int vSizeFile;
    unsigned char sqtdtam[11];
    unsigned char vPosAnim = 0, vStep;
    unsigned short vAntX = 0;

    // Envia comando resetar e abortar tudo
    fsSendByte('a', FS_CMD);

    // Comando recebido ok ?
    vByte = fsRecByte(FS_CMD);

    if (vByte != ALL_OK)
        return vByte;

    // Envia comando
    fsSendByte('s', FS_CMD);

    // Comando recebido ok ?
    vByte = fsRecByte(FS_CMD);

    if (vByte != ALL_OK)
        return vByte;

    // Comando Executado ok ?
    vByte = fsRecByte(FS_CMD);

    if (vByte != ALL_OK)
        return vByte;

    printText(" ");

    /*--------------*/
    vAntX = videoCursorPosColX;
    vStep = 0;
    /*--------------*/

    while (1)
    {
        // Verifica o tamanho recebido
        vByte = fsRecByte(FS_CMD);
        vSizeFile = vByte << 8;
        vByte = fsRecByte(FS_CMD);
        vSizeFile |= vByte;

        /*--------------*/
        vStep++;

        if (vStep % 13 == 0)
        {
            if (vPosAnim == 3) 
            {
                vPosAnim = 0;
                videoCursorPosColX = vAntX;
                printText("   ");
                videoCursorPosColX = vAntX;
            }
            else
            {
                printChar('<',1);
                vPosAnim++;
            }
        }
        /*--------------*/

        // Carrega Dados Recebidos
        for (cc = 0; cc < vSizeFile ; cc++)
        {
            vByte = fsRecByte(FS_DATA);
            *xaddress = vByte;
            xaddress += 1;
        }

        if (vSizeFile < 512)
            break;

        fsSendByte('t', FS_CMD);    // Continua Enviado o SO

        // Comando recebido ok ?
        vByte = fsRecByte(FS_CMD);

        if (vByte != ALL_OK)
            return vByte;

        // Comando Executado ok ?
        vByte = fsRecByte(FS_CMD);

        if (vByte != ALL_OK)
            return vByte;
    }

    videoCursorPosColX = vAntX;
    printText("Done!");
//    printChar(' ',0);

    printText("\r\n\0");

    return 0;
}

//-----------------------------------------------------------------------------
void carregaOSDisk(void)
{
    unsigned int verro;

    printText("Loading OS. Please Wait...\0");

    verro = carregaSO();

    if (verro)
        printText("IO Error....\r\n\0");
    else {
        printText("Ok\r\n\0");
    }
}

//-----------------------------------------------------------------------------
// Delay Function
//-----------------------------------------------------------------------------
void delayms(int pTimeMS)
{
    volatile unsigned int ix;
    unsigned int iTempo = (100 * pTimeMS);

    for(ix = 0; ix <= iTempo; ix++);    // +/- 1ms * pTimeMs parada
}

//-----------------------------------------------------------------------------
void delayus(int pTimeUS)
{
    volatile unsigned int ix;

    pTimeUS /= 4;

    for(ix = 0; ix <= pTimeUS; ix++);    // +/- 1us * pTimeMs parada
}

#ifdef __KEYPS2__
//-----------------------------------------------------------------------------
// KBD PS2 Functions
//-----------------------------------------------------------------------------
void processCode(void)
{
    unsigned char decoded;

    if ((scanCode /*| 0x10*/ ) == 0xF0)
    {
        // release code!
        *kbdvreleased = 0x01;
    }
    else if ((scanCode /*| 0x10*/ ) == 0xE0)
    {
        // apenas prepara para o proximo codigo
        *kbdve0 = 0x01;
    }
    else if ((scanCode /*| 0x10*/ ) == 0xE1)
    {
        // apenas prepara para o proximo codigo
    }
    else
    {
        // normal character received
        if (!*kbdvcaps && !*kbdvshift)
            decoded = convertCode(scanCode,keyCode,ascii);
        else if (!*kbdvcaps && *kbdvshift)
            decoded = convertCode(scanCode,keyCode,ascii2);
        else if (*kbdvcaps && !*kbdvshift)
            decoded = convertCode(scanCode,keyCode,ascii3);
        else if (*kbdvcaps && *kbdvshift)
            decoded = convertCode(scanCode,keyCode,ascii4);

        if (decoded != '\0')
        {
            // allowed key code character received
            if (!*kbdvreleased)
            {
                kbdKeyBuffer[kbdKeyPtrW] = decoded;
                kbdKeyPtrW++;
                if (kbdKeyPtrW > kbdKeyBuffMax)
                    kbdKeyPtrW = 0;
            }
        }
        else
        {
            // other character received
            switch (scanCode)
            {
                case 0x12:  // Shift
                case 0x59:
                    *kbdvshift = ~*kbdvreleased & 0x01;
                    break;
                case 0x14:  // Ctrl
                    *kbdvctrl = ~*kbdvreleased & 0x01;
                    break;
                case 0x11:  // Alt
                    *kbdvalt = ~*kbdvreleased & 0x01;
                    break;
                case 0x58:  // Caps Lock
                    if (!*kbdvreleased)
                    {
                        *kbdvcaps = ~*kbdvcaps & 0x01;
                    }
                    break;
                case 0x77:  // Num Lock
                    if (!*kbdvreleased)
                    {
                        *kbdvnum = ~*kbdvnum & 0x01;
                    }
                    break;
                case 0x7E:  // Scroll Lock
                    if (!*kbdvreleased)
                    {
                        *kbdvscr = ~*kbdvscr & 0x01;
                    }
                    break;
                case 0x66:  // backspace
                    if (!*kbdvreleased)
                    {
                        kbdKeyBuffer[kbdKeyPtrW] = 0x08;
                        kbdKeyPtrW++;
                        if (kbdKeyPtrW > kbdKeyBuffMax)
                            kbdKeyPtrW = 0;
                    }
                    break;
                case 0x5A:  // enter
                    if (!*kbdvreleased)
                    {
                        kbdKeyBuffer[kbdKeyPtrW] = 0x0D;
                        kbdKeyPtrW++;
                        if (kbdKeyPtrW > kbdKeyBuffMax)
                            kbdKeyPtrW = 0;
                    }
                    break;
                case 0x76:  // ESCAPE
                    if (!*kbdvreleased)
                    {
                        kbdKeyBuffer[kbdKeyPtrW] = 0x1B;
                        kbdKeyPtrW++;
                        if (kbdKeyPtrW > kbdKeyBuffMax)
                            kbdKeyPtrW = 0;
                    }
                    break;
                case 0x0D:  // TAB
                    if (!*kbdvreleased)
                    {
                        kbdKeyBuffer[kbdKeyPtrW] = 0x09;
                        kbdKeyPtrW++;
                        if (kbdKeyPtrW > kbdKeyBuffMax)
                            kbdKeyPtrW = 0;
                    }
                    break;
                case 0x75: // up arrow
                    if (!*kbdvreleased)
                    {
                        if (*kbdve0)
                            kbdKeyBuffer[kbdKeyPtrW] = 0x11; // 17
                        else
                            kbdKeyBuffer[kbdKeyPtrW] = '8';

                        kbdKeyPtrW++;
                        if (kbdKeyPtrW > kbdKeyBuffMax)
                            kbdKeyPtrW = 0;
                    }
                    break;
                case 0x6B: // left arrow
                    if (!*kbdvreleased)
                    {
                        if (*kbdve0)
                            kbdKeyBuffer[kbdKeyPtrW] = 0x12; // 18
                        else
                            kbdKeyBuffer[kbdKeyPtrW] = '4';

                        kbdKeyPtrW++;
                        if (kbdKeyPtrW > kbdKeyBuffMax)
                            kbdKeyPtrW = 0;
                    }
                    break;
                case 0x72: // down arrow
                    if (!*kbdvreleased)
                    {
                        if (*kbdve0)
                            kbdKeyBuffer[kbdKeyPtrW] = 0x13; // 19
                        else
                            kbdKeyBuffer[kbdKeyPtrW] = '2';

                        kbdKeyPtrW++;
                        if (kbdKeyPtrW > kbdKeyBuffMax)
                            kbdKeyPtrW = 0;
                    }
                    break;
                case 0x74: // right arrow
                    if (!*kbdvreleased)
                    {
                        if (*kbdve0)
                            kbdKeyBuffer[kbdKeyPtrW] = 0x14; // 20
                        else
                            kbdKeyBuffer[kbdKeyPtrW] = '6';

                        kbdKeyPtrW++;
                        if (kbdKeyPtrW > kbdKeyBuffMax)
                            kbdKeyPtrW = 0;
                    }
                    break;
            } // end switch

            *kbdvreleased = 0;
            *kbdve0 = 0;
        } // end if (decoded>0x00)

        *kbdvreleased = 0;
    }
}

//-----------------------------------------------------------------------------
unsigned char convertCode(unsigned char codeToFind, unsigned char *source, unsigned char *destination)
{
    while(*source != codeToFind && *source++ > 0x00)
        destination++;

    return *destination;
}

//-----------------------------------------------------------------------------
void sendByte(unsigned char b)
{
    /*unsigned char a=0;
    unsigned char p = 1;
    unsigned char t = 0;

    // Desabilita KBD and VDP Interruption
    *(vmfp + REG_IERA) &= 0x3E;

    *(vmfp + REG_GPDR) &= 0xBF; // Zera Clock (I6)
    *(vmfp + REG_DDR)  |= 0x40; // I6 as Output

    delayus(125);

    *(vmfp + REG_GPDR) &= 0xFE; // Zera Data (I0)
    *(vmfp + REG_DDR)  |= 0x01; // I0 as Output

    delayus(125);

    *(vmfp + REG_DDR)  &= 0xBF; // I6 as Input

    for(a = 0; a < 8; a++) {
        t = (b >> a) & 0x01;

        while ((*(vmfp + REG_GPDR) & 0x40) == 0x40); //wait clock for 0

        *(vmfp + REG_GPDR) |= t;

        if (t) p++;

        while ((*(vmfp + REG_GPDR) & 0x40) == 0x00); //wait clock for 1
    }

    while((*(vmfp + REG_GPDR) & 0x40) == 0x40); //wait clock for 0
    *(vmfp + REG_GPDR) |= p & 0x01;
    while((*(vmfp + REG_GPDR) & 0x40) == 0x00); //wait clock for 1

    *(vmfp + REG_DDR)  &= 0xFE; // I0 as Input
    while((*(vmfp + REG_GPDR) & 0x01) == 0x01); //wait data for 0
    while((*(vmfp + REG_GPDR) & 0x40) == 0x40); //wait clock for 0

    // Habilita KBD and VDP Interruption
    *(vmfp + REG_IERA) |= 0xC0;*/
}
#endif

//-----------------------------------------------------------------------------
void basicFuncBios(void)
{
}

//-----------------------------------------------------------------------------
void funcSpuriousInt(void)
{
}

//-----------------------------------------------------------------------------
void funcIntPIC(void)
{
    // Chamada de dados do PIC para o processador
}

//-----------------------------------------------------------------------------
void funcIntUsbSerial(void)
{
}

//-----------------------------------------------------------------------------
void funcIntVideo(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMouse(void)
{
}

//-----------------------------------------------------------------------------
void funcIntKeyboard(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMultiTask(void)
{
    // Nao usara por enquanto, porque sera controlado pelo SO
    // E serah feito em ASM por causa das trocas de SP (A7)
}

//-----------------------------------------------------------------------------
void funcIntMfpGpi0(void)
{
    // TBD

    *(vmfp + REG_ISRB) &= 0xFE;  // Reseta flag de interrupcao GPI0 no MFP
}

//-----------------------------------------------------------------------------
void funcIntMfpGpi1(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMfpGpi2(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMfpGpi3(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMfpTmrD(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMfpTmrC(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMfpGpi4(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMfpGpi5(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMfpTmrB(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMfpXmitErr(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMfpXmitBufEmpty(void)
{
    vBufXmitEmpty = 1; // Buffer Transmissao Vazio
    //*(vmfp + REG_GPDR) = 0x05;
//    *(vmfp + REG_ISRA) &= 0xFB; // Reseta flag de interrupcao no MFP
}

//-----------------------------------------------------------------------------
void funcIntMfpRecErr(void)
{

}

//-----------------------------------------------------------------------------
void funcIntMfpRecBufFull(void)
{
    vBufReceived = *(vmfp + REG_UDR);   // Carrega byte do buffer do MFP
//    *(vmfp + REG_ISRA) &= 0xEF;  // Reseta flag de interrupcao no MFP
}

//-----------------------------------------------------------------------------
void funcIntMfpTmrA(void)
{
    SysClockms = SysClockms + 1;

    // Reseta flag de interrupcao no MFP do Timer A
//    *(vmfp + REG_ISRA) &= 0xDF;
}

//-----------------------------------------------------------------------------
void funcIntMfpGpi6(void)
{
    #ifdef __KEYPS2_EXT__
        unsigned char decoded = 0xFF;
        int vTimeout;

        if (debugMessages)
            writeLongSerial("Aqui 0\r\n\0");

        // Pega dados do controlador via protocolo
        while (decoded != 0)
        {
            vTimeout = 0x0FF;

            *(vmfp + REG_GPDR) &= 0xEF;  // Seta CS (I4) = 0 do controlador e/ou indicando que ja leu MSB
            while (*(vmfp + REG_GPDR) & 0x20 && vTimeout) vTimeout--; // Aguarda Controlador liberar LSB para leitura
            decoded = *(vmfp + REG_GPDR) & 0x0F;

            vTimeout = 0x0FF;

            *(vmfp + REG_GPDR) |= 0x10;  // Seta CS (I4) = 1 do controlador indicando que ja leu LSB
            while (!(*(vmfp + REG_GPDR) & 0x20) && vTimeout) vTimeout--; // Aguarda Controlador liberar MSB para leitura
            decoded |= ((*(vmfp + REG_GPDR) & 0x0F) << 4);

            if (!vTimeout)
            {
                if (debugMessages)
                    writeLongSerial("Aqui 0.1\r\n\0");

                *(vmfp + REG_GPDR) &= 0xEF;  // Seta CS (I4) = 0 do controlador e/ou indicando que ja leu MSB
                break;
            }

            if (decoded != 0x00)
            {
                // Coloca tecla digitada no buffer
                kbdKeyBuffer[kbdKeyPtrW] = decoded;
                kbdKeyPtrW = kbdKeyPtrW + 1;

                if (kbdKeyPtrW > kbdKeyBuffMax)
                    kbdKeyPtrW = 0;
            }

            *(vmfp + REG_GPDR) &= 0xEF;  // Seta CS (I4) = 0 do controlador e/ou indicando que ja leu MSB
        }

        *(vmfp + REG_GPDR) |= 0x10;  // Seta CS = 1 (I4) do controlador

        if (debugMessages)
            writeLongSerial("Aqui 1\r\n\0");
    #endif
    
    #ifdef __KEYPS2__
        unsigned int vTimeout;

        /*if (*kbdPs2Readtype == 1)
        {
            // Verifica se deu timout no timer A do MFP
            if (*kbdClockCount != 0 && (SysClockms > *kbdtimeout))
            {
                // Timer A zerou: timeout ocorreu. Reinicia nova sequencia
                scanCode = 0;
                *kbdClockCount = 0;
            }

            if (*kbdClockCount >= 1 && *kbdClockCount <= 8)
            {
                // No 11 bits received yet: add to the scancode [start][d0...d7][parity][stop]
                scanCode = (scanCode >> 1);
                scanCode = scanCode | ((*(vmfp + REG_GPDR) & 0x01) << 7);
            }

            if (*kbdClockCount == 10)
            {
                // 11 bits received: process the code
                processCode();

                scanCode = 0;
                *kbdClockCount = 0;
            }
            else
            {
                *kbdClockCount = *kbdClockCount + 1;
            }

            *kbdtimeout = SysClockms + 64;
        }
        else
        {*/
            vTimeout = 0x0FFF;
            while((*mfpgpdr & 0x01) && vTimeout) vTimeout--; // Wait data = 0
            if (vTimeout)
            {
                scanCode = readKbdPs2();

                if (scanCode != 0xFE)   // If not retry, continue
                {
                    scanCode = (scanCode >> 1) & 0x00FF;  // shift out the start bit

                    // Grava o scancode no buffer
                    kbdScanCodeBuf[kbdScanCodePtrW] = scanCode;

                    // Soma 1 no ponteiro de gravacao do buffer circular
                    kbdScanCodePtrW++;

                    // Se chegar em 16, volta pra 0
                    if (kbdScanCodePtrW > kbdKeyBuffMax)
                        kbdScanCodePtrW = 0;
                }
            }
        //}
    #endif

    // Reseta flag de interrupcao no MFP do I6
//    *(vmfp + REG_ISRA) &= 0xBF;
}

//-----------------------------------------------------------------------------
void funcIntMfpGpi7(void)
{
    #ifdef __MOUSEPS2__EXT__
        unsigned char decoded = 0xFF;
        int vTimeout;
        
        if (debugMessages)
            writeLongSerial("Aqui 2\r\n\0");

        // Pega dados do controlador via protocolo
        while (1)
        {
            if (*(vmfp + REG_GPDR) & 0x80)
            {
                break;
            }

            vTimeout = 0xFF;

            *(vmfp + REG_GPDR) &= 0xEF;  // Seta CS (I4) = 0 do controlador e/ou indicando que ja leu MSB
            while (*(vmfp + REG_GPDR) & 0x20 && vTimeout) vTimeout--; // Aguarda Controlador liberar LSB para leitura
            decoded = *(vmfp + REG_GPDR) & 0x0F;

            if (vTimeout)
                vTimeout = 0xFF;

            *(vmfp + REG_GPDR) |= 0x10;  // Seta CS (I4) = 1 do controlador indicando que ja leu LSB
            while (!(*(vmfp + REG_GPDR) & 0x20) && vTimeout) vTimeout--; // Aguarda Controlador liberar MSB para leitura
            decoded |= ((*(vmfp + REG_GPDR) & 0x0F) << 4);

            if (!vTimeout)
            {
                if (debugMessages)
                    writeLongSerial("Aqui 2.1\r\n\0");

                *(vmfp + REG_GPDR) &= 0xEF;  // Seta CS (I4) = 0 do controlador e/ou indicando que ja leu MSB
                break;
            }

            // Coloca dado mouse lido no buffer
            MseMovBuffer[MseMovPtrW] = decoded;
            MseMovPtrW = MseMovPtrW + 1;

            if (MseMovPtrW > kbdKeyBuffMax)
                MseMovPtrW = 0;

            *(vmfp + REG_GPDR) &= 0xEF;  // Seta CS (I4) = 0 do controlador e/ou indicando que ja leu MSB
        }

        *(vmfp + REG_GPDR) |= 0x10;  // Seta CS = 1 (I4) do controlador

        // Verifica se ao final, o cursor de gravacao é modulo 3, ou seja, sempre entrou 3 dados do mouse
        // Se nao for modulo 3, volta até ser modulo 3.
        while ((MseMovPtrW % 3) != 0)
            MseMovPtrW = MseMovPtrW - 1;

        if (debugMessages)
            writeLongSerial("Aqui 3\r\n\0");
    #endif
}

#ifdef __KEYPS2__
//-----------------------------------------------------------------------------
// read a byte from Keyboard
//-----------------------------------------------------------------------------
unsigned char readKbdPs2 (void)
{
    unsigned long bData = 0;
    unsigned long vTimeout = 0xFFFF;
    unsigned int ix;

    // shift in 11 bits from MSB to LSB
    for(ix = 0; ix < 11; ix++)
    {
        while((*mfpgpdr & 0x40) && vTimeout) vTimeout--; // wait for the clock to go LOW

        if (vTimeout)
            bData += ((*mfpgpdr & 0x01) << ix);    // shift in a bit

        while(!(*mfpgpdr & 0x40) && vTimeout) vTimeout--;  // wait here while the clock is still low
    }

    if (!vTimeout)
    {
        writeKbdPs2(0xFE);  // Resend Last Byte
        bData = 0xFE;
    }

    return (unsigned char)bData;
}

//-----------------------------------------------------------------------------
// write a byte to the keyboard
//-----------------------------------------------------------------------------
void writeKbdPs2(unsigned char pData)
{
    int ix, pParity;
    unsigned long vTimeout = 0xFFFF;
    unsigned char pDataTemp;

    // bring the clock low to stop mouse from communicating
    *mfpddr |= 0x40;   // I6 - Output
    *mfpgpdr &= 0xBF;  // Seta Clk = 0
    delayus(200); // wait for mouse 200us

    // bring data low to tell mouse that the host wants to communicate
    *mfpddr |= 0x01;   // I0 - Output
    *mfpgpdr &= 0xFE;  // Seta Data = 0
    delayus(50);  // wait for mouse 50us

    // release control of clock by putting it back to a input
    *mfpddr &= 0xBF;   // I6 - Input

    while((*mfpgpdr & 0x40) && vTimeout) vTimeout--;     // wait for clk to go low

    *Ps2PairChk = 0x00;

    for (ix = 0; ix < 8; ix++)
    {
        pDataTemp = ((pData & (1 << ix)) >> ix);
        *mfpgpdr = ((*mfpgpdr & 0xFE) | pDataTemp);
        *Ps2PairChk += pDataTemp;         // count if 1 for pairity check later
        while(!(*mfpgpdr & 0x40) && vTimeout) vTimeout--;  // wait for clk to go high
        while((*mfpgpdr & 0x40) && vTimeout) vTimeout--;     // wait for clk to go low
    }

    // Send parity data
    if(*Ps2PairChk % 2 == 0)
        *mfpgpdr |= 0x01;
    else
        *mfpgpdr &= 0xFE;

    *vStatusMse = *mfpgpdr;

    while(!(*mfpgpdr & 0x40) && vTimeout) vTimeout--;  // wait for clk to go high
    while((*mfpgpdr & 0x40) && vTimeout) vTimeout--;     // wait for clk to go low

    // release control of data
    *mfpddr &= 0xFE;   // I0 - Input

    while((*mfpgpdr & 0x01) && vTimeout) vTimeout--; // wait for data to go low
    while((*mfpgpdr & 0x40) && vTimeout) vTimeout--; // wait for clk to go low

    while(!(*mfpgpdr & 0x01) && vTimeout) vTimeout--; // wait for data to go high again
    while(!(*mfpgpdr & 0x40) && vTimeout) vTimeout--;  // wait for clk to go high again
}
#endif

#ifdef __MOUSEPS2__
//-----------------------------------------------------------------------------
// write a byte to the mouse
//-----------------------------------------------------------------------------
void writeMsePs2(unsigned char pData)
{
    int ix, pParity;
    unsigned long vTimeout = 0xFFFF;
    unsigned char pDataTemp;

    // bring the clock low to stop mouse from communicating
    *mfpddr |= 0x80;   // I7 - Output
    *mfpgpdr &= 0x7F;  // Seta Clk = 0
    delayus(200); // wait for mouse 200us

    // bring data low to tell mouse that the host wants to communicate
    *mfpddr |= 0x02;   // I1 - Output
    *mfpgpdr &= 0xFD;  // Seta Data = 0
    delayus(50);  // wait for mouse 50us

    // release control of clock by putting it back to a input
    *mfpddr &= 0x7F;   // I7 - Input

    while((*mfpgpdr & 0x80) && vTimeout) vTimeout--;     // wait for clk to go low

    *Ps2PairChk = 0x00;

    for (ix = 0; ix < 8; ix++)
    {
        pDataTemp = ((pData & (1 << ix)) >> ix);
        *mfpgpdr = (*mfpgpdr & 0xFD) | (pDataTemp << 1);
        //*(MseMovBuffer + 8 + ix) = pDataTemp;
        *Ps2PairChk += pDataTemp;         // count if 1 for pairity check later
        while(!(*mfpgpdr & 0x80) && vTimeout) vTimeout--;  // wait for clk to go high
        while((*mfpgpdr & 0x80) && vTimeout) vTimeout--;     // wait for clk to go low
    }

    // Send parity data
    if(*Ps2PairChk % 2 == 0)
        *mfpgpdr |= 0x02;
    else
        *mfpgpdr &= 0xFD;

    *vStatusMse = *mfpgpdr;

    while(!(*mfpgpdr & 0x80) && vTimeout) vTimeout--;  // wait for clk to go high
    while((*mfpgpdr & 0x80) && vTimeout) vTimeout--;     // wait for clk to go low

    // release control of data
    *mfpddr &= 0xFD;   // I1 - Input

    while((*mfpgpdr & 0x02) && vTimeout) vTimeout--; // wait for data to go low
    while((*mfpgpdr & 0x80) && vTimeout) vTimeout--; // wait for clk to go low

    while(!(*mfpgpdr & 0x02) && vTimeout) vTimeout--; // wait for data to go high again
    while(!(*mfpgpdr & 0x80) && vTimeout) vTimeout--;  // wait for clk to go high again
}

//-----------------------------------------------------------------------------
// read a byte from Mouse
//-----------------------------------------------------------------------------
unsigned char readMsePs2 (void)
{
    unsigned long bData = 0;
    unsigned long vTimeout = 0xFFFF;
    unsigned int ix;

    // shift in 11 bits from MSB to LSB
    for(ix = 0; ix < 11; ix++)
    {
        while((*mfpgpdr & 0x80) && vTimeout) vTimeout--; // wait for the clock to go LOW

        if (vTimeout)
            bData += (((*mfpgpdr & 0x02) >> 1) << ix);    // shift in a bit

        while(!(*mfpgpdr & 0x80) && vTimeout) vTimeout--;  // wait here while the clock is still low
    }

    bData = (bData >> 1) & 0x00FF; // shift out the start bit

    return (unsigned char)bData;
}


//-----------------------------------------------------------------------------
// clear input clk/data from Mouse
//-----------------------------------------------------------------------------
void flushMsePs2 (void)
{
    unsigned long bData = 0;
    unsigned long vTimeout = 0xFFF;
    unsigned int ix;

    for(ix = 0; ix < 11; ix++)
    {
        while(!(*mfpgpdr & 0x80) && vTimeout) vTimeout--;  // wait here while the clock is still low
    }
}
#endif

//-----------------------------------------------------------------------------
void funcZeroesLeft(unsigned char* buffer, unsigned char vTam)
{
    unsigned char vbuffer[20];
    unsigned char pad;
    unsigned char len;

    if (buffer == NULL)
        return;

    if (vTam == 0)
        return;

    if (vTam >= sizeof(vbuffer))
        vTam = (unsigned char)(sizeof(vbuffer) - 1);

    strncpy((char *)vbuffer, (char *)buffer, sizeof(vbuffer) - 1);
    vbuffer[sizeof(vbuffer) - 1] = '\0';

    len = (unsigned char)strlen((char *)vbuffer);
    if (len >= vTam)
    {
        strncpy((char *)buffer, (char *)vbuffer, vTam);
        buffer[vTam] = '\0';
        return;
    }

    pad = (unsigned char)(vTam - len);
    memset(buffer, '0', pad);
    strcpy((char *)(buffer + pad), (char *)vbuffer);
}

//-----------------------------------------------------------------------------
void funcErrorBusAddr(void)
{
    unsigned int ix = 0, iz;
    unsigned char sqtdtam[20];
    unsigned short vOP = 0;

    videoCursorPosColX = 0;
    videoCursorPosRowY = 0;
    videoScroll = 1;       // Ativo
    videoScrollDir = 1;    // Pra Cima
    videoCursorBlink = 1;
    videoCursorShow = 0;
    vdpMaxCols = 39;
    vdpMaxRows = 23;
    vdp_init_textmode(VDP_WHITE, VDP_DARK_RED);

    clearScr();
    printChar(218,1);
    for (ix = 0; ix < 36; ix++)
        printChar(196,1);
    printChar(191,1);
    printText(" \r\n");
    printChar(179,1);
    printText("          EXCEPTION OCCURRED        ");
    printChar(179,1);
    printText(" \r\n");
    printChar(195,1);
    for (ix = 0; ix < 36; ix++)
        printChar(196,1);
    printChar(180,1);
    printText(" \r\n");

    vOP = *errorBufferAddrBus;

    switch (vOP)
    {
        case 0x0000:
            printChar(179,1);
            printText("      BUS ERROR / ADDRESS ERROR     ");
            printChar(179,1);
            printText(" \r\n");
            break;
        case 0x0001:
            printChar(179,1);
            printText("         ILLEGAL INSTRUCTION        ");
            printChar(179,1);
            printText(" \r\n");
            break;
        case 0x0002:
            printChar(179,1);
            printText("             ZERO DIVIDE            ");
            printChar(179,1);
            printText(" \r\n");
            break;
        case 0x0003:
            printChar(179,1);
            printText("           CHK INSTRUCTION          ");
            printChar(179,1);
            printText(" \r\n");
            break;
        case 0x0004:
            printChar(179,1);
            printText("                TRAPV               ");
            printChar(179,1);
            printText(" \r\n");
            break;
        case 0x0005:
            printChar(179,1);
            printText("         PRIVILEGE VIOLATION        ");
            printChar(179,1);
            printText(" \r\n");
            break;
        default:
            itoa(errorBufferAddrBus,sqtdtam,16);
            funcZeroesLeft(&sqtdtam, 8);
            printText(sqtdtam);
            printText(" : ");
            itoa(vOP,sqtdtam,16);
            funcZeroesLeft(&sqtdtam, 4);
            printText(sqtdtam);
            printText("\r\n");
            break;
    }
    printChar(195,1);
    for (ix = 0; ix < 36; ix++)
        printChar(196,1);
    printChar(180,1);
    printText(" \r\n");

    ix++;

    // Mostra Registradores: 2 words by register
    printChar(179,1);
    printText(" D0       D1       D2       D3      ");
    printChar(179,1);
    printText(" \r\n");

    printChar(179,1);
    printChar(' ',1);
    for (iz = 0; iz < 4; iz++)  // Mostra d0-d3
    {
        itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
        funcZeroesLeft(&sqtdtam, 4);
        printText(sqtdtam);
        ix++;
        itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
        funcZeroesLeft(&sqtdtam, 4);
        printText(sqtdtam);
        ix++;

        if (iz < 3)
            printText(" ");
    }
    printChar(179,1);
    printText("\r\n");

    printChar(179,1);
    printText(" D4       D5       D6       D7      ");
    printChar(179,1);
    printText(" \r\n");

    printChar(179,1);
    printChar(' ',1);
    for (iz = 0; iz < 4; iz++)  // Mostra d4-d7
    {
        itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
        funcZeroesLeft(&sqtdtam, 4);
        printText(sqtdtam);
        ix++;
        itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
        funcZeroesLeft(&sqtdtam, 4);
        printText(sqtdtam);
        ix++;

        if (iz < 3)
            printText(" ");
    }
    printChar(179,1);
    printText("\r\n");

    printChar(179,1);
    printText(" A0       A1       A2       A3      ");
    printChar(179,1);
    printText(" \r\n");

    printChar(179,1);
    printChar(' ',1);
    for (iz = 0; iz < 4; iz++)  // Mostra d0-d3
    {
        itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
        funcZeroesLeft(&sqtdtam, 4);
        printText(sqtdtam);
        ix++;
        itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
        funcZeroesLeft(&sqtdtam, 4);
        printText(sqtdtam);
        ix++;

        if (iz < 3)
            printText(" ");
    }
    printChar(179,1);
    printText(" \r\n");

    printChar(179,1);
    printText(" A4       A5       A6               ");
    printChar(179,1);
    printText(" \r\n");

    printChar(179,1);
    printChar(' ',1);
    for (iz = 0; iz < 3; iz++)  // Mostra d4-d7
    {
        itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
        funcZeroesLeft(&sqtdtam, 4);
        printText(sqtdtam);
        ix++;
        itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
        funcZeroesLeft(&sqtdtam, 4);
        printText(sqtdtam);
        ix++;

        printText(" ");
    }
    printText("        ");
    printChar(179,1);
    printText("\r\n");
    printChar(179,1);
    printText("                                    ");
    printChar(179,1);
    printText("\r\n");

    printChar(179,1);
    printText(" SR   PC       OffSet Special_Word  ");
    printChar(179,1);
    printText(" \r\n");

    printChar(179,1);
    printChar(' ',1);

    // Mostra SR: 1 word
    itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
    funcZeroesLeft(&sqtdtam, 4);
    printText(sqtdtam);
    ix++;
    printText(" ");

    // Mostra PC to Return: 2 words
    itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
    funcZeroesLeft(&sqtdtam, 4);
    printText(sqtdtam);
    ix++;
    itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
    funcZeroesLeft(&sqtdtam, 4);
    printText(sqtdtam);
    ix++;
    printText(" ");

    // Mostra Vector offset: 1 word
    itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
    funcZeroesLeft(&sqtdtam, 4);
    printText(sqtdtam);
    ix++;
    printText("   ");

    // Mostra Special Status Word: 1 word
    itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
    funcZeroesLeft(&sqtdtam, 4);
    printText(sqtdtam);
    ix++;
    printText("          ");
    printChar(179,1);
    printText("\r\n");
    printChar(179,1);
    printText("                                    ");
    printChar(179,1);
    printText("\r\n");

    printChar(179,1);
    printText(" FaultAddr OutB InB  Instr.InB      ");
    printChar(179,1);
    printText(" \r\n");

    printChar(179,1);
    printChar(' ',1);

    // Mostra Fault Address: 2 words
    itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
    funcZeroesLeft(&sqtdtam, 4);
    printText(sqtdtam);
    ix++;
    itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
    funcZeroesLeft(&sqtdtam, 4);
    printText(sqtdtam);
    ix++;
    printText("  ");

    // unused: 1 word
    ix++;

    // Mostra output buffer: 1 word
    itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
    funcZeroesLeft(&sqtdtam, 4);
    printText(sqtdtam);
    ix++;
    printText(" ");

    // unused: 1 word
    ix++;

    // Mostra input buffer: 1 word
    itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
    funcZeroesLeft(&sqtdtam, 4);
    printText(sqtdtam);
    ix++;
    printText(" ");

    // unused: 1 word
    ix++;
    // Mostra instruction input buffer: 1 word
    itoa(*(errorBufferAddrBus + ix),sqtdtam,16);
    funcZeroesLeft(&sqtdtam, 4);
    printText(sqtdtam);
    ix++;
    printText("           ");
    printChar(179,1);
    printText("\r\n");

    // Halt
    printChar(195,1);
    for (ix = 0; ix < 36; ix++)
        printChar(196,1);
    printChar(180,1);
    printText(" \r\n");
    printChar(179,1);
    printText("            SYSTEM HALTED           ");
    printChar(179,1);
    printText(" \r\n");
    printChar(192,1);
    for (ix = 0; ix < 36; ix++)
        printChar(196,1);
    printChar(217,1);
    printText(" \r\n");
    for(;;);
}