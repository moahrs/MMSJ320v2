.text
.global runFromOsCmd
.global _runFromOsCmd
.extern runOSMemory

runFromOsCmd:
_runFromOsCmd:
    movem.l %d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5,-(%a7)
    move.l runOSMemory,%a0
    jsr (%a0)
    movem.l (%a7)+,%d0/%d1/%d2/%d3/%d4/%d5/%d6/%d7/%a0/%a1/%a2/%a3/%a4/%a5
    rts
