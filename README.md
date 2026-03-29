MMSJ320v2

CPU 68000
256KB Ram sistema
1MB Ram Usuario

Using Compiler m68k-elf-gcc

 Mapa de Memoria
 ---------------

     SLOT 0                          SLOT 1
 +-------------+ 000000h
 |   EEPROM    |
 |   512KB     |
 |   (BIOS)    | 07FFFFh
 +-------------+ 080000h
 |    LIVRE    | 1FFFFFh
 +-------------+ 200000h
 |             |
 |  EXPANSAO   |
 |             | 3FFFFFh
 +-------------+ 400000h
 |             |
 | PERIFERICOS |
 |             | 5FFFFFh
 +-------------+ 600000h
 |  RAM 256KB  |
 |  BUFFER E   |
 |  SISTEMA    | 63FFFFh
 +-------------+ 640000h
 |    LIVRE    | 7FFFFFh
 +-------------+ 800000h
 |             |
 |   ATUAL     |
 |    RAM      |
 |  USUARIO    |
 |    1MB      | 8FFFFFh
 +-------------+ 900000h
 |             |
 |             |
 |    RAM      |
 |  USUARIO    |
 |    7MB      |
 |             |
 |             |
 |             |
 |             |
 |             |
 |             |
 |             |
 |             |
 +-------------+ FFFFFFh
--------------------------------------------------------------------------------

 Enderecos de Perifericos

 00200001h e 00200003 - DISK Arduino UNO (Temp)
                        - A1 = 0: r/w 4 bits LSB
                        - A1 = 1: r/w 4 bits MSB
 00400020h a 0040003F - MFP MC68901p - Cristal de 2.4576MHz
                        - SERIAL 9600, 8, 1, n
                        - TECLADO (PC-AT/Mouse - PS/2)
                        - Controle de Interrupcoes e PS/2
 00400040h a 00400043 - VIDEO TMS9118 (16KB VRAM):
             00400041 - Data Mode
             00400043 - Register / Adress Mode
