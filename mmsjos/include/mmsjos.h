#ifndef   MMSJ_OS_H
#define   MMSJ_OS_H

/*typedef void                    VOID;
typedef char                    CHAR;
typedef unsigned char           BYTE;                           // 8-bit unsigned
typedef unsigned short          WORD;                           // 16-bit unsigned
typedef unsigned long           DWORD;                          // 32-bit unsigned
*/
#define FAT32       3

#define S_OK         0  // no error
#define S_NOTINIT    1  // ArduinoFDC.begin() was not called
#define S_NOTREADY   2  // Drive is not ready (no disk or power)
#define S_NOSYNC     3  // No sync marks found
#define S_NOHEADER   4  // Sector header not found
#define S_INVALIDID  5  // Sector data record has invalid id
#define S_CRC        6  // Sector data checksum error
#define S_NOTRACK0   7  // No track0 signal
#define S_VERIFY     8  // Verify after write failed
#define S_READONLY   9  // Attempt to write to a write-protected disk

typedef struct
{
	unsigned short       firsts;
	unsigned short       fat;
	unsigned short       root;
	unsigned short       data;
	unsigned short       maxroot;
	unsigned short       maxcls;
	unsigned short       RootEntiesCount;
	unsigned short       numheads;
	unsigned short       sectorSize;
	unsigned short       secperfat;
	unsigned short       secpertrack;
	unsigned short       fatsize;
	unsigned char        NumberOfFATs;
	unsigned short       reserv;
	unsigned char        SecPerClus;
	unsigned char        type;
	unsigned char        mount;
} DISK12;

typedef struct
{
	unsigned long        firsts;
	unsigned long        fat;
	unsigned long        root;
	unsigned long        data;
	unsigned short       maxroot;
	unsigned long        maxcls;
	unsigned short       sectorSize;
	unsigned long        fatsize;
	unsigned short       reserv;
	unsigned char        SecPerClus;
	unsigned char        type;
	unsigned char        mount;
} DISK;

typedef struct
{
	unsigned char        Name[8];
	unsigned char        Ext[3];
	unsigned char        Attr;
	unsigned short       CreateDate;
	unsigned short       CreateTime;
	unsigned short       LastAccessDate;
	unsigned short       UpdateDate;
	unsigned short       UpdateTime;
	unsigned long        FirstCluster;
	unsigned long        Size;
	unsigned long        DirClusSec;
	unsigned short       DirEntry;
	unsigned char        Updated;
} FAT32_DIR;

typedef struct
{
	unsigned char        Name[8];
	unsigned char        Ext[3];
} FILE_NAME;

typedef struct
{
	char                 Name[13];
	unsigned long        ClusterDir;
	unsigned long        ClusterDirAtu;
} RET_PATH;

typedef struct
{
	unsigned long *prev;
	char name[11];
	unsigned long address;
	unsigned long size;
	char status;
	unsigned long *next;
} MEM_ALOC;

extern FAT32_DIR vdir;
extern DISK  vdisk;
extern unsigned long vclusterdir;
extern unsigned char vbuf[128];
extern unsigned char gDataBuffer[512];
extern unsigned short verroSo;
extern unsigned char vdiratu[128];
extern unsigned short vdiratuidx;
extern unsigned char verro;
extern RET_PATH vretpath;
extern RET_PATH vretpath2;
extern MEM_ALOC vMemAloc;

#define FAT16       3

#define FS_CMD      0
#define FS_DATA     1
#define FS_PAR      2

#define TRUE        1
#define FALSE       0

#if !defined(NULL)
	#define NULL        '\0'
#endif

#define MEDIA_SECTOR_SIZE   512

#define ATTR_READ_ONLY      0x01
#define ATTR_HIDDEN         0x02
#define ATTR_SYSTEM         0x04
#define ATTR_VOLUME         0x08
#define ATTR_LONG_NAME      0x0f
#define ATTR_DIRECTORY      0x10
#define ATTR_ARCHIVE        0x20
#define ATTR_MASK           0x3f

#define CLUSTER_EMPTY               0x0000
#define LAST_CLUSTER_FAT32      0x0FFFFFFF
#define END_CLUSTER_FAT32       0x0FFFFFF7
#define CLUSTER_FAIL_FAT32      0x0FFFFFFF

#define NUMBER_OF_unsigned charS_IN_DIR_ENTRY    32
#define DIR_DEL             0xE5
#define DIR_EMPTY           0
#define DIR_NAMESIZE        8
#define DIR_EXTENSION       3
#define DIR_NAMECOMP        (DIR_NAMESIZE+DIR_EXTENSION)

#define EOF             ((int)-1)

#define OPER_READ      0x01
#define OPER_WRITE     0x02
#define OPER_READWRITE 0x03

#define CONV_DATA    0x01
#define CONV_HORA    0x02

#define INFO_SIZE    0x01
#define INFO_CREATE  0x02
#define INFO_UPDATE  0x03
#define INFO_LAST    0x04

#define FIND_PATH_PART 0x00
#define FIND_PATH_LAST 0x01

#define FIND_PATH_RET_ERROR 0xFF
#define FIND_PATH_RET_FOLDER 0x01
#define FIND_PATH_RET_FILE 0x02

#define TYPE_DIRECTORY   0x01
#define TYPE_FILE        0x02
#define TYPE_EMPTY_ENTRY 0x03
#define TYPE_CREATE_FILE 0x04
#define TYPE_CREATE_DIR  0x05
#define TYPE_DEL_FILE    0x06
#define TYPE_DEL_DIR     0x07
#define TYPE_FIRST_ENTRY 0x08
#define TYPE_NEXT_ENTRY  0x09
#define TYPE_ALL         0xFF

#define FREE_FREE 0x01
#define FREE_USE  0x02
#define NEXT_FREE 0x03
#define NEXT_FULL 0x04
#define NEXT_FIND 0x05

#define ERRO_D_START         0xFFFFFFF0
#define ERRO_D_FILE_NOT_FOUND 0xFFFFFFF0
#define ERRO_D_READ_DISK      0xFFFFFFF1
#define ERRO_D_WRITE_DISK     0xFFFFFFF2
#define ERRO_D_OPEN_DISK      0xFFFFFFF3
#define ERRO_D_DISK_FULL      0xFFFFFFF4
#define ERRO_D_INVALID_NAME   0xFFFFFFF5
#define ERRO_D_NOT_FOUND      0xFFFFFFFF

#define ALL_OK                0x00
#define ERRO_B_START          0xE0
#define ERRO_B_FILE_NOT_FOUND 0xE0
#define ERRO_B_READ_DISK      0xE1
#define ERRO_B_WRITE_DISK     0xE2
#define ERRO_B_OPEN_DISK      0xE3
#define ERRO_B_INVALID_NAME   0xE4
#define ERRO_B_DIR_NOT_FOUND  0xE5
#define ERRO_B_CREATE_FILE    0xE6
#define ERRO_B_APAGAR_ARQUIVO 0xE7
#define ERRO_B_FILE_FOUND     0xE8
#define ERRO_B_UPDATE_DIR     0xE9
#define ERRO_B_OFFSET_READ    0xEA
#define ERRO_B_DISK_FULL      0xEB
#define ERRO_B_READ_FILE      0xEC
#define ERRO_B_WRITE_FILE     0xED
#define ERRO_B_DIR_FOUND      0xEE
#define ERRO_B_CREATE_DIR     0xEF
#define ERRO_B_NOT_FOUND      0xFF

#define RETURN_OK             0x00

#define TASK_MMSJOS_MAIN    10

extern const unsigned char strValidChars[];
extern const unsigned char vmesc[12][3];

extern unsigned long fsInit(void);
extern void fsVer(void);
extern void printDiskError(unsigned char pError);
extern unsigned char fsMountDisk(void);
extern unsigned long fsOsCommand(unsigned char * linhaParametro);
extern unsigned char fsFormat (long int serialNumber, char * volumeID);
extern void fsSetClusterDir (unsigned long vclusdiratu);
extern unsigned long fsGetClusterDir (void);
extern unsigned char fsSectorWrite(unsigned long vsector, unsigned char* vbuffer, unsigned char vtipo);
extern unsigned char fsSectorRead(unsigned long vsector, unsigned char* vbuffer);
extern int fsRecSerial(unsigned char* pByte, unsigned char pTimeOut);
extern int fsSendSerial(unsigned char pByte);
extern int fsSendByte(unsigned char vByte, unsigned char pType);
extern unsigned char fsRecByte(unsigned char pType);
extern int fsSendLongSerial(unsigned char *msg);
extern void fsConvClusterToTHS(unsigned short cluster, unsigned char* vtrack, unsigned char* vside, unsigned char* vsector);
extern void fsReadDir(unsigned short ix, unsigned short vdata);
extern unsigned char fsCreateFile(char * vfilename);
extern unsigned char fsOpenFile(char * vfilename);
extern unsigned char fsCloseFile(char * vfilename, unsigned char vupdated);
extern unsigned long fsInfoFile(char * vfilename, unsigned char vtype);
extern unsigned char fsRWFile(unsigned long vclusterini, unsigned long voffset, unsigned char *buffer, unsigned char vtype);
extern unsigned short fsReadFile(char * vfilename, unsigned long voffset, unsigned char *buffer, unsigned short vsizebuffer);
extern unsigned char fsWriteFile(char * vfilename, unsigned long voffset, unsigned char *buffer, unsigned char vsizebuffer);
extern unsigned char fsDelFile(char * vfilename);
extern unsigned char fsRenameFile(char * vfilename, char * vnewname);
extern void runFromOsCmd(void);
extern unsigned long loadFile(unsigned char *parquivo, unsigned short* xaddress);
extern void catFile(unsigned char *parquivo);
extern unsigned char fsLoadSerialToFile(char * vfilename, char * vPosMem);
extern unsigned char fsFindDirPath(char * vpath, char vtype);
extern void fsGetDirAtuData(FAT32_DIR *pDir);
extern unsigned long fsMalloc(unsigned long vMemSize);
extern void fsFree(unsigned long vAddress);
extern void runFromMGUI(unsigned long vEnderExec);
extern unsigned char fsMakeDir(char * vdirname);
extern unsigned char fsChangeDir(char * vdirname);
extern unsigned char fsRemoveDir(char * vdirname);
extern unsigned char fsPwdDir(unsigned char *vdirpath);
extern unsigned short fsLoadFat(unsigned short vclusteratual);
extern unsigned long fsFindInDir(char * vname, unsigned char vtype);
extern unsigned char fsUpdateDir(void);
extern unsigned long fsFindNextCluster(unsigned long vclusteratual, unsigned char vtype);
extern unsigned long fsFindClusterFree(unsigned char vtype);
extern unsigned int bcd2dec(unsigned int bcd);
extern int getDateTimeAtu(void);
extern unsigned short datetimetodir(unsigned char hr_day, unsigned char min_month, unsigned char sec_year, unsigned char vtype);
extern unsigned long pow(int val, int pot);
extern int hex2int(char ch);
extern unsigned long hexToLong(char *pHex);
extern void strncpy2(char* _dst, const char* _src, int _n);
extern int isValidFilename(char *filename);
extern unsigned char matches_wildcard(const char *pattern, const char *filename);
extern unsigned char contains_wildcards(const char *pattern);
extern void fsSetMfp(unsigned int Config, unsigned char Value, unsigned char TypeSet);
extern unsigned int fsGetMfp(unsigned int Config);
extern void memInit(void);

#endif
