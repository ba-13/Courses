.data

numbers: .word 8, 100, 0, 3, 7, 9, 2, 7, -3, 0  # create array which holds numbers
size: .word 10

.text
main:
    la      $s7,    numbers                     # load address of numbers into $s7
    li      $s0,    0                           # initialize counter 1 for loop 1
    lw      $s6,    size                        # n
    addi    $s6,    $s6,        -1              # n-1
    li      $s1,    0                           # initialize counter 2 for loop 2
loop:
    sll     $t7,    $s1,        2               # multiply $s1 by 2 and put it in t7
    add     $t7,    $s7,        $t7             # add the address of numbers to t7
    lw      $t0,    0($t7)                      # load numbers[j]
    lw      $t1,    4($t7)                      # load numbers[j+1]
    slt     $t2,    $t0,        $t1             # if t0 < t1
    bne     $t2,    $zero,      increment
    sw      $t1,    0($t7)                      # swap
    sw      $t0,    4($t7)
increment:
    addi    $s1,    $s1,        1               # increment t1
    sub     $s5,    $s6,        $s0             # subtract s0 from s6
    bne     $s1,    $s5,        loop            # if s1 (counter for second loop) does not equal 9, loop
    addi    $s0,    $s0,        1               # otherwise add 1 to s0
    li      $s1,    0                           # reset s1 to 0
    bne     $s0,    $s6,        loop            # go back through loop with s1 = s1 + 1

    li      $t3,    0                           # initialize counter for printing
    lw      $t4,    size                        # load size into t4






print:
    beq     $t3,    $t4,        final           # if t3 = t4 go to final

    lw      $t5,    0($s7)                      # load from numbers

    li      $v0,    1                           # print the number
    move    $a0,    $t5
    syscall 

    li      $a0,    32                          # print space
    li      $v0,    11
    syscall 

    addi    $s7,    $s7,        4               # increment through the numbers
    addi    $t3,    $t3,        1               # increment counter

    j       print

final:
    li      $v0,    10                          # end program
    syscall 