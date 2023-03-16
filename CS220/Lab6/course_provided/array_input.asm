.data
myarray:
.space 12
st:
.asciiz "Enter the 3 Elements"

.text
main:
    li      $v0,    4
    la      $a0,    st
    syscall                                 # print st
    addi    $sp,    $sp,            -4
    sw      $t0,    0($sp)                  # store t0 in stack
    li      $t0,    0                       # clear t0
    jal     fun                             # jump to fun, saving return at $ra
    li      $t0,    0                       # clear t0 again
    jal     disp                            # jump to disp
    lw      $t0,    0($sp)
    addi    $sp,    $sp,            4       # load back t0
    li      $v0,    10
    syscall                                 # exit

disp:
    li      $v0,    1
    la      $a0,    myarray($t0)
    syscall 
    beq     $t0,    12,             exit
    add     $t0,    $t0,            4
    j       disp

fun:
    li      $v0,    5
    syscall                                 # read int, inputted int stored in v0
    beq     $t0,    12,             exit    # jump if t0 == 40, to exit
    sw      $v0,    myarray($t0)
    add     $t0,    $t0,            4
    j       fun

exit:
    jr      $ra
