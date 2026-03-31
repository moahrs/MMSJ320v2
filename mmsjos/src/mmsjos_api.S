.text
.global start
.global execso
.global _APIfsOsCommand
.global _APIfsFormat
.global _APIfsSetClusterDir
.global _APIfsGetClusterDir
.global _APIfsSectorWrite
.global _APIfsSectorRead
.global _APIfsSendByte
.global _APIfsRecByte
.global _APIfsCreateFile
.global _APIfsOpenFile
.global _APIfsCloseFile
.global _APIfsInfoFile
.global _APIfsRWFile
.global _APIfsReadFile
.global _APIfsWriteFile
.global _APIfsDelFile
.global _APIfsRenameFile
.global _APIloadFile
.global _APIfsMakeDir
.global _APIfsChangeDir
.global _APIfsRemoveDir
.global _APIfsPwdDir
.global _APIfsFindInDir
.global _APIfsUpdateDir
.global _APIfsFindNextCluster
.global _APIfsFindClusterFree

.extern main
.extern fsOsCommand
.extern fsFormat
.extern fsSetClusterDir
.extern fsGetClusterDir
.extern fsSectorWrite
.extern fsSectorRead
.extern fsSendByte
.extern fsRecByte
.extern fsCreateFile
.extern fsOpenFile
.extern fsCloseFile
.extern fsInfoFile
.extern fsRWFile
.extern fsReadFile
.extern fsWriteFile
.extern fsDelFile
.extern fsRenameFile
.extern loadFile
.extern fsMakeDir
.extern fsChangeDir
.extern fsRemoveDir
.extern fsPwdDir
.extern fsFindInDir
.extern fsUpdateDir
.extern fsFindNextCluster
.extern fsFindClusterFree

start:
    lea data(%pc),%a5
    jmp main

execso:
    jmp main

_APIfsOsCommand:      jmp fsOsCommand
_APIfsFormat:         jmp fsFormat
_APIfsSetClusterDir:  jmp fsSetClusterDir
_APIfsGetClusterDir:  jmp fsGetClusterDir
_APIfsSectorWrite:    jmp fsSectorWrite
_APIfsSectorRead:     jmp fsSectorRead
_APIfsSendByte:       jmp fsSendByte
_APIfsRecByte:        jmp fsRecByte
_APIfsCreateFile:     jmp fsCreateFile
_APIfsOpenFile:       jmp fsOpenFile
_APIfsCloseFile:      jmp fsCloseFile
_APIfsInfoFile:       jmp fsInfoFile
_APIfsRWFile:         jmp fsRWFile
_APIfsReadFile:       jmp fsReadFile
_APIfsWriteFile:      jmp fsWriteFile
_APIfsDelFile:        jmp fsDelFile
_APIfsRenameFile:     jmp fsRenameFile
_APIloadFile:         jmp loadFile
_APIfsMakeDir:        jmp fsMakeDir
_APIfsChangeDir:      jmp fsChangeDir
_APIfsRemoveDir:      jmp fsRemoveDir
_APIfsPwdDir:         jmp fsPwdDir
_APIfsFindInDir:      jmp fsFindInDir
_APIfsUpdateDir:      jmp fsUpdateDir
_APIfsFindNextCluster:jmp fsFindNextCluster
_APIfsFindClusterFree:jmp fsFindClusterFree

.section .data
.balign 2
data:
