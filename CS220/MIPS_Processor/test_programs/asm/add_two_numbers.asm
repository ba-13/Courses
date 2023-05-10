.text

main:
    ori     $t0,    $zero,  0x1234
    ori     $t1,    $zero,  0x5678
    add     $t2,    $t0,    $t1

    sll     $zero,  $zero,  0x0