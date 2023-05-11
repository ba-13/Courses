.text
ori     $t0,    $zero,  0x0         # save 0x0 in t0
ori     $t1,    $zero,  0x00012     # save 18 in t1
ori     $t2,    $zero,  0x0000a     # save 10 in t2
sw      $t1,    0x0,    $t0         # store 18 in memory
sw      $t2,    0x1,    $t0         # store 10 in memory

nop     