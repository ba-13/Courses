.text
.globl main

main:
    la      $s0,    array                               # set $s0 to point array[0]
    li      $s1,    0                                   # $s1 = 0 (for counting the array elements)

# Read in the array elements

loop1:
    li      $v0,    4
    la      $a0,    ask
    syscall                                             # Ask for a number
    li      $v0,    5
    syscall                                             # Read an integer in $v0
    lw      $t0,    endmark
    beq     $v0,    $t0,        loop2                   # if $v0 = endmark then exit loop
    sw      $v0,    0($s0)                              # store $v0 into the current element of the array
    addi    $s0,    $s0,        4                       # move to the next element
    addi    $s1,    $s1,        1                       # increment the counter
    b       loop1

# Sort the array by using the bubble sort algorithm

loop2:
    la      $s0,    array                               # set $s0 to point to array[0]
    add     $s2,    $0,         $s1                     # $s2 = $s1 (the number of elements)
    add     $s2,    $s2,        -1                      # number of swaps = number of elements - 1
    add     $t3,    $0,         $0                      # number of swaps ($t3) = 0

loop3:
    lw      $t0,    0($s0)                              # $t0 = current element
    lw      $t1,    4($s0)                              # $t1 = next element
    slt     $t2,    $t1,        $t0                     # if $t1 < $t0 then $t2 = 1
    beq     $t2,    $0,         skip                    # if $t2 = 0 the go to skip

    sw      $t0,    4($s0)                              # swap the current element and
    sw      $t1,    0($s0)                              # the next element
    addi    $t3,    $t3,        1                       # increment $t3 (number of swaps)

skip:
    add     $s0,    $s0,        4                       # move to the next element
    addi    $s2,    $s2,        -1                      # decrement the element counter
    bgt     $s2,    $0,         loop3                   # if $s2 > 0 then go to loop3

    bne     $t3,    $0,         loop2                   # if number of swaps > 0 then go to loop2

# Print the sorted array

    li      $v0,    4
    la      $a0,    sorted                              # Print "Sorted: "
    syscall 
    la      $s0,    array
loop4:
    lw      $a0,    0($s0)
    li      $v0,    1
    syscall 
    li      $v0,    4
    la      $a0,    nl
    syscall 
    add     $s0,    $s0,        4
    addi    $s1,    $s1,        -1
    bne     $s1,    $0,         loop4

    li      $v0,    4
    la      $a0,    bye                                 # Print the end message
    syscall 
    li      $v0,    5
    syscall                                             # wait for Enter
    li      $v0,    10
    syscall                                             # end of program


.data
ask:     .asciiz "Enter an integer (-999 for exit): "
endmark: .word -999
sorted:  .asciiz "Sorted:\n"
bye:     .asciiz "Press enter to exit..."
nl:      .asciiz "\n"
array:   .word 0                                        # the array starts here