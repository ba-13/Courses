.data
space:	.asciiz " "
line:	.asciiz	"\n"
colonsp:	.asciiz ": "
size:	.word	5                                                                               # initial count of the elements in the array.
array:  .space 1000                                                                         # max 250 elements

param_info_string:	.asciiz	"Input number of values to be sorted (0 < N < 1000): "
receive_values_loop_info_string:	.asciiz	"Input each value: "
receive_values_loop_iter_string:	.asciiz	"Input value"
sorted_array_string:	.asciiz "Sorted:"

.text
.globl	main
main:
params_info:
    li      $v0,                    4
    la      $a0,                    param_info_string
    syscall 
params:
    li      $v0,                    5
    syscall 
    la      $t0,                    size                                                    # load address of size to $t0.
    sw      $v0,                    0($t0)                                                  # store returned value in $v0 to size.
receive_values_loop_info:
    li      $v0,                    4
    la      $a0,                    receive_values_loop_info_string
    syscall 
    li      $v0,                    4
    la      $a0,                    line
    syscall 
receive_values_loop_prep:
    la      $t0,                    array                                                   # load array to $t0.
    lw      $t1,                    size                                                    # load size to $t1.
    li      $t2,                    0                                                       # idx starting from 0.
receive_values_loop:
    bge     $t2,                    $t1,                                receive_values_end  # while ($t2 < $t1).
    li      $v0,                    4
    la      $a0,                    receive_values_loop_iter_string
    syscall 
    li      $v0,                    4
    la      $a0,                    colonsp
    syscall 

    li      $v0,                    6
    syscall 
    s.s     $f0,                    0($t0)

    addi    $t0,                    $t0,                                4                   # increment array pointer by 4.
    addi    $t2,                    $t2,                                1                   # increment loop runner by 1.
    j       receive_values_loop                                                             # jump back to the beginning of the loop.
receive_values_end:
    jal     print                                                                           # call print routine.

sort_prep:
    la      $t0,                    array                                                   # load array to $t0.
    lw      $t1,                    size                                                    # load array size to $t1.
    li      $t2,                    1                                                       # loop runner, starting from 1.
sort_xloop:
    la      $t0,                    array                                                   # load array to $t0.
    bge     $t2,                    $t1,                                sort_xloop_end      # while (t2 < $t1).
    move    $t3,                    $t2                                                     # copy $t2 to $t3.
sort_iloop:
    la      $t0,                    array                                                   # load array to $t0.
    sll     $t5,                    $t3,                                2                   # multiply $t3 with 4, and store in $t5
    add     $t0,                    $t0,                                $t5                 # add the array address with $t5, which is the index multiplied with 4.
    ble     $t3,                    $zero,                              sort_iloop_end      # while (t3 > 0).
    l.s     $f12,                   0($t0)                                                  # load array[$t3] to $f12.
    l.s     $f11,                   -4($t0)
    c.lt.s  $f11,                   $f12                                                    # load array[$t3 - 1] to $f11.
    bc1t    sort_iloop_end                                                                  # while (array[$t3] < array[$t3 - 1]).
    l.s     $f13,                   0($t0)                                                  # swap
    s.s     $f11,                   0($t0)
    s.s     $f13,                   -4($t0)
    addi    $t3,                    $t3,                                -1
    j       sort_iloop
sort_iloop_end:
    addi    $t2,                    $t2,                                1
    j       sort_xloop
sort_xloop_end:
    li      $v0,                    4
    la      $a0,                    sorted_array_string
    syscall 
    li      $v0,                    4
    la      $a0,                    line
    syscall 
    jal     print
exit:
    li      $v0,                    10
    syscall 

print:
print_loop_prep:
    la      $t0,                    array
    lw      $t1,                    size
    li      $t2,                    0
print_loop:
    bge     $t2,                    $t1,                                print_end
    li      $v0,                    2
    l.s     $f12,                   0($t0)
    syscall 
    li      $v0,                    4
    la      $a0,                    space
    syscall 
    addi    $t0,                    $t0,                                4
    addi    $t2,                    $t2,                                1
    j       print_loop
print_end:
    li      $v0,                    4
    la      $a0,                    line
    syscall 
    jr      $ra