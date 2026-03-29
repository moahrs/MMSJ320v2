#ifndef   MMSJ_320_VDP_H
#define   MMSJ_320_VDP_H

/** Struct
 * @brief 4-Byte record defining sprite attributes
 */
typedef struct 
{
    unsigned char x; //Sprite X position
    unsigned char y; //Sprite Y position
    unsigned char name_ptr; //Sprite name in pattern table
    unsigned char ecclr; //Bit 7: Early clock bit, bit 3:0 color
} Sprite_attributes;

typedef struct
{
    unsigned char x;
    unsigned char y;
    unsigned char maxx;
    unsigned char maxy;
} VDP_COORD;

typedef struct
{
    unsigned char fg;
    unsigned char bg;
} VDP_COLOR;

typedef struct
{
    unsigned int pattern;
    unsigned int color;
} VDP_MODE_SETUP;

extern unsigned char *vvdgd; // VDP TMS9118 Data Mode
extern unsigned char *vvdgc; // VDP TMS9118 Registers/Address Mode

extern unsigned char fgcolor;    // Buffer da VRAM 6KB onde o computador vai trabalhar e a cada interrupcao, sera enviado a VRAM
extern unsigned char bgcolor; // QTD de BYTES no eixo X (COLS) a srem enviados pra VRAM
extern unsigned char videoBufferQtdY; // QTD de BYTES no eixo Y (Rows) a serem enviados pra VRAM
extern unsigned int color_table;
extern unsigned int sprite_attribute_table; // Contador da quantidade ja enviada acumulada. A cada 2KB, ele esera uma nova interrupcao
extern unsigned long videoFontes; // Ponteir para a posicao onde estão as Fontes para vga
extern unsigned short videoCursorPosCol;  // Posicao atual caracter do cursor na coluna (0 a 31)
extern unsigned short videoCursorPosRow;  // Posical atual caracter do cursor na linha (0 a 23) 
extern unsigned short videoCursorPosColX;  // Posicao atual do cursor na coluna (0 a 255)
extern unsigned short videoCursorPosRowY;  // Posical atual do cursor na linha (0 a 191)
extern unsigned char videoCursorBlink; // Cursor piscante (1 = sim, 0 = nao)
extern unsigned char videoCursorShow;  // Mostrar Cursor  (1 = sim, 0 = nao)
extern unsigned int name_table;
extern unsigned char vdp_mode; // Modo de video 0 = caracter (32 x 24), 1 = grafico (256 x 192)
extern unsigned char videoScroll;    // Define se quando a linha passar de 23 (0-23), a tela será rolada (0-nao, 1-sim)
extern unsigned char videoScrollDir;    // Define a direcao do scroll (1-up, 2-down, 3-left, 4-right)
extern unsigned int pattern_table;
extern unsigned char sprite_size_sel;
extern unsigned char vdpMaxCols; // max col number
extern unsigned char vdpMaxRows;
extern unsigned char fgcolorAnt; // Cor Anterior de Frente
extern unsigned char bgcolorAnt; // Cor Anterior de Fundo
extern unsigned int sprite_pattern_table;
extern unsigned int color_table_size;

#define NOREPOS_CURSOR             0
#define REPOS_CURSOR               1
#define REPOS_CURSOR_ON_CHANGE     2
#define ADD_POS_SCR                3
#define NOADD_POS_SCR              4

//--- Constantes de Tela
#define VDP_TRANSPARENT 0
#define VDP_BLACK 1
#define VDP_MED_GREEN 2
#define VDP_LIGHT_GREEN 3
#define VDP_DARK_BLUE 4
#define VDP_LIGHT_BLUE 5
#define VDP_DARK_RED 6
#define VDP_CYAN 7
#define VDP_MED_RED 8
#define VDP_LIGHT_RED 9
#define VDP_DARK_YELLOW 10
#define VDP_LIGHT_YELLOW 11
#define VDP_DARK_GREEN 12
#define VDP_MAGENTA 13
#define VDP_GRAY 14
#define VDP_WHITE 15

#define VDP_MODE_G1 0
#define VDP_MODE_G2 1
#define VDP_MODE_MULTICOLOR 2
#define VDP_MODE_TEXT 3

#define VDP_CSR_UP 0
#define VDP_CSR_DOWN 1
#define VDP_CSR_LEFT 2
#define VDP_CSR_RIGHT 3

extern void setRegister(unsigned char registerIndex, unsigned char value);
extern unsigned char read_status_reg(void);
extern void setWriteAddress(unsigned int address);
extern void setReadAddress(unsigned int address);
extern char vdp_read_color_pixel(unsigned char x, unsigned char y);

/**
 * @brief initialize the VDP
 * Not all parameters are useful for all modes. Refer to documentation
 *
 * @param mode VDP_MODE_G1 | VDP_MODE_G2 | VDP_MODE_MULTICOLOR | VDP_MODE_TEXT
 * @param color
 * @param big_sprites true: Use 16x16 sprites false: use 8x8 sprites
 * @param magnify true: Scale sprites up by 2
 * @return int
 */
extern int vdp_init(unsigned char mode, unsigned char color, unsigned char big_sprites, unsigned char magnify);

/**
 * @brief Initializes the VDP in text mode
 *
 * @param fgcolor Text color default: default black
 * @param bgcolor Background color: default white
 * @returns VDP_ERROR | VDP_SUCCESS
 */
extern int vdp_init_textmode(unsigned char fg, unsigned char bg);


/**
 * @brief Initializes the VDP in Graphic Mode 1
 *
 * @param fgcolor Text color default: default black
 * @param bgcolor Background color: default white
 * @returns VDP_ERROR | VDP_SUCCESS
 * @deprecated Not really useful if more than 4k Video ram is available
 */
extern int vdp_init_g1(unsigned char fg, unsigned char bg);

/**
 * @brief Initializes the VDP in Graphic Mode 2
 *
 * @param big_sprites true: use 16x16 sprites false: use 8x8 sprites
 * @param scale_sprites Scale sprites up by 2
 * @returns VDP_ERROR | VDP_SUCCESS
 */
extern int vdp_init_g2(unsigned char big_sprites, unsigned char scale_sprites);

/**
 * @brief Initializes the VDP in 64x48 Multicolor Mode
 *
 * @returns VDP_ERROR | VDP_SUCCESS
 * @deprecated Not really useful if more than 4k Video ram is available
 */
extern int vdp_init_multicolor(void);


/**
 * @brief Set foreground and background color of the pattern at the current cursor position
 * Only available in Graphic mode 2
 * @param fgcolor Foreground color
 * @param bgcolor Background color
 */
extern void vdp_colorize(unsigned char fg, unsigned char bg);

/**
 * @brief Plot a point at position (x,y), where x <= 255. The full resolution of 256 by 192 is available.
 * Only two different colors are possible whithin 8 neighboring pixels
 * VDP_MODE G2 only
 *
 * @param x
 * @param y
 * @param color1 Color of pixel at (x,y). If NULL, plot a pixel with color2
 * @param color2 Color of the pixels not set or color of pixel at (x,y) when color1 == NULL
 */
extern void vdp_plot_hires(unsigned char x, unsigned char y, unsigned char color1, unsigned char color2);

/**
 * @brief Plot a point at position (x,y), where x <= 64. In Graphics mode2, the resolution is 64 by 192 pixels, neighboring pixels can have different colors.
 * In Multicolor  mode, the resolution is 64 by 48 pixels
 *
 * @param x
 * @param y
 * @param color
 */
extern void vdp_plot_color(unsigned char x, unsigned char y, unsigned char color);

/**
 * @brief Print string at current cursor position. These Escape sequences are supported:
 * <ul>
 * <li>\\n (newline) </li>
 * <li>\\r (carriage return)</li>
 * <li>Graphic Mode 2 only: \\033[<fg>;[<bg>]m sets the colors and optionally the background of the subsequent characters </li>
 * </ul>
 * Example: vdp_print("\033[4m Dark blue on transparent background\n\r\033[4;14m dark blue on gray background");
 * @param text Text to print
 */
//void vdp_print(unsigned char *text);

/**
 * @brief Set backdrop color
 *
 * @param color
 */
extern void vdp_set_bdcolor(unsigned char color);

/**
 * @brief Set the color of patterns at the cursor position
 *
 * @param index VDP_MODE_G2: Number of pattern to set the color, VDP_MODE_G1: one of 32 groups of 8 subsequent patterns
 * @param fg Pattern foreground color
 * @param bg Pattern background color
 */
extern void vdp_set_pattern_color(unsigned int index, unsigned char fg, unsigned char bg);

/**
 * @brief Position the cursor at the specified position
 *
 * @param col column
 * @param row row
 */
extern void vdp_set_cursor(unsigned char pcol, unsigned char prow);
extern VDP_COORD vdp_get_cursor(void);
extern VDP_COLOR vdp_get_color(void);
extern void vdp_get_cfg(unsigned int *pat, unsigned int *cor);
extern unsigned long getVideoFontes(void);

/**
 * @brief Move the cursor along the specified direction
 *
 * @param direction {VDP_CSR_UP|VDP_CSR_DOWN|VDP_CSR_LEFT|VDP_CSR_RIGHT}
 */
extern void vdp_set_cursor_pos(unsigned char direction);

/**
 * @brief set foreground and background color of the characters printed after this function has been called.
 * In Text Mode and Graphics Mode 1, all characters are changed. In Graphics Mode 2, the escape sequence \\033[<fg>;<bg>m can be used instead.
 * See vdp_print()
 *
 * @param fg Foreground color
 * @param bg Background color
 */
extern void vdp_textcolor(unsigned char fg, unsigned char bg);

/**
 * @brief Write ASCII character at current cursor position
 *
 * @param chr Pattern at the respective location of the  pattern memory. Graphic Mode 1 and Text Mode: Ascii code of character
 */
extern void vdp_write(unsigned char chr);

/**
 * @brief Write a sprite into the sprite pattern table
 *
 * @param name Reference of sprite 0-255 for 8x8 sprites, 0-63 for 16x16 sprites
 * @param sprite Array with sprite data. Type unsigned char[8] for 8x8 sprites, unsigned char[32] for 16x16 sprites
 */
extern void vdp_set_sprite_pattern(unsigned char number, const unsigned char *sprite);

/**
 * @brief Set the sprite color
 *
 * @param addr Sprite Handle returned by vdp_sprite_init()
 * @param color
 */
extern void vdp_sprite_color(unsigned int addr, unsigned char color);

/**
 * @brief Get the sprite attributes
 *
 * @param addr Sprite Handle returned by vdp_sprite_init()
 * @return Sprite_attributes
 */
extern Sprite_attributes vdp_sprite_get_attributes(unsigned int addr);

/**
 * @brief Get the current position of a sprite
 *
 * @param addr Sprite Handle returned by vdp_sprite_init()
 * @param xpos Reference to x-position
 * @param ypos Reference to y-position
 */
extern Sprite_attributes vdp_sprite_get_position(unsigned int addr);

/**
 * @brief Activate a sprite
 *
 * @param name Number of the sprite as defined in vdp_set_sprite()
 * @param priority 0: Highest priority; 31: Lowest priority
 * @param color
 * @returns     Sprite Handle
 */
extern unsigned int vdp_sprite_init(unsigned char name, unsigned char priority, unsigned char color);

/**
 * @brief Set position of a sprite
 *
 * @param addr  Sprite Handle returned by vdp_sprite_init()
 * @param x
 * @param y
 * @returns     true: In case of a collision with other sprites
 */
extern unsigned char vdp_sprite_set_position(unsigned int addr, unsigned int x, unsigned char y);

// VDP6847 VRAM ACCESS AND VARIABLES
#define vdpAddCol           0x000100   // A0 VDP = A8 CPU
#define vdpAddRow           0x002000   // 32 BYTES PER ROW
#define vdpCharWidth        5
#define vdpCharHeitgh       7
#define vdpPixelBetweenChar 3          // 8 x 8
#define vdpMaxX             256
#define vdpMaxY             192

/**
 * @brief VDP Status flags from status register
 */
#define VDP_FLAG_COIN 0x20 /*Coincidence flag, set when sprites overlap*/
#define VDP_FLAG_S5 0x40  /*5th sprite flag, set when more than 4 sprite per line */

/**
 * @brief VDP status
 */
#define VDP_OK 0
#define VDP_ERROR 1

#define addline 0x01
#define noaddline 0x00

extern void geraScroll(void);
extern void clearScr(void);
extern void printChar(unsigned char pchr, unsigned char pmove);
extern void printText(unsigned char *msg);

#endif
