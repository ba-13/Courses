.data

numbers: .word 8, 100, 0, 3, 7, 9, 2, 7, -3, 0  # create array which holds numbers
size: .word 10

.text
    bne     $at,    $zero,  main                # 00 for checking bne working
    ori     $zero,  $zero,  0x0                 # 00 for checking ori working
main:
    ori     $s7,    $zero,  0x1                 # 01 load first address of array
    ori     $s0,    $zero,  0x0                 # 02 counter 1
    ori     $t0,    $zero,  0x0                 # 03
    lw      $s6,    0x0,    $t0                 # 04 load s6 = size of array
    addi    $s6,    $s6,    -0x1                # 05 s6 = size - 1
    ori     $s1,    $zero,  0x0                 # 06 counter 2
loop:                                           # 07 0x7
    add     $t7,    $s7,    $s1                 # 08 load address offset
    lw      $t0,    0x0,    $t7                 # 09 load number at that offset
    lw      $t1,    0x1,    $t7                 # 10 load next number
    slt     $t2,    $t0,    $t1                 # 11
    bne     $t2,    $zero,  increment           # 12
    sw      $t1,    0x0,    $t7                 # 13
    sw      $t0,    0x1,    $t7                 # 14
increment:                                      # 15 0xf
    addi    $s1,    $s1,    0x1                 # 16
    sub     $s5,    $s6,    $s0                 # 17
    bne     $s1,    $s5,    loop                # 18 jump to 0x7
    addi    $s0,    $s0,    0x1                 # 19
    ori     $s1,    $zero,  0x0                 # 20
    bne     $s0,    $s6,    loop                # 21

    sll     $zero,  $zero,  0x0                 # 22