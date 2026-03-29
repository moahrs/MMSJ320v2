// Variable Shared Definitions
#define MONITOR_VAR_VDP_TABLE 0x00600000

// Function Shared Definitions
#define MONITOR_FUNC_TABLE    0x0000041A

typedef int (*processCmdType)(void);
typedef void (*clearScrType)(void);
typedef void (*printTextType)(unsigned char *msg);
typedef void (*printCharType)(unsigned char pchr, unsigned char pmove);
typedef void (*delaymsType)(int pTimeMS);
typedef unsigned char (* loadSerialToMemType)(unsigned char *pEnder, unsigned char ptipo);
typedef unsigned char (* readCharType)(void);
typedef void (*hideCursorType)(void);
typedef void (*showCursorType)(void);
typedef unsigned char (* inputLineType)(unsigned int pQtdInput, unsigned char pTipo);
typedef void (*modeVideoType)(unsigned char *pMode);
typedef int (*vdp_initType)(unsigned char mode, unsigned char color, unsigned char big_sprites, unsigned char magnify);
typedef void (*vdp_colorizeType)(unsigned char fg, unsigned char bg);
typedef void (*vdp_plot_hiresType)(unsigned char x, unsigned char y, unsigned char color1, unsigned char color2);
typedef void (*vdp_plot_colorType)(unsigned char x, unsigned char y, unsigned char color);
typedef void (*vdp_set_bdcolorType)(unsigned char color);
typedef void (*vdp_set_pattern_colorType)(unsigned int index, unsigned char fg, unsigned char bg);
typedef void (*vdp_set_cursorType)(unsigned char pcol, unsigned char prow);
typedef void (*vdp_set_cursor_posType)(unsigned char direction);
typedef void (*vdp_textcolorType)(unsigned char fg, unsigned char bg);
typedef void (*vdp_writeType)(unsigned char chr);
typedef void (*vdp_set_sprite_patternType)(unsigned char number, const unsigned char *sprite);
typedef void (*vdp_sprite_colorType)(unsigned int addr, unsigned char color);
typedef Sprite_attributes (*vdp_sprite_get_attributesType)(unsigned int addr);
typedef Sprite_attributes (*vdp_sprite_get_positionType)(unsigned int addr);
typedef unsigned int (*vdp_sprite_initType)(unsigned char name, unsigned char priority, unsigned char color);
typedef unsigned char (*vdp_sprite_set_positionType)(unsigned int addr, unsigned int x, unsigned char y);
typedef void (*writeLongSerialType)(unsigned char *msg);
typedef void (*writeSerialType)(unsigned char pchr);
typedef char (*vdp_read_color_pixelType)(unsigned char x, unsigned char y);
typedef VDP_COORD (*vdp_get_cursorType)(void);
typedef VDP_COLOR (*vdp_get_colorType)(void);
typedef unsigned long (*lstmGetSizeType)(void);
typedef void (*vdp_get_cfgType)(unsigned int *pat, unsigned int *cor);
typedef void (*setRegisterType)(unsigned char registerIndex, unsigned char value);
typedef unsigned char (*read_status_regType)(void);
typedef void (*setWriteAddressType)(unsigned int address);
typedef void (*setReadAddressType)(unsigned int address);
typedef unsigned long (*getVideoFontesType)(void);
typedef unsigned char (*readMouseType)(unsigned char *vStat, unsigned char *vMovX, unsigned char *vMovY);

#define processCmd ((processCmdType *)(unsigned long)MONITOR_FUNC_TABLE)[0] // Índice da função
#define clearScr ((clearScrType *)(unsigned long)MONITOR_FUNC_TABLE)[1] // Índice da função
#define printText ((printTextType *)(unsigned long)MONITOR_FUNC_TABLE)[2] // Índice da função
#define printChar ((printCharType *)(unsigned long)MONITOR_FUNC_TABLE)[3] // Índice da função
#define delayms ((delaymsType *)(unsigned long)MONITOR_FUNC_TABLE)[4] // Índice da função
#define loadSerialToMem ((loadSerialToMemType *)(unsigned long)MONITOR_FUNC_TABLE)[5] // Índice da função
#define readChar ((readCharType *)(unsigned long)MONITOR_FUNC_TABLE)[6] // Índice da função
#define hideCursor ((hideCursorType *)(unsigned long)MONITOR_FUNC_TABLE)[7] // Índice da função
#define showCursor ((showCursorType *)(unsigned long)MONITOR_FUNC_TABLE)[8] // Índice da função
#define inputLine ((inputLineType *)(unsigned long)MONITOR_FUNC_TABLE)[9] // Índice da função
#define modeVideo ((modeVideoType *)(unsigned long)MONITOR_FUNC_TABLE)[10] // Índice da função
#define vdp_init ((vdp_initType *)(unsigned long)MONITOR_FUNC_TABLE)[11] // Índice da função
#define vdp_colorize ((vdp_colorizeType *)(unsigned long)MONITOR_FUNC_TABLE)[12] // Índice da função
#define vdp_plot_hires ((vdp_plot_hiresType *)(unsigned long)MONITOR_FUNC_TABLE)[13] // Índice da função
#define vdp_plot_color ((vdp_plot_colorType *)(unsigned long)MONITOR_FUNC_TABLE)[14] // Índice da função
#define vdp_set_bdcolor ((vdp_set_bdcolorType *)(unsigned long)MONITOR_FUNC_TABLE)[15] // Índice da função
#define vdp_set_pattern_color ((vdp_set_pattern_colorType *)(unsigned long)MONITOR_FUNC_TABLE)[16] // Índice da função
#define vdp_set_cursor ((vdp_set_cursorType *)(unsigned long)MONITOR_FUNC_TABLE)[17] // Índice da função
#define vdp_set_cursor_pos ((vdp_set_cursor_posType *)(unsigned long)MONITOR_FUNC_TABLE)[18] // Índice da função
#define vdp_textcolor ((vdp_textcolorType *)(unsigned long)MONITOR_FUNC_TABLE)[19] // Índice da função
#define vdp_write ((vdp_writeType *)(unsigned long)MONITOR_FUNC_TABLE)[20] // Índice da função
#define vdp_set_sprite_pattern ((vdp_set_sprite_patternType *)(unsigned long)MONITOR_FUNC_TABLE)[21] // Índice da função
#define vdp_sprite_color ((vdp_sprite_colorType *)(unsigned long)MONITOR_FUNC_TABLE)[22] // Índice da função
#define vdp_sprite_get_attributes ((vdp_sprite_get_attributesType *)(unsigned long)MONITOR_FUNC_TABLE)[23] // Índice da função
#define vdp_sprite_get_position ((vdp_sprite_get_positionType *)(unsigned long)MONITOR_FUNC_TABLE)[24] // Índice da função
#define vdp_sprite_init ((vdp_sprite_initType *)(unsigned long)MONITOR_FUNC_TABLE)[25] // Índice da função
#define vdp_sprite_set_position ((vdp_sprite_set_positionType *)(unsigned long)MONITOR_FUNC_TABLE)[26] // Índice da função
#define writeLongSerial ((writeLongSerialType *)(unsigned long)MONITOR_FUNC_TABLE)[27] // Índice da função
#define writeSerial ((writeSerialType *)(unsigned long)MONITOR_FUNC_TABLE)[28] // Índice da função
#define vdp_read_color_pixel ((vdp_read_color_pixelType *)(unsigned long)MONITOR_FUNC_TABLE)[29] // Índice da função
#define vdp_get_cursor ((vdp_get_cursorType *)(unsigned long)MONITOR_FUNC_TABLE)[30] // Índice da função
#define vdp_get_color ((vdp_get_colorType *)(unsigned long)MONITOR_FUNC_TABLE)[31] // Índice da função
#define lstmGetSize ((lstmGetSizeType *)(unsigned long)MONITOR_FUNC_TABLE)[32] // Índice da função
#define vdp_get_cfg ((vdp_get_cfgType *)(unsigned long)MONITOR_FUNC_TABLE)[33] // Índice da função
#define setRegister ((setRegisterType *)(unsigned long)MONITOR_FUNC_TABLE)[34] // Índice da função
#define read_status_reg ((read_status_regType *)(unsigned long)MONITOR_FUNC_TABLE)[35] // Índice da função
#define setWriteAddress ((setWriteAddressType *)(unsigned long)MONITOR_FUNC_TABLE)[36] // Índice da função
#define setReadAddress ((setReadAddressType *)(unsigned long)MONITOR_FUNC_TABLE)[37] // Índice da função
#define getVideoFontes ((getVideoFontesType *)(unsigned long)MONITOR_FUNC_TABLE)[38] // Índice da função
#define readMouse ((readMouseType *)(unsigned long)MONITOR_FUNC_TABLE)[39] // Índice da função
