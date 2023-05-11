#!/usr/bin/env python3

from read_write import read_instructions, write_instructions
from parser import parser_instruction
import argparse
import os

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
parser.add_argument(
    "-d",
    action='store_true',
    help="default location of binary file is in bin folder of the file's directory",
    required=False,
    default=False,
)
args = parser.parse_args()

data, instructions, labels = read_instructions(args.file)
print("Labels detected:", labels)

codes = []
for addr_instruction in instructions:
    addr, instruction = addr_instruction
    machine_code = parser_instruction(addr_instruction, labels)
    codes.append(machine_code)

if args.d == False:
    write_instructions(codes, args.o)
else:
    path, filename = os.path.split(args.file)
    os.makedirs(os.path.join(path, "bin"), exist_ok=True)
    write_instructions(codes, os.path.join(path, "bin", filename[:-4] + ".bin"))