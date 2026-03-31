#ifndef MMSJOSAPI_H
#define MMSJOSAPI_H

#include "mmsjos.h"
#include "mgui.h"

#define MMSJOS_FUNC_TABLE    0x00800032
#define MGUI_FUNC_TABLE      0x00805576

typedef unsigned char (*fsGetDirAtuDataType)(FAT32_DIR *pDir);
typedef void (*fsSetClusterDirType)(unsigned long vclusdiratu);
typedef unsigned long (*fsGetClusterDirType)(void);
typedef unsigned char (*fsSectorWriteType)(unsigned long vsector, unsigned char* vbuffer, unsigned char vtipo);
typedef unsigned char (*fsSectorReadType)(unsigned long vsector, unsigned char* vbuffer);
typedef unsigned char (*fsFindDirPathType)(char * vpath, char vtype);
typedef unsigned long (*fsOsCommandType)(unsigned char * linhaParametro);
typedef unsigned char (*fsCreateFileType)(char * vfilename);
typedef unsigned char (*fsOpenFileType)(char * vfilename);
typedef unsigned char (*fsCloseFileType)(char * vfilename, unsigned char vupdated);
typedef unsigned long (*fsInfoFileType)(char * vfilename, unsigned char vtype);
typedef unsigned char (*fsFreeType)(unsigned long vAddress);
typedef unsigned short (*fsReadFileType)(char * vfilename, unsigned long voffset, unsigned char *buffer, unsigned short vsizebuffer);
typedef unsigned char (*fsWriteFileType)(char * vfilename, unsigned long voffset, unsigned char *buffer, unsigned char vsizebuffer);
typedef unsigned char (*fsDelFileType)(char * vfilename);
typedef unsigned char (*fsRenameFileType)(char * vfilename, char * vnewname);
typedef unsigned long (*loadFileType)(unsigned char *parquivo, unsigned short* xaddress);
typedef unsigned char (*fsMakeDirType)(char * vdirname);
typedef unsigned char (*fsChangeDirType)(char * vdirname);
typedef unsigned char (*fsRemoveDirType)(char * vdirname);
typedef unsigned char (*fsPwdDirType)(unsigned char *vdirpath);
typedef unsigned long (*fsFindInDirType)(char * vname, unsigned char vtype);
typedef unsigned long (*fsMallocType)(unsigned long vMemSize);
typedef unsigned long (*fsFindNextClusterType)(unsigned long vclusteratual, unsigned char vtype);
typedef unsigned long (*fsFindClusterFreeType)(unsigned char vtype);
typedef unsigned char (*OSTimeDlyHMSMType)(unsigned char hours, unsigned char minutes, unsigned char seconds, unsigned int ms);
typedef void (*fsSetMfpType)(unsigned int Config, unsigned char Value, unsigned char TypeSet);
typedef unsigned int (*fsGetMfpType)(unsigned int Config);

typedef void (*writesxyType)(unsigned short x, unsigned short y, unsigned char sizef, unsigned char *msgs, unsigned short pcolor, unsigned short pbcolor);
typedef void (*writecxyType)(unsigned char sizef, unsigned char pbyte, unsigned short pcolor, unsigned short pbcolor);
typedef void (*locatexyType)(unsigned short xx, unsigned short yy);
typedef void (*SaveScreenNewType)(MGUI_SAVESCR *mguiSave, unsigned short xi, unsigned short yi, unsigned short pwidth, unsigned short pheight);
typedef void (*RestoreScreenType)(MGUI_SAVESCR vEnderSave);
typedef void (*SetDotType)(unsigned short x, unsigned short y, unsigned short color);
typedef void (*SetByteType)(unsigned short ix, unsigned short iy, unsigned char pByte, unsigned short pfcolor, unsigned short pbcolor);
typedef void (*FillRectType)(unsigned char xi, unsigned char yi, unsigned short pwidth, unsigned char pheight, unsigned char pcor);
typedef void (*DrawLineType)(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color);
typedef void (*DrawRectType)(unsigned short xi, unsigned short yi, unsigned short pwidth, unsigned short pheight, unsigned short color);
typedef void (*DrawRoundRectType)(unsigned int xi, unsigned int yi, unsigned int pwidth, unsigned int pheight, unsigned char radius, unsigned char color);
typedef void (*DrawCircleType)(unsigned short x0, unsigned short y0, unsigned char r, unsigned char pfil, unsigned short pcor);
typedef void (*PutIconeType)(unsigned int* vimage, unsigned short x, unsigned short y, unsigned char numSprite);
typedef void (*InvertRectType)(unsigned short xi, unsigned short yi, unsigned short pwidth, unsigned short pheight);
typedef void (*SelRectType)(unsigned short x, unsigned short y, unsigned short pwidth, unsigned short pheight);
typedef void (*PutImageType)(unsigned char* cimage, unsigned short x, unsigned short y);
typedef void (*LoadIconLibType)(unsigned char* cfile);
typedef unsigned char (*waitButtonType)(void);
typedef unsigned char (*messageType)(char* bstr, unsigned char bbutton, unsigned short btime);
typedef void (*drawButtonsnewType)(unsigned char *vbuttons, unsigned char *pbbutton, unsigned short xib, unsigned short yib);
typedef void (*showWindowType)(unsigned char* bstr, unsigned char x1, unsigned char y1, unsigned short pwidth, unsigned char pheight, unsigned char bbutton);
typedef void (*TrocaSpriteMouseType)(unsigned char vicone);
typedef void (*MostraIconeType)(unsigned short xi, unsigned short yi, unsigned char vicone, unsigned char colorfg, unsigned char colorbg);
typedef void (*importFileType)(void);
typedef void (*putImagePbmP4Type)(unsigned char* memoria, unsigned short ix, unsigned short iy);
typedef void (*setPosPressedType)(unsigned char vppostx, unsigned char vpposty);
typedef void (*getMouseDataType)(MGUI_MOUSE *pmouseData);
typedef void (*toggleboxType)(unsigned char* bstr, unsigned char *vvar, unsigned short x, unsigned short y, unsigned char vtipo);
typedef void (*radiosetType)(unsigned char* vopt, unsigned char *vvar, unsigned short x, unsigned short y, unsigned char vtipo);
typedef void (*fillinType)(unsigned char* vvar, unsigned short x, unsigned short y, unsigned short pwidth, unsigned char vtipo);
typedef void (*getColorDataType)(MGUI_COLOR *pColor);
typedef unsigned char (*buttonType)(unsigned char* title, unsigned short xib, unsigned short yib, unsigned short pwidth, unsigned short height, unsigned char vtipo);

#define fsGetDirAtuData ((fsGetDirAtuDataType *)(unsigned long)MMSJOS_FUNC_TABLE)[0]
#define fsSetClusterDir ((fsSetClusterDirType *)(unsigned long)MMSJOS_FUNC_TABLE)[1]
#define fsGetClusterDir ((fsGetClusterDirType *)(unsigned long)MMSJOS_FUNC_TABLE)[2]
#define fsSectorWrite ((fsSectorWriteType *)(unsigned long)MMSJOS_FUNC_TABLE)[3]
#define fsSectorRead ((fsSectorReadType *)(unsigned long)MMSJOS_FUNC_TABLE)[4]
#define fsFindDirPath ((fsFindDirPathType *)(unsigned long)MMSJOS_FUNC_TABLE)[5]
#define fsOsCommand ((fsOsCommandType *)(unsigned long)MMSJOS_FUNC_TABLE)[6]
#define fsCreateFile ((fsCreateFileType *)(unsigned long)MMSJOS_FUNC_TABLE)[7]
#define fsOpenFile ((fsOpenFileType *)(unsigned long)MMSJOS_FUNC_TABLE)[8]
#define fsCloseFile ((fsCloseFileType *)(unsigned long)MMSJOS_FUNC_TABLE)[9]
#define fsInfoFile ((fsInfoFileType *)(unsigned long)MMSJOS_FUNC_TABLE)[10]
#define fsFree ((fsFreeType *)(unsigned long)MMSJOS_FUNC_TABLE)[11]
#define fsReadFile ((fsReadFileType *)(unsigned long)MMSJOS_FUNC_TABLE)[12]
#define fsWriteFile ((fsWriteFileType *)(unsigned long)MMSJOS_FUNC_TABLE)[13]
#define fsDelFile ((fsDelFileType *)(unsigned long)MMSJOS_FUNC_TABLE)[14]
#define fsRenameFile ((fsRenameFileType *)(unsigned long)MMSJOS_FUNC_TABLE)[15]
#define loadFile ((loadFileType *)(unsigned long)MMSJOS_FUNC_TABLE)[16]
#define fsMakeDir ((fsMakeDirType *)(unsigned long)MMSJOS_FUNC_TABLE)[17]
#define fsChangeDir ((fsChangeDirType *)(unsigned long)MMSJOS_FUNC_TABLE)[18]
#define fsRemoveDir ((fsRemoveDirType *)(unsigned long)MMSJOS_FUNC_TABLE)[19]
#define OSTimeDlyHMSM ((OSTimeDlyHMSMType *)(unsigned long)MMSJOS_FUNC_TABLE)[20]
#define fsFindInDir ((fsFindInDirType *)(unsigned long)MMSJOS_FUNC_TABLE)[21]
#define fsMalloc ((fsMallocType *)(unsigned long)MMSJOS_FUNC_TABLE)[22]
#define fsFindNextCluster ((fsFindNextClusterType *)(unsigned long)MMSJOS_FUNC_TABLE)[23]
#define fsFindClusterFree ((fsFindClusterFreeType *)(unsigned long)MMSJOS_FUNC_TABLE)[24]
#define fsSetMfp ((fsSetMfpType *)(unsigned long)MMSJOS_FUNC_TABLE)[25]
#define fsGetMfp ((fsGetMfpType *)(unsigned long)MMSJOS_FUNC_TABLE)[26]

#define writesxy ((writesxyType *)(unsigned long)MGUI_FUNC_TABLE)[0]
#define writecxy ((writecxyType *)(unsigned long)MGUI_FUNC_TABLE)[1]
#define locatexy ((locatexyType *)(unsigned long)MGUI_FUNC_TABLE)[2]
#define SaveScreenNew ((SaveScreenNewType *)(unsigned long)MGUI_FUNC_TABLE)[3]
#define RestoreScreen ((RestoreScreenType *)(unsigned long)MGUI_FUNC_TABLE)[4]
#define SetDot ((SetDotType *)(unsigned long)MGUI_FUNC_TABLE)[5]
#define SetByte ((SetByteType *)(unsigned long)MGUI_FUNC_TABLE)[6]
#define FillRect ((FillRectType *)(unsigned long)MGUI_FUNC_TABLE)[7]
#define DrawLine ((DrawLineType *)(unsigned long)MGUI_FUNC_TABLE)[8]
#define DrawRect ((DrawRectType *)(unsigned long)MGUI_FUNC_TABLE)[9]
#define DrawRoundRect ((DrawRoundRectType *)(unsigned long)MGUI_FUNC_TABLE)[10]
#define DrawCircle ((DrawCircleType *)(unsigned long)MGUI_FUNC_TABLE)[11]
#define PutIcone ((PutIconeType *)(unsigned long)MGUI_FUNC_TABLE)[12]
#define InvertRect ((InvertRectType *)(unsigned long)MGUI_FUNC_TABLE)[13]
#define SelRect ((SelRectType *)(unsigned long)MGUI_FUNC_TABLE)[14]
#define PutImage ((PutImageType *)(unsigned long)MGUI_FUNC_TABLE)[15]
#define LoadIconLib ((LoadIconLibType *)(unsigned long)MGUI_FUNC_TABLE)[16]
#define waitButton ((waitButtonType *)(unsigned long)MGUI_FUNC_TABLE)[17]
#define message ((messageType *)(unsigned long)MGUI_FUNC_TABLE)[18]
#define drawButtonsnew ((drawButtonsnewType *)(unsigned long)MGUI_FUNC_TABLE)[19]
#define showWindow ((showWindowType *)(unsigned long)MGUI_FUNC_TABLE)[20]
#define TrocaSpriteMouse ((TrocaSpriteMouseType *)(unsigned long)MGUI_FUNC_TABLE)[21]
#define MostraIcone ((MostraIconeType *)(unsigned long)MGUI_FUNC_TABLE)[22]
#define importFile ((importFileType *)(unsigned long)MGUI_FUNC_TABLE)[23]
#define putImagePbmP4 ((putImagePbmP4Type *)(unsigned long)MGUI_FUNC_TABLE)[24]
#define setPosPressed ((setPosPressedType *)(unsigned long)MGUI_FUNC_TABLE)[25]
#define getMouseData ((getMouseDataType *)(unsigned long)MGUI_FUNC_TABLE)[26]
#define togglebox ((toggleboxType *)(unsigned long)MGUI_FUNC_TABLE)[27]
#define radioset ((radiosetType *)(unsigned long)MGUI_FUNC_TABLE)[28]
#define fillin ((fillinType *)(unsigned long)MGUI_FUNC_TABLE)[29]
#define getColorData ((getColorDataType *)(unsigned long)MGUI_FUNC_TABLE)[30]
#define button ((buttonType *)(unsigned long)MGUI_FUNC_TABLE)[31]

#endif
