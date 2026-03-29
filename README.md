# MMSJ320v2

## Especificacoes

- CPU: 68000
- RAM de sistema: 256 KB
- RAM de usuario: 1 MB
- Compilador: `m68k-elf-gcc`

## Mapa de Memoria

```text
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
```

---

## Enderecos de Perifericos

### `00200001h` e `00200003h`

- DISK Arduino UNO (Temp)
- A1 = 0: r/w 4 bits LSB
- A1 = 1: r/w 4 bits MSB

### `00400020h` a `0040003Fh`

- MFP MC68901p (Cristal de 2.4576 MHz)
- SERIAL 9600, 8, 1, n
- TECLADO (PC-AT/Mouse - PS/2)
- Controle de Interrupcoes e PS/2

### `00400040h` a `00400043h`

- VIDEO TMS9118 (16 KB VRAM)
- `00400041h`: Data Mode
- `00400043h`: Register / Adress Mode
