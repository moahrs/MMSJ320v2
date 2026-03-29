typedef struct  {
  unsigned char nameVar[3];  // variable name
  long endVar; // address off the counter variable
  long target;  // target value
  long step; // step inc/dec
  long progPosPointerRet;
} for_stack;

struct keyword_token
{
  char *keyword;
  int token;
};

typedef struct
{
  unsigned char tString[250];
  short tInt;
  long tLong;
  unsigned char tType;  // 0 - String, 1 - Int, 2 - Long
} typeInf;

unsigned char vbufInput[256];
unsigned char fgcolorBas;    // Buffer da VRAM 6KB onde o computador vai trabalhar e a cada interrupcao, sera enviado a VRAM
unsigned char bgcolorBas; // QTD de BYTES no eixo X (COLS) a srem enviados pra VRAM
unsigned char fgcolorBasAnt;    // Buffer da VRAM 6KB onde o computador vai trabalhar e a cada interrupcao, sera enviado a VRAM
unsigned char bgcolorBasAnt; // QTD de BYTES no eixo X (COLS) a srem enviados pra VRAM
unsigned char vdpModeBas; // Modo de video 0 = caracter (32 x 24), 1 = grafico (256 x 192)
unsigned char vdpMaxCols; // max col number
unsigned char vdpMaxRows;
unsigned short videoCursorPosCol;  // Posicao atual caracter do cursor na coluna (0 a 31)
unsigned short videoCursorPosRow;  // Posical atual caracter do cursor na linha (0 a 23) 
unsigned short videoCursorPosColX;  // Posicao atual do cursor na coluna (0 a 255)
unsigned short videoCursorPosRowY;  // Posical atual do cursor na linha (0 a 191)

unsigned char *pStartSimpVar        = 0x00800000;   // Area Variaveis Simples
unsigned char *pStartArrayVar       = 0x00803000;   // Area Arrays
unsigned char *pStartString         = 0x00810000;   // Area Strings
unsigned char *pStartProg           = 0x00830000;   // Area Programa  deve ser 0x00810000
unsigned char *pStartXBasLoad       = 0x00890000;   // Area onde será importado o programa em basic texto a ser tokenizado depois
unsigned char *pStartStack          = 0x008FE000;   // Area variaveis sistema e stack pointer

unsigned char *pProcess             = 0x008FFFFE;
unsigned char *pTypeLine            = 0x008FFFFC; // 0x00 = Proxima linha tem "READY" e ">". 0x01 = Proxima linha tem somente ">"
unsigned long *nextAddrLine         = 0x008FFFF8; // Endereco da proxima linha disponivel pra ser incluida do basic
unsigned short *firstLineNumber     = 0x008FFFF6; // Numero de Linha mais Baixo
unsigned long *addrFirstLineNumber  = 0x008FFFF2; // Endereco do numero de linha mais baixo
unsigned long *addrLastLineNumber   = 0x008FFFEE; // Endereco do numero de linha mais baixo
unsigned long *nextAddr             = 0x008FFFEA; // usado para controleno runProg
unsigned long *nextAddrSimpVar      = 0x008FFFE6; // Endereco da proxima linha disponivel pra definir variavel
unsigned long *nextAddrArrayVar     = 0x008FFFE2; // Endereco da proxima linha disponivel pra definir array
unsigned long *nextAddrString       = 0x008FFFDE; // Endereco da proxima linha disponivel pra incluir string

unsigned char *comandLineTokenized  = 0x008FFEDF; // Linha digitada sem numeros inicial e sem comandos basicos irá interpretada com tokens (255 Bytes)
unsigned char *vParenteses          = 0x008FFEDD; // Controle de Parenteses na linha inteira durante processamento
unsigned char *vInicioSentenca      = 0x008FFEDB; // Indica inicio de sentenca, sempre inicio analise ou depois de uma ",", ":", "THEN" ou "ELSE"
unsigned char *vMaisTokens          = 0x008FFED9; // Tem um = como atribuicao mas tem mais variaveis e/ou tokens no sistema
unsigned char *vTemIf               = 0x008FFED7; // Linha comecou com if, e tem que ter pelo menos then ou then e else
unsigned char *doisPontos           = 0x008FFED3; // Se teve 2 pontos na linha e inicia novo comando como se fosse linha nova
unsigned char *vTemAndOr            = 0x008FFED1; // Foi lido uma vez um AND ou um OR, e deve finalizar a condicao anterior, na proxima deve executar ele.
unsigned char *vTemThen             = 0x008FFECF; // Foi lido then
unsigned char *vTemElse             = 0x008FFECD; // Foi lido else
unsigned char *vTemIfAndOr          = 0x008FFECA; // Foi lido uma vez um AND ou um OR com IF, e deve finalizar a condicao anterior, na proxima deve executar ele.
unsigned short *vErroProc           = 0x008FFEC6; // Erro global
int *ftos                           = 0x008FFEC2; // index to top of FOR stack
int *gtos                           = 0x008FFEBE; // index to top of GOSUB stack

unsigned long *floatBufferStr       = 0x008FEFA6; // Endereco da variavel do buffer q vai receber a string vindo do floattostr
unsigned long *floatNumD7           = 0x008FEE9E; // com o valor a colocar em D7 para calculos float
unsigned long *floatNumD6           = 0x008FEE96; // com o valor a colocar em D6 para calculos float
unsigned long *floatNumA0           = 0x008FEE8E; // com o valor a colocar em A0 para saber qual BNE usar
unsigned long *randSeed             = 0x008FEE86; // Numero para ser usado no randomico
unsigned char *lastHgrX             = 0x008FEE84; //
unsigned char *lastHgrY             = 0x008FEE82; //
unsigned long *vDataBkpPointerProg  = 0x008FEE70; // 
unsigned long *vDataPointer         = 0x008FEE5A; // 
unsigned long *pointerRunProg       = 0x008FEE56; // Ponteiro da execucao do programa ou linha digitada
unsigned char *tok                  = 0x008FEE54;
unsigned char *token_type           = 0x008FEE52;
unsigned char *value_type           = 0x008FEE50;
unsigned long *onErrGoto            = 0x008FEE4A;
unsigned long *changedPointer       = 0x008FEE42; // Se ouve mudanca de endereço (goto/gosub/for-next e etc), nao usa sequencia, usa o endereço definido
unsigned char *token                = 0x008FED6E; // 
unsigned char *varName              = 0x008FEC6E; // ja esta ok
unsigned char *traceOn              = 0x008FEC66; // Mostra numero linhas durante execucao, para debug
unsigned char *debugOn              = 0x008FEC64; // Mostra debug, para debug
unsigned long *gosubStack           = 0x008FE966; // ja esta ok stack for gosub/return
unsigned long *vDataFirst           = 0x008FE962; // 
unsigned long *vDataLineAtu         = 0x008FE95E; // 
for_stack *forStack                 = 0x008FF6BE; // stack for FOR/NEXT loop
unsigned long *atuVarAddr           = 0x008FF6B0; // Endereco da variavel atualmente usada pelo basLet

const keywords_count = 67;

// -------------------------------------------------------------------------------
// Mensagens de Erro
// -------------------------------------------------------------------------------
static unsigned char *listError[]= {
    /* 00 */ "?reserved 0",
    /* 01 */ "?Stopped",
    /* 02 */ "?No expression present",
    /* 03 */ "?Equals sign expected",
    /* 04 */ "?Not a variable",
    /* 05 */ "?Out of range",
    /* 06 */ "?Illegal quantity",
    /* 07 */ "?Line not found",
    /* 08 */ "?THEN expected",
    /* 09 */ "?TO expected",
    /* 10 */ "?Too many nested FOR loops",
    /* 11 */ "?NEXT without FOR",
    /* 12 */ "?Too many nested GOSUBs",
    /* 13 */ "?RETURN without GOSUB",
    /* 14 */ "?Syntax error",
    /* 15 */ "?Unbalanced parentheses",
    /* 16 */ "?Incompatible types",
    /* 17 */ "?Line number expected",
    /* 18 */ "?Comma Espected",
    /* 19 */ "?Timeout",
    /* 20 */ "?Load with Errors",
    /* 21 */ "?Size error",
    /* 22 */ "?Out of memory",
    /* 23 */ "?Variable name already exist",
    /* 24 */ "?Wrong mode resolution",
    /* 25 */ "?Illegal position",
    /* 26 */ "?Out of data"
};

// -------------------------------------------------------------------------------
// Tokens
//    0x80 - 0xBF : Commands
//    0xC0 - 0xEF : Functions
//    0xF0 - 0xFF : Operands
// -------------------------------------------------------------------------------
static const struct keyword_token keywords[] =
{                      // 1a 2a 3a - versoes (-- : desenv/testar, ok : funcionando, .. : nao feito)
  {"LET", 		0x80},   // ok ok ok
  {"PRINT", 	0x81},   // ok ok ok
  {"IF", 		  0x82},   // .. ok ok
  {"THEN", 		0x83},   // .. ok ok
  {"FOR", 		0x85},   // .. .. ok
  {"TO", 		  0x86},   // .. .. ok
  {"NEXT", 		0x87},   // .. .. ok
  {"STEP", 		0x88},   // .. .. ok
  {"GOTO" , 	0x89},   // .. .. ok
  {"GOSUB", 	0x8A},   // .. .. ok
  {"RETURN", 	0x8B},   // .. .. ok
  {"REM", 		0x8C},   // .. .. ok
  {"INVERSE", 0x8D},   // .. .. ok
  {"NORMAL",  0x8E},   // .. .. ok
  {"DIM", 		0x8F},   // .. .. ok
  {"ON",      0x91},   // .. .. ok
  {"INPUT", 	0x92},   // .. ok ok
  {"GET",     0x93},   // .. ok ok
  {"VTAB",    0x94},   // .. .. ok
  {"HTAB",    0x95},   // .. .. ok
  {"HOME", 		0x96},   // ok ok ok
  {"CLEAR", 	0x97},   // .. .. ok
  {"DATA", 		0x98},   // .. .. ok
  {"READ", 		0x99},   // .. .. ok
  {"RESTORE", 0x9A},   // .. .. ok
  {"END",     0x9E},   // .. .. ok
  {"STOP",    0x9F},   // .. .. ok
  {"TEXT",    0xB0},   // .. .. ok
  {"GR",      0xB1},   // .. .. ok
  {"HGR",     0xB2},   // .. .. ok
  {"COLOR",   0xB3},   // .. .. ok
  {"PLOT",    0xB4},   // .. .. ok
  {"HLIN",    0xB5},   // .. .. ok
  {"VLIN",    0xB6},   // .. .. ok
  {"HCOLOR",  0xB8},   // .. .. ok
  {"HPLOT",   0xB9},   // .. .. ok
  {"AT",      0xBA},   // .. .. ok
  {"ONERR",   0xBB},   // .. .. ok
  {"ASC",     0xC4},   // .. .. ok
  {"PEEK",    0xCD},   // .. .. ok
  {"POKE",    0xCE},   // .. .. ok
  {"RND",     0xD1},   // .. .. ok
  {"LEN",     0xDB},   // ok ok ok
  {"VAL",     0xDC},   // ok ok ok
  {"STR$",    0xDD},   // ok ok ok
  {"SCRN",    0xE0},   // .. .. ok
  {"CHR$",    0xE1},   // ok ok ok
  {"FRE",     0xE2},   // ok ok ok
  {"SQRT",    0xE3},   // ok ok ok
  {"SIN",     0xE4},   // ok ok ok
  {"COS",     0xE5},   // ok ok ok
  {"TAN",     0xE6},   // ok ok ok
  {"LOG",     0xE7},   // .. .. ok
  {"EXP",     0xE8},   // .. .. ok
  {"SPC",     0xE9},   // .. .. ok
  {"TAB",     0xEA},   // .. .. ok
  {"MID$",    0xEB},   // .. .. ok
  {"RIGHT$",  0xEC},   // .. .. ok
  {"LEFT$",   0xED},   // .. .. ok
  {"INT",     0xEE},   // .. .. ok
  {"ABS",     0xEF},   // .. .. ok
  {"AND",     0xF3},   // ok ok ok
  {"OR",      0xF4},   // ok ok ok
  {">=",      0xF5},   // ok ok ok
  {"<=",      0xF6},   // ok ok ok
  {"<>",      0xF7},   // ok ok ok
  {"NOT",     0xF8}    // .. .. ok
};

const char operandsWithTokens[] = "+-*/^>=<";

#define VARTYPEDEFAULT 0x23 
#define FOR_NEST 80
#define SUB_NEST 190

#define FINISHED  0xE0
#define END       0xE1
#define EOL       0xE2

#define DELIMITER  1
#define VARIABLE  2
#define NUMBER    3
#define COMMAND   4
#define STRING    5
#define QUOTE     6
#define DOISPONTOS 7
#define OPENPARENT 8
#define CLOSEPARENT 9
