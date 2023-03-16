.data
strPromptFirst:
.asciiz "Enter first integer value n1: "
strPromptSecond:
.asciiz "Enter second integer value n2: "
strResult:
.asciiz "Sum is: "
strCR:
.asciiz "\n"

.text
.globl main
main:
                                                # STEP 1 -- get the first operand
                                                # Print a prompt asking user for input
    li      $v0,    4                           # syscall number 4 will print string whose address is in $a0
    la      $a0,    strPromptFirst              # "load address" of the string
    syscall                                     # actually print the string

# Now read in the first operand
    li      $v0,    5                           # syscall number 5 will read an int
    syscall                                     # actually read the int
    move    $s0,    $v0                         # save result in $s0 for later

# STEP 2 -- repeat above steps to get the second operand
# First print the prompt
    li      $v0,    4                           # syscall number 4 will print string whose address is in $a0
    la      $a0,    strPromptSecond             # "load address" of the string
    syscall                                     # actually print the string

# Now read in the second operand
    li      $v0,    5                           # syscall number 5 will read an int
    syscall                                     # actually read the int
    move    $s1,    $v0                         # save result in $s1 for later

# STEP 3 -- print the sum
# First print the string prelude
    li      $v0,    4                           # syscall number 4 -- print string
    la      $a0,    strResult
    syscall                                     # actually print the string
                                                # Then print the actual sum
    li      $v0,    1                           # syscall number 1 -- print int
    add     $a0,    $s0,                $s1     # add our operands and put in $a0 for print
    syscall                                     # actually print the int

# STEP 5 -- exit
    li      $v0,    10                          # Syscall number 10 is to terminate the program
    syscall                                     # exit now
