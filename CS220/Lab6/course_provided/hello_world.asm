.data
msg:
.asciiz "Hello World"

.text
.globl main
main:
    li      $v0,    4       # syscall 4 (print_string)
    la      $a0,    msg     # argument: string (la is a pseudo-instruction: load address)
                            # loads the address of the label msg into $a0
    syscall                 # print the string
    jr      $ra             # return

