#!/usr/bin/env python3

from read_write import read_instructions, write_instructions
from parser import parser_instruction
import argparse

parser = argparse.ArgumentParser(description="Assembler for CSE-BUBBLE")
parser.add_argument(
    "-file",
    type=str,
    help="file to parse and convert to mips-asm, does not handle pseudo instructions",
    required=True,
)
parser.add_argument(
    "-o",
    type=str,
    help="output file, default is out.bin",
    required=False,
    default="out.bin",
)
args = parser.parse_args()

data, instructions, labels = read_instructions(args.file)
print(labels)

codes = []
for addr_instruction in instructions:
    addr, instruction = addr_instruction
    machine_code = parser_instruction(addr_instruction, labels)
    codes.append(machine_code)

write_instructions(codes, args.o)
