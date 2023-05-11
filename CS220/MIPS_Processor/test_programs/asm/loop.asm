.text

main:
    ori     $s0,    $zero,  0x11
    ori     $s1,    $zero,  0x2
    ori     $s2,    $zero,  0x1
    ori     $t0,    $zero,  0x0
loop:
    add     $s1,    $s1,    $s2
    add     $s2,    $zero,  $s1
    addi    $t0,    $t0,    0x1
    bne     $t0,    $s0,    loop

    sll     $zero,  $zero,  0x0