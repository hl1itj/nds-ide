@---------------------------------------------------------------------------------
@ DS processor selection
@---------------------------------------------------------------------------------
	.arch	armv5te
	.cpu	arm946e-s
@---------------------------------------------------------------------------------
    .section ".bss"
    .global  _BIN_BUF
@---------------------------------------------------------------------------------
    .align  4
    .arm

@#define BIN_SIZE    1048576
#define BIN_SIZE    524288
#define JMP_SIZE    1024
#define ARM9_START  0x2000000

@_BIN_BUF:
@    .space  BIN_SIZE        @ 1MB of empty space
@_JMP_BUF:
@    .space  JMP_SIZE        @ 1024 Byte of Jump & Copy
    
    .section ".text"
    .global  _copy_bin_and_jump
    .align  4
    .arm
_copy_bin_and_jump:
    ldr     r1, =_copy_and_jump
    ldr     r2, =XXX_BUF_       @ ARM9 Binary Start
    mov     r3, #JMP_SIZE       @ 1024B: sizeof(_copy_and_jump)
loop1:
    ldmia   r1!, {r0}       @ Load  source word
    stmia   r2!, {r0}       @ Store into destination
    subs    r3, r3, #4      @ decrese count by 4
    bne     loop1           @ if undone, loop
    bx      lr
@    bl      _JMP_BUF

    .align  4
    .arm
_copy_and_jump:
@---------------------------------------------------------------------------------
    ldr     r1, =XXX_BUF_
    mov     r2, #ARM9_START @ ARM9 Binary Start
    mov     r3, #BIN_SIZE   @ 1MB sizeof BINBUF
_cloop:
    ldmia   r1!, {r0}       @ Load  source word
    stmia   r2!, {r0}       @ Store into destination
    subs    r3, r3, #4      @ decrese count by 4
    bne     _cloop          @ if undone, loop

    mov     lr, #ARM9_START @ Load link register 0x4000000
    bx      lr

@---------------------------------------------------------------------------------
	.align
	.pool
	.end
@---------------------------------------------------------------------------------
