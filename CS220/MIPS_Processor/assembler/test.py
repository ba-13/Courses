from parser import parser_instruction
from read_write import read_instruction

jtype = ("j      0x1001", "00001000000000000001000000000001")
itype_lw = ("lw      $t0,    0x10,   $t1", "10001101001010000000000000010000")
itype_sw = ("sw      $t0,    0x10,   $t1", "10101101001010000000000000010000")
itype_bne = ("bne     $s0,    $s6,    0xfff7", "00010110000101101111111111110111")
itype_imm = ("ori     $s1,    $zero,  0x0", "00110100000100010000000000000000")
rtype_sll = ("sll     $t0,    $t1,    0x2", "00000000000010010100000010000000")
rtype_add = ("add     $t0,    $t1,    $t2", "00000001001010100100000000100000")

instructions = [jtype, itype_lw, itype_sw, itype_bne, itype_imm, rtype_sll, rtype_add]
for pair_instruction_binary in instructions:
    instruction, binary = pair_instruction_binary
    tokens = read_instruction(instruction)
    binary_out = parser_instruction((0, tokens), {})
    if binary != binary_out:
        print("Error in instruction: ", instruction)
        print("Expected:\t", binary)
        print("Got:\t\t", binary_out)
    else:
        print("Matched!")
