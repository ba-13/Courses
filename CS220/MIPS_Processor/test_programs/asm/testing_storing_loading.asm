.text
main:

    ori     $s0,    $zero,  0x00a
    ori     $s1,    $zero,  0x00b
    sw      $s0,    0x0,    $zero
    sw      $s1,    0x1,    $zero
    lw      $s2,    0x0,    $zero
    lw      $s3,    0x1,    $zero

    sll     $zero,  $zero,  0x0