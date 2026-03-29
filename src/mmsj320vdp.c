#include "mmsj320vdp.h"

unsigned char *vvdgd = 0x00400041; // VDP TMS9118 Data Mode
unsigned char *vvdgc = 0x00400043; // VDP TMS9118 Registers/Address Mode

unsigned char fgcolor;    // Buffer da VRAM 6KB onde o computador vai trabalhar e a cada interrupcao, sera enviado a VRAM
unsigned char bgcolor; // QTD de BYTES no eixo X (COLS) a srem enviados pra VRAM
unsigned char videoBufferQtdY; // QTD de BYTES no eixo Y (Rows) a serem enviados pra VRAM
unsigned int color_table;
unsigned int sprite_attribute_table; // Contador da quantidade ja enviada acumulada. A cada 2KB, ele esera uma nova interrupcao
unsigned long videoFontes; // Ponteir para a posicao onde estão as Fontes para vga
unsigned short videoCursorPosCol;  // Posicao atual caracter do cursor na coluna (0 a 31)
unsigned short videoCursorPosRow;  // Posical atual caracter do cursor na linha (0 a 23)
unsigned short videoCursorPosColX;  // Posicao atual do cursor na coluna (0 a 255)
unsigned short videoCursorPosRowY;  // Posical atual do cursor na linha (0 a 191)
unsigned char videoCursorBlink; // Cursor piscante (1 = sim, 0 = nao)
unsigned char videoCursorShow;  // Mostrar Cursor  (1 = sim, 0 = nao)
unsigned int name_table;
unsigned char vdp_mode; // Modo de video 0 = caracter (32 x 24), 1 = grafico (256 x 192)
unsigned char videoScroll;    // Define se quando a linha passar de 23 (0-23), a tela será rolada (0-nao, 1-sim)
unsigned char videoScrollDir;    // Define a direcao do scroll (1-up, 2-down, 3-left, 4-right)
unsigned int pattern_table;
unsigned char sprite_size_sel;
unsigned char vdpMaxCols; // max col number
unsigned char vdpMaxRows;
unsigned char fgcolorAnt; // Cor Anterior de Frente
unsigned char bgcolorAnt; // Cor Anterior de Fundo
unsigned int sprite_pattern_table;
unsigned int color_table_size;

//-----------------------------------------------------------------------------
// VDP Functions
//-----------------------------------------------------------------------------
void setRegister(unsigned char registerIndex, unsigned char value)
{
    *vvdgc = value;
    *vvdgc = (0x80 | registerIndex);
}

//-----------------------------------------------------------------------------
unsigned char read_status_reg(void)
{
    unsigned char memByte;

    memByte = *vvdgc;

    return memByte;
}

//-----------------------------------------------------------------------------
void setWriteAddress(unsigned int address)
{
    *vvdgc = (unsigned char)(address & 0xff);
    *vvdgc = (unsigned char)(0x40 | (address >> 8) & 0x3f);
}

//-----------------------------------------------------------------------------
void setReadAddress(unsigned int address)
{
    *vvdgc = (unsigned char)(address & 0xff);
    *vvdgc = (unsigned char)((address >> 8) & 0x3f);
}

//-----------------------------------------------------------------------------
int vdp_init(unsigned char mode, unsigned char color, unsigned char big_sprites, unsigned char magnify)
{
    unsigned int i, j;
    unsigned char *tempFontes = videoFontes;

    vdp_mode = mode;
    sprite_size_sel = big_sprites;

    // Clear Ram
    setWriteAddress(0x0);
    for (i = 0; i < 0x3FFF; i++)
        *vvdgd = 0;

    switch (mode)
    {
        case VDP_MODE_G1:
            setRegister(0, 0x00);
            setRegister(1, 0xC0 | (big_sprites << 1) | magnify); // Ram size 16k, activate video output
            setRegister(2, 0x05); // Name table at 0x1400
            setRegister(3, 0x80); // Color, start at 0x2000
            setRegister(4, 0x01); // Pattern generator start at 0x800
            setRegister(5, 0x20); // Sprite attriutes start at 0x1000
            setRegister(6, 0x00); // Sprite pattern table at 0x000
            sprite_pattern_table = 0;
            pattern_table = 0x800;
            sprite_attribute_table = 0x1000;
            name_table = 0x1400;
            color_table = 0x2000;
            color_table_size = 32;
            // Initialize pattern table with ASCII patterns
            setWriteAddress(pattern_table + 0x100);
            for (i = 0; i < 1784; i++)  // era 768
            {
                tempFontes = videoFontes + i;
                *vvdgd = *tempFontes;
            }
            break;

        case VDP_MODE_G2:
            setRegister(0, 0x02);
            setRegister(1, 0xC0 | (big_sprites << 1) | magnify); // Ram size 16k, Disable Int, 16x16 Sprites, mag off, activate video output
            setRegister(2, 0x0E); // Name table at 0x3800
            setRegister(3, 0xFF); // Color, start at 0x2000             // segundo manual, deve ser 7F para 0x0000 ou FF para 0x2000
            setRegister(4, 0x03); // Pattern generator start at 0x000   // segundo manual, deve ser 03 para 0x0000 ou 07 para 0x2000
            setRegister(5, 0x76); // Sprite attriutes start at 0x3800
            setRegister(6, 0x03); // Sprite pattern table at 0x1800
            pattern_table = 0x00;
            sprite_pattern_table = 0x1800;
            color_table = 0x2000;
            name_table = 0x3800;
            sprite_attribute_table = 0x3B00;
            color_table_size = 0x1800;
            vdpMaxCols = 255;
            vdpMaxRows = 191;
            setWriteAddress(name_table);
            for (i = 0; i < 768; i++)  // era 768
                *vvdgd = (unsigned char)(i & 0xFF);
            break;

        case VDP_MODE_MULTICOLOR:
            setRegister(0, 0x00);
            setRegister(1, 0xC8 | (big_sprites << 1) | magnify); // Ram size 16k, Multicolor
            setRegister(2, 0x05); // Name table at 0x1400
            // setRegister(3, 0xFF); // Color table not available
            setRegister(4, 0x01); // Pattern table start at 0x800
            setRegister(5, 0x76); // Sprite Attribute table at 0x1000
            setRegister(6, 0x03); // Sprites Pattern Table at 0x0
            pattern_table = 0x800;
            name_table = 0x1400;
            vdpMaxCols = 63;
            vdpMaxRows = 47;
            setWriteAddress(name_table); // Init name table
            for (j = 0; j < 24; j++)
                for (i = 0; i < 32; i++)
                    *vvdgd = (i + 32 * (j / 4));

            break;

        case VDP_MODE_TEXT:
            setRegister(0, 0x00);
            setRegister(1, 0xD2); // Ram size 16k, Disable Int
            setRegister(2, 0x02); // Name table at 0x800
            setRegister(4, 0x00); // Pattern table start at 0x0
            pattern_table = 0x00;
            name_table = 0x800;
            vdpMaxCols = 39;
            vdpMaxRows = 23;
            setWriteAddress(pattern_table + 0x100);
            for (i = 0; i < 1784; i++)  // era 768
            {
                tempFontes = videoFontes + i;
                *vvdgd = *tempFontes;
            }
            vdp_textcolor(VDP_WHITE, VDP_BLACK);
            break;
        default:
            return VDP_ERROR; // Unsupported mode
    }

    setRegister(7, color);
    return VDP_OK;
}

//-----------------------------------------------------------------------------
void vdp_colorize(unsigned char fg, unsigned char bg)
{
    unsigned int name_offset = videoCursorPosRowY * (vdpMaxCols + 1) + videoCursorPosColX; // Position in name table
    unsigned int color_offset = name_offset << 3;                      // Offset of pattern in pattern table
    unsigned int i;

    if (vdp_mode != VDP_MODE_G2)
        return;

    setWriteAddress(color_table + color_offset);

    for (i = 0; i < 8; i++)
        *vvdgd = ((fg << 4) + bg);
}

//-----------------------------------------------------------------------------
void vdp_plot_hires(unsigned char x, unsigned char y, unsigned char color1, unsigned char color2)
{
    unsigned int offset, posX, posY, modY;
    unsigned char pixel;
    unsigned char color;
    unsigned char sqtdtam[10];

    posX = (int)(8 * (x / 8));
    posY = (int)(256 * (y / 8));
    modY = (int)(y % 8);

    offset = posX + modY + posY;

    setReadAddress(pattern_table + offset);
    setReadAddress(pattern_table + offset);
    pixel = *vvdgd;
    setReadAddress(color_table + offset);
    setReadAddress(color_table + offset);
    color = *vvdgd;

    if(color1 != 0x00)
    {
        pixel |= 0x80 >> (x % 8); //Set a "1"
        color = (color & 0x0F) | (color1 << 4);
    }
    else
    {
        pixel &= ~(0x80 >> (x % 8)); //Set bit as "0"
        color = (color & 0xF0) | (color2 & 0x0F);
    }

    setWriteAddress(pattern_table + offset);
    *vvdgd = (pixel);
    setWriteAddress(color_table + offset);
    *vvdgd = (color);
}

//-----------------------------------------------------------------------------
void vdp_plot_color(unsigned char x, unsigned char y, unsigned char color)
{
    unsigned int addr = pattern_table + 8 * (x / 2) + y % 8 + 256 * (y / 8);
    unsigned char dot = *vvdgd;
    unsigned int offset = 8 * (x / 2) + y % 8 + 256 * (y / 8);
    unsigned char color_ = *vvdgd;

    if (vdp_mode == VDP_MODE_MULTICOLOR)
    {
        setReadAddress(addr);
        setWriteAddress(addr);

        if (x & 1) // Odd columns
            *vvdgd = ((dot & 0xF0) + (color & 0x0f));
        else
            *vvdgd = ((dot & 0x0F) + (color << 4));
    }
    else if (vdp_mode == VDP_MODE_G2)
    {
        // Draw bitmap
        setReadAddress(color_table + offset);

        if((x & 1) == 0) //Even
        {
            color_ &= 0x0F;
            color_ |= (color << 4);
        }
        else
        {
            color_ &= 0xF0;
            color_ |= color & 0x0F;
        }

        setWriteAddress(pattern_table + offset);
        *vvdgd = (0xF0);
        setWriteAddress(color_table + offset);
        *vvdgd = (color_);
        // Colorize
    }
}

//-----------------------------------------------------------------------------
void vdp_set_sprite_pattern(unsigned char number, const unsigned char *sprite)
{
    unsigned char i;

    if(sprite_size_sel)
    {
        setWriteAddress(sprite_pattern_table + (32 * number));
        for (i = 0; i<32; i++)
        {
            *vvdgd = (sprite[i]);
        }
    }
    else
    {
        setWriteAddress(sprite_pattern_table + (8 * number));
        for (i = 0; i<8; i++)
        {
            *vvdgd = (sprite[i]);
        }
    }
}

//-----------------------------------------------------------------------------
void vdp_sprite_color(unsigned int addr, unsigned char color)
{
    unsigned char ecclr;

    setReadAddress(addr + 3);
    ecclr = *vvdgd & 0x80 | (color & 0x0F);
    setWriteAddress(addr + 3);
    *vvdgd = (ecclr);
}

//-----------------------------------------------------------------------------
Sprite_attributes vdp_sprite_get_attributes(unsigned int addr)
{
    Sprite_attributes attrs;
    setReadAddress(addr);
    attrs.y = *vvdgd;
    attrs.x = *vvdgd;
    attrs.name_ptr = *vvdgd;
    attrs.ecclr = *vvdgd;
    return attrs;
}

//-----------------------------------------------------------------------------
Sprite_attributes vdp_sprite_get_position(unsigned int addr)
{
    unsigned char x;
    unsigned char eccr;
    unsigned char vdumbread;
    Sprite_attributes attrs;
    setReadAddress(addr);

    attrs.y = *vvdgd;

    x = *vvdgd;

    vdumbread = *vvdgd;

    eccr = *vvdgd;

    attrs.x = eccr & 0x80 ? x : x+32;
    return attrs;
}

//-----------------------------------------------------------------------------
unsigned int vdp_sprite_init(unsigned char name, unsigned char priority, unsigned char color)
{
    unsigned int addr = sprite_attribute_table + 4*priority;
    unsigned char byteVdp;

    while (1)
    {
        setWriteAddress(addr);

        *vvdgd = (0);
        *vvdgd = (0);

        if(sprite_size_sel)
            *vvdgd = (4*name);
        else
            *vvdgd = (4*name);
        *vvdgd = (0x80 | (color & 0xF));

        setReadAddress(addr);
        setReadAddress(addr);

        byteVdp = *vvdgd;
        if (byteVdp != 0)
            continue;

        byteVdp = *vvdgd;
        if (byteVdp != 0)
            continue;

        byteVdp = *vvdgd;
        if (byteVdp != (4*name))
            continue;

        byteVdp = *vvdgd;
        if (byteVdp != (0x80 | (color & 0xF)))
            continue;

        break;
    }

    return addr;
}

//-----------------------------------------------------------------------------
unsigned char vdp_sprite_set_position(unsigned int addr, unsigned int x, unsigned char y)
{
    unsigned char ec, xpos;
    unsigned char color;

    xpos = (unsigned char)(x & 0xFF);
    ec = 0;

    setReadAddress(addr + 3);
    setReadAddress(addr + 3);
    color = *vvdgd & 0x0f;

    setWriteAddress(addr);
    *vvdgd = y;
    setWriteAddress(addr + 1);
    *vvdgd = xpos;

    setWriteAddress(addr + 3);
    *vvdgd = ((ec << 7) | color);
    return read_status_reg();
}

//-----------------------------------------------------------------------------
void vdp_set_bdcolor(unsigned char color)
{
    setRegister(7, color);
}

//-----------------------------------------------------------------------------
void vdp_set_pattern_color(unsigned int index, unsigned char fg, unsigned char bg)
{
    if (vdp_mode == VDP_MODE_G1)
    {
        index &= 31;
    }
    setWriteAddress(color_table + index);
    *vvdgd = ((fg << 4) + bg);
}

//-----------------------------------------------------------------------------
void vdp_set_cursor(unsigned char pcol, unsigned char prow)
{
    if (pcol == 255) //<0
    {
        pcol = vdpMaxCols;
        prow--;
    }
    else if (pcol > vdpMaxCols)
    {
        pcol = 0;
        prow++;
    }

    if (prow == 255)
    {
        prow = vdpMaxRows;
    }
    else if (prow > vdpMaxRows)
    {
        prow = vdpMaxRows; //0;
        geraScroll();
    }

    videoCursorPosColX = pcol;
    videoCursorPosRowY = prow;
}

//-----------------------------------------------------------------------------
VDP_COORD vdp_get_cursor(void)
{
    VDP_COORD cursor;

    cursor.x = videoCursorPosColX;
    cursor.y = videoCursorPosRowY;
    cursor.maxx = vdpMaxCols;
    cursor.maxy = vdpMaxRows;

    return cursor;
}

//-----------------------------------------------------------------------------
VDP_COLOR vdp_get_color(void)
{
    VDP_COLOR cores;

    cores.fg = fgcolor;
    cores.bg = bgcolor;

    return cores;
}

//-----------------------------------------------------------------------------
void vdp_get_cfg(unsigned int *pat, unsigned int *cor)
{
    *pat = pattern_table;
    *cor = color_table;
}

//-----------------------------------------------------------------------------
unsigned long getVideoFontes(void)
{
    return videoFontes;
}

//-----------------------------------------------------------------------------
void vdp_set_cursor_pos(unsigned char direction)
{
    unsigned char pMoveId = 1;

    if (vdp_mode != VDP_MODE_TEXT)
        pMoveId = 8;

    switch (direction)
    {
        case VDP_CSR_UP:
            vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY - pMoveId);
            break;
        case VDP_CSR_DOWN:
            vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY + pMoveId);
            break;
        case VDP_CSR_LEFT:
            vdp_set_cursor(videoCursorPosColX - pMoveId, videoCursorPosRowY);
            break;
        case VDP_CSR_RIGHT:
            vdp_set_cursor(videoCursorPosColX + pMoveId, videoCursorPosRowY);
            break;
    }
}

//-----------------------------------------------------------------------------
void vdp_write(unsigned char chr)
{
    unsigned int name_offset = videoCursorPosRowY * (vdpMaxCols + 1) + videoCursorPosColX; // Position in name table
    unsigned int pattern_offset = name_offset << 3;                    // Offset of pattern in pattern table
    char i, ix;
    unsigned short vAntX, vAntY;
    unsigned char *tempFontes = videoFontes;
    unsigned long vEndFont, vEndPart;

    if (vdp_mode == VDP_MODE_G2)
    {
        vEndPart = chr - 32;
        vEndPart = vEndPart << 3;
        vAntY = videoCursorPosRowY;
        for (i = 0; i < 8; i++)
        {
            vEndFont = videoFontes;
            vEndFont += vEndPart + i;
            tempFontes = vEndFont;
            vAntX = videoCursorPosColX;
            for (ix = 7; ix >=0; ix--)
            {
                vdp_plot_hires(videoCursorPosColX, videoCursorPosRowY, ((*tempFontes >> ix) & 0x01) ? fgcolor : 0, bgcolor);
                videoCursorPosColX = videoCursorPosColX + 1;
            }
            videoCursorPosColX = vAntX;
            videoCursorPosRowY = videoCursorPosRowY + 1;
        }
        videoCursorPosRowY = vAntY;
    }
    else if (vdp_mode == VDP_MODE_MULTICOLOR)
    {
        vEndPart = chr - 32;
        vEndPart = vEndPart << 3;
        vAntY = videoCursorPosRowY;
        for (i = 0; i < 8; i++)
        {
            vEndFont = videoFontes;
            vEndFont += vEndPart + i;
            tempFontes = vEndFont;
            vAntX = videoCursorPosColX;
            for (ix = 7; ix >=0; ix--)
            {
                vdp_plot_color(videoCursorPosColX, videoCursorPosRowY, ((*tempFontes >> ix) & 0x01) ? fgcolor : bgcolor);
                videoCursorPosColX = videoCursorPosColX + 1;
            }
            videoCursorPosColX = vAntX;
            videoCursorPosRowY = videoCursorPosRowY + 1;
        }
        videoCursorPosRowY = vAntY;
    }
    else // G1 and text mode
    {
        setWriteAddress(name_table + name_offset);
        *vvdgd = (chr);
    }
}

//-----------------------------------------------------------------------------
void vdp_textcolor(unsigned char fg, unsigned char bg)
{
    fgcolor = fg;
    bgcolor = bg;
    if (vdp_mode == VDP_MODE_TEXT)
        setRegister(7, (fg << 4) + bg);
}

//-----------------------------------------------------------------------------
int vdp_init_textmode(unsigned char fg, unsigned char bg)
{
    unsigned int vret;
    fgcolor = fg;
    bgcolor = bg;
    vret = vdp_init(VDP_MODE_TEXT, (fgcolor<<4) | (bgcolor & 0x0f), 0, 0);
    return vret;
}

//-----------------------------------------------------------------------------
int vdp_init_g1(unsigned char fg, unsigned char bg)
{
    unsigned int vret;
    fgcolor = fg;
    bgcolor = bg;
    vret = vdp_init(VDP_MODE_G1, (fgcolor<<4) | (bgcolor & 0x0f), 0, 0);
    return vret;
}

//-----------------------------------------------------------------------------
int vdp_init_g2(unsigned char big_sprites, unsigned char scale_sprites) // 1, false
{
    unsigned int vret;
    vret = vdp_init(VDP_MODE_G2, 0x0, big_sprites, scale_sprites);
    return vret;
}

//-----------------------------------------------------------------------------
int vdp_init_multicolor(void)
{
    unsigned int vret;
    vret = vdp_init(VDP_MODE_MULTICOLOR, 0, 0, 0);
    return vret;
}

//-----------------------------------------------------------------------------
char vdp_read_color_pixel(unsigned char x, unsigned char y)
{
    char vRetColor = -1;
    unsigned int addr = 0;

    if (vdp_mode == VDP_MODE_MULTICOLOR)
    {
        addr = pattern_table + 8 * (x / 2) + y % 8 + 256 * (y / 8);

        setReadAddress(addr);
        setReadAddress(addr);

        if (x & 1) // Odd columns
            vRetColor = (*vvdgd & 0x0f);
        else
            vRetColor = (*vvdgd >> 4);
    }

    return vRetColor;
}

//-----------------------------------------------------------------------------
void geraScroll(void)
{
    unsigned int name_offset = 0; // Position in name table
    unsigned int i, j;
    unsigned char chr[40];
    unsigned char vdumbread;

    if (vdp_mode == VDP_MODE_TEXT)
    {
        for (i = 1; i < 24; i++)
        {
            // Ler Linha
            name_offset = (i * (vdpMaxCols + 1)); // Position in name table
            setWriteAddress((name_table + name_offset));

            vdumbread = *vvdgd;

            for (j = 0; j < 40; j++)
            {
                chr[j] = *vvdgd;
            }

            // Escrever na linha anterior
            name_offset = ((i - 1) * (vdpMaxCols + 1)); // Position in name table
            setWriteAddress((name_table + name_offset));

            for (j = 0; j < 40; j++)
            {
                *vvdgd = chr[j];
            }
        }

        // Apaga Ultima Linha
        name_offset = (23 * (vdpMaxCols + 1)); // Position in name table
        setWriteAddress((name_table + name_offset));

        for (j = 0; j < 40; j++)
        {
            *vvdgd = 0x00;
        }
    }
}


//-----------------------------------------------------------------------------
void clearScr(void)
{
    unsigned int i;

    // Ler Linha
    setWriteAddress(name_table);

    for (i = 0; i < 960; i++)
        *vvdgd = 0x00;

    videoCursorPosColX = 0;
    videoCursorPosRowY = 0;

    #ifdef __MON_SERIAL_VDG__
        writeLongSerial("\r\n\r\n\0");
        writeLongSerial("\033[2J");   // Clear Screeen
        writeLongSerial("\033[H");    // Cursor to Upper left corner
    #endif
}

//-----------------------------------------------------------------------------
void printChar(unsigned char pchr, unsigned char pmove)
{
    switch (pchr)
    {
        case 0x0A:  // LF
            videoCursorPosRowY = videoCursorPosRowY + 1;
            if (videoCursorPosRowY == 24)
            {
                videoCursorPosRowY = 23;
                geraScroll();
            }
            vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY);
            break;
        case 0x0D:  // CR
            videoCursorPosColX = 0;
            vdp_set_cursor(0, videoCursorPosRowY);
            break;
        case 0x08:  // BackSpace
            if (videoCursorPosColX > 0)
            {
                videoCursorPosColX = videoCursorPosColX - 1;
                vdp_set_cursor(videoCursorPosColX, videoCursorPosRowY);
            }
            break;
        case 0xFF:  // Cursor
            if (videoCursorShow)
                vdp_write(0xFE);
            else
                vdp_write(0x20);
            break;
        default:
            vdp_write(pchr);

            if (vdp_mode == VDP_MODE_TEXT)
                vdp_colorize(fgcolor, bgcolor);

            if (pmove)
            {
                vdp_set_cursor_pos(VDP_CSR_RIGHT);

                if (vdp_mode == VDP_MODE_TEXT && videoCursorPosRowY == 24)
                {
                    videoCursorPosRowY = 23;
                    geraScroll();
                }
            }
    }
}

//-----------------------------------------------------------------------------
void printText(unsigned char *msg)
{
    while (*msg)
    {
        printChar(*msg++, 1);
    }
}