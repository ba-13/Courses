.text

main:
    ori     $s0,    $zero,  0x10
    ori     $s1,    $zero,  0x1
    ori     $s2,    $zero,  0x0
    ori     $t0,    $zero,  0x0
loop:
    add     $s2,    $zero,  $s1
    add     $s1,    $s1,    $s2
    addi    $t0,    $t0,    0x1
    bne     $t0,    $s0,    loop

    sll     $zero,  $zero,  0x0