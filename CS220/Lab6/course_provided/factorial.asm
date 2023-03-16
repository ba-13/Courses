.data
prompt: .asciiz "Input an integer x:\n"
result: .asciiz "Fact(x) = "
.text
main:
                                            # show prompt
    li      $v0,            4
    la      $a0,            prompt
    syscall 
                                            # read x
    li      $v0,            5
    syscall 


# function call
    move    $a0,            $v0
    jal     factorial                       # jump factorial and save position to $ra

    move    $t0,            $v0             # $t0 = $v0
                                            # show prompt
    li      $v0,            4
    la      $a0,            result
    syscall 

# print the result
    li      $v0,            1               # syscall 1 - print int
    move    $a0,            $t0             # $a0 = $t0
    syscall                                 # execute

# return 0
    li      $v0,            10              # $v0 = 10
    syscall 


.text
factorial:
                                            # base case -- still in parent's stack segment
                                            # adjust stack pointer to store return address and argument
    addi    $sp,            $sp,    -8
                                            # save $s0 and $ra
    sw      $s0,            4($sp)
    sw      $ra,            0($sp)
    bne     $a0,            0,      else
    addi    $v0,            $zero,  1       # return 1
    j       fact_return

else:
                                            # backup $a0
    move    $s0,            $a0
    addi    $a0,            $a0,    -1      # x -= 1
    jal     factorial
                                            # when we get here, we already have Fact(x-1) store in $v0
    multu   $s0,            $v0             # return x*Fact(x-1)
    mflo    $v0
fact_return:
    lw      $s0,            4($sp)
    lw      $ra,            0($sp)
    addi    $sp,            $sp,    8
    jr      $ra