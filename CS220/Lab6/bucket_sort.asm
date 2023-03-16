.data
promptEnterN: .asciiz "Enter number of numbers: "
promptEnterNumber: .asciiz "Enter Float Number: "
promptNumbers: .asciiz "Numbers you entered: "
promptSorted: .asciiz "Sorted Numbers:\n"
promptError: .asciiz "[ERR]: Wrong place to be in.\n"
space:	.asciiz " "
line:	.asciiz	"\n"
colonsp:	.asciiz ": "
size:   .word 3
arr:    .space 1000
bucket_0: .space 1000
bucket_1: .space 1000
bucket_2: .space 1000
bucket_3: .space 1000
bucket_4: .space 1000
b0: .float 0.2
b1: .float 0.4
b2: .float 0.6
b3: .float 0.8
b4: .float 1.0

.text
main:
    la      $a0,                promptEnterN
    li      $v0,                4
    syscall                                                             # prompt for number of numbers
    li      $v0,                5
    syscall                                                             # take in n
    la      $t0,                size
    sw      $v0,                0($t0)                                  # save n in size
    lw      $s5,                size                                    # save n in $t5
    la      $s0,                arr                                     # point $s0 at arr
    li      $t1,                0                                       # make $t1 as idx keeper

input_loop_main:
    la      $a0,                promptEnterNumber
    li      $v0,                4
    syscall                                                             # prompt for entering number
    li      $v0,                6
    syscall                                                             # Read the user's input
    mov.s   $f0,                $f0                                     # Move input value from $f0 to $f0 to ensure it's loaded properly
    s.s     $f0,                ($s0)                                   # Store the input value in the arr
    addi    $t1,                $t1,                1
    addi    $s0,                $s0,                4                   # Increment the index

    bne     $s5,                $t1,                input_loop_main
    la      $a0,                arr
    lw      $s0,                size
    jal     print_arr
    jal     bucket_sort
    la      $a0,                arr
    lw      $s0,                size
    jal     print_arr
    j       exit

bucket_sort:
    addi    $sp,                $sp,                -4
    sw      $ra,                0($sp)
                                                                        # handle used registers in stack
    la      $s0,                arr
    lw      $s5,                size
    li      $t1,                0
init_bs:
    la      $s2,                bucket_0
    sw      $zero,              0($s2)
    la      $s2,                bucket_1
    sw      $zero,              0($s2)
    la      $s2,                bucket_2
    sw      $zero,              0($s2)
    la      $s2,                bucket_3
    sw      $zero,              0($s2)
    la      $s2,                bucket_4
    sw      $zero,              0($s2)                                  # initialize index to be 0 for each bucket
loop_fb_bs:                                                             # fb stands for fill_bucket
    l.s     $f12,               0($s0)                                  # load the element at $t1 th index
    lwc1    $f10,               b0
    c.lt.s  $f12,               $f10
    bc1t    fb_0                                                        # case of filling bucket_0
    lwc1    $f10,               b1
    c.lt.s  $f12,               $f10
    bc1t    fb_1                                                        # case of filling bucket_1
    lwc1    $f10,               b2
    c.lt.s  $f12,               $f10
    bc1t    fb_2                                                        # case of filling bucket_2
    lwc1    $f10,               b3
    c.lt.s  $f12,               $f10
    bc1t    fb_3                                                        # case of filling bucket_3
    lwc1    $f10,               b4
    j       fb_4

    nop     
    c.le.s  $f12,               $f10
    bc1t    fb_4                                                        # case of filling bucket_4
    li      $v0,                4
    la      $a0,                promptError                             # error, should not have a number greater than 1.0
    syscall 
    j       exit
    nop     

fb_0:
    lw      $s6,                bucket_0                                # load idx to fill next in that bucket
    addi    $s6,                $s6,                1                   # add 1
    la      $s7,                bucket_0
    sll     $s4,                $s6,                2
    add     $s7,                $s7,                $s4                 # base address + offset
    s.s     $f12,               0($s7)                                  # save element inplace
    sw      $s6,                bucket_0
    j       contd_fb_bs
fb_1:
    lw      $s6,                bucket_1
    addi    $s6,                $s6,                1                   # load index of bucket_1, and add 1
    la      $s7,                bucket_1
    sll     $s4,                $s6,                2
    add     $s7,                $s7,                $s4
    s.s     $f12,               0($s7)
    sw      $s6,                bucket_1
    j       contd_fb_bs
fb_2:
    lw      $s6,                bucket_2
    addi    $s6,                $s6,                1                   # load index of bucket_2, and add 1
    la      $s7,                bucket_2
    sll     $s4,                $s6,                2
    add     $s7,                $s7,                $s4
    s.s     $f12,               0($s7)
    sw      $s6,                bucket_2
    j       contd_fb_bs
fb_3:
    lw      $s6,                bucket_3
    addi    $s6,                $s6,                1                   # load index of bucket_3, and add 1
    la      $s7,                bucket_3
    sll     $s4,                $s6,                2
    add     $s7,                $s7,                $s4
    s.s     $f12,               0($s7)
    sw      $s6,                bucket_3
    j       contd_fb_bs
fb_4:
    lw      $s6,                bucket_4
    addi    $s6,                $s6,                1                   # load index of bucket_4, and add 1
    la      $s7,                bucket_4
    sll     $s4,                $s6,                2
    add     $s7,                $s7,                $s4
    s.s     $f12,               0($s7)
    sw      $s6,                bucket_4
    j       contd_fb_bs

contd_fb_bs:
    addi    $t1,                $t1,                1
    addi    $s0,                $s0,                4
    bne     $t1,                $s5,                loop_fb_bs

sort_bs:
    la      $a0,                bucket_0
    addi    $a0,                4
    lw      $s0,                bucket_0
    jal     insertion_sort
    la      $a0,                bucket_1
    addi    $a0,                4
    lw      $s0,                bucket_1
    jal     insertion_sort
    la      $a0,                bucket_2
    addi    $a0,                4
    lw      $s0,                bucket_2
    jal     insertion_sort
    la      $a0,                bucket_3
    addi    $a0,                4
    lw      $s0,                bucket_3
    jal     insertion_sort
    la      $a0,                bucket_4
    addi    $a0,                4
    lw      $s0,                bucket_4
    jal     insertion_sort

    nop     
    jal     print_buckets_bs
    nop     

rewrite_bs:
    li      $t1,                0                                       # initialise index of arr
    la      $a0,                bucket_0
    addi    $a0,                4
    lw      $s0,                bucket_0
    jal     fill_arr
    la      $a0,                bucket_1
    addi    $a0,                4
    lw      $s0,                bucket_1
    jal     fill_arr
    la      $a0,                bucket_2
    addi    $a0,                4
    lw      $s0,                bucket_2
    jal     fill_arr
    la      $a0,                bucket_3
    addi    $a0,                4
    lw      $s0,                bucket_3
    jal     fill_arr
    la      $a0,                bucket_4
    addi    $a0,                4
    lw      $s0,                bucket_4
    jal     fill_arr
end_bs:
    lw      $ra,                0($sp)
    addi    $sp,                $sp,                4
    jr      $ra


# function that takes in $t1 as the index of arr, $a0 and $s0 as the array to fill arr
fill_arr:
    addi    $sp,                $sp,                -12
    sw      $a1,                8($sp)
    sw      $a0,                4($sp)
    sw      $t3,                0($sp)                                  # save to stack
    la      $a1,                arr                                     # a1 points to arr
    sll     $t7,                $t1,                2
    add     $a1,                $a1,                $t7                 # address + offset
    li      $t3,                0                                       # stores the index of bucket array
                                                                        # t1 stores the index of arr
loop_fa:
    bge     $t3,                $s0,                loop_end_fa
    l.s     $f10,               0($a0)
    s.s     $f10,               0($a1)
    addi    $a0,                $a0,                4
    addi    $t1,                $t1,                1
    addi    $a1,                $a1,                4
    addi    $t3,                $t3,                1
    j       loop_fa

loop_end_fa:
    lw      $a1,                8($sp)
    lw      $a0,                4($sp)
    lw      $t3,                0($sp)
    addi    $sp,                $sp,                12
    jr      $ra

# insertion sort: a0, s0 has array pointer and size respectively
.globl insertion_sort
insertion_sort:
    add     $t0,                $a0,                $zero               # load arr to $t0.
    add     $t1,                $s0,                $zero               # load arr size to $t1.
    li      $t2,                1                                       # loop runner, starting from 1.
xloop_is:
    add     $t0,                $a0,                $zero               # load arr to $t0.
    bge     $t2,                $t1,                end_xloop_is        # while (t2 < $t1).
    move    $t3,                $t2                                     # copy $t2 to $t3.
iloop_is:
    add     $t0,                $a0,                $zero               # load arr to $t0.
    sll     $t5,                $t3,                2                   # multiply $t3 with 4, and store in $t5
    add     $t0,                $t0,                $t5                 # add the arr address with $t5, which is the index multiplied with 4.
    ble     $t3,                $zero,              end_iloop_is        # while (t3 > 0).
    l.s     $f12,               0($t0)                                  # load arr[$t3] to $f12.
    l.s     $f11,               -4($t0)
    c.lt.s  $f11,               $f12                                    # load arr[$t3 - 1] to $f11.
    bc1t    end_iloop_is                                                # while (arr[$t3] < arr[$t3 - 1]).
    l.s     $f13,               0($t0)                                  # swap
    s.s     $f11,               0($t0)
    s.s     $f13,               -4($t0)
    addi    $t3,                $t3,                -1
    j       iloop_is
end_iloop_is:
    addi    $t2,                $t2,                1
    j       xloop_is
end_xloop_is:
    jr      $ra

# exit cleanly
exit:
    li      $v0,                10
    syscall 

# print buckets
print_buckets_bs:
    addi    $sp,                $sp,                -12
    sw      $ra,                8($sp)
    sw      $s0,                4($sp)                                  # save on stack
    sw      $a0,                0($sp)
    la      $a0,                bucket_0
    addi    $a0,                4                                       # shift to place storing number
    lw      $s0,                bucket_0
    jal     print_arr
    la      $a0,                bucket_1
    addi    $a0,                4                                       # shift to place storing number
    lw      $s0,                bucket_1
    jal     print_arr
    la      $a0,                bucket_2
    addi    $a0,                4                                       # shift to place storing number
    lw      $s0,                bucket_2
    jal     print_arr
    la      $a0,                bucket_3
    addi    $a0,                4                                       # shift to place storing number
    lw      $s0,                bucket_3
    jal     print_arr
    la      $a0,                bucket_4
    addi    $a0,                4                                       # shift to place storing number
    lw      $s0,                bucket_4
    jal     print_arr
    lw      $a0,                0($sp)
    lw      $s0,                4($sp)
    lw      $ra,                8($sp)
    addi    $sp,                $sp,                12
    jr      $ra

# print_arr function, needs arr address and size to be in $a0 and $s0
.globl print_arr
print_arr:
print_loop_prep:
    addi    $sp,                $sp,                -20
    sw      $a0,                16($sp)
    sw      $v0,                12($sp)
    sw      $t0,                8($sp)
    sw      $t1,                4($sp)
    sw      $t2,                0($sp)

    add     $t0,                $a0,                $zero               # load a0 value to t0, equivalent to address of array passed
    add     $t1,                $s0,                $zero               # load s0 to t1, equivalent to size of array
    li      $t2,                0
print_loop:
    bge     $t2,                $t1,                print_end
    li      $v0,                2
    l.s     $f12,               0($t0)
    syscall 
    li      $v0,                4
    la      $a0,                space
    syscall 
    addi    $t0,                $t0,                4
    addi    $t2,                $t2,                1
    j       print_loop
print_end:
    li      $v0,                4
    la      $a0,                line
    syscall 

    lw      $t2,                0($sp)
    lw      $t1,                4($sp)
    lw      $t0,                8($sp)
    lw      $v0,                12($sp)
    lw      $a0,                16($sp)
    addi    $sp,                $sp,                20
    jr      $ra
