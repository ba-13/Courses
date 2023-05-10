import sys
import re
from mapping import opcodes, functs, registers
from env import NEXT_INSTRUCTION_JUMP


def mode_instruction(operand: str):
    if operand in ["j", "jal"]:
        return "j"
    elif operand in functs.keys():
        return "r"
    elif operand in opcodes.keys():
        return "i"
    else:
        print("Error reading mode", sys.stderr)
        return None


def int_to_binary_string(number: int, length: int):
    return f"{number:#0{length+2}b}"[2:]


def parser_instruction(addr_instruction: tuple, labels: dict = {}):
    address, instruction = addr_instruction
    opcode = None
    funct = None
    rs = None
    rd = None
    rt = None
    shamt = None
    mode = None

    operand = instruction[0]
    opcode = opcodes[operand]
    mode = mode_instruction(operand)
    if mode == "j":
        # assumed that j offset is the format
        label = instruction[1]
        if label[:2] == "0x":
            # is a direct address
            addr = int(label, 16)
        else:
            # is a label
            addr = labels[label]
        # offset = addr - address + NEXT_INSTRUCTION_JUMP
        offset = addr
        offset = int_to_binary_string(offset, 26)
        machine_code = opcode + offset
        return machine_code
    if mode == "i":
        # handling lw, sw
        if operand in ["lw", "sw"]:
            # format is:
            # lw $rt, offset, $rs
            # offset is in hex
            offset = instruction[2]
            offset = int(offset, 16)
            rt = instruction[1]
            rs = instruction[3]
            machine_code = (
                opcode
                + registers[rs]
                + registers[rt]
                + int_to_binary_string(offset, 16)
            )
            return machine_code
        if operand in ["beq", "bne"]:
            # format is:
            # bne $rs, $rt, offset
            # offset is in hex
            label = instruction[3]
            if label[:2] == "0x":
                # is a direct address
                addr = int(label, 16)
            else:
                # is a label
                addr = labels[label]
            rs = instruction[1]
            rt = instruction[2]
            machine_code = (
                opcode + registers[rs] + registers[rt] + int_to_binary_string(addr, 16)
            )
            return machine_code
        # format is:
        # addi $rt, $rs, imm
        # imm in in hex
        imm = instruction[3]
        imm = int(imm, 16)
        rt = instruction[1]
        rs = instruction[2]
        machine_code = (
            opcode + registers[rs] + registers[rt] + int_to_binary_string(imm, 16)
        )
        return machine_code
    if mode == "r":
        if operand in ["sll", "srl"]:
            # format is:
            # sll $rs, $rt, shamt
            funct = functs[operand]
            shamt = instruction[3]
            shamt = int(shamt, 16)
            rd = instruction[1]
            rt = instruction[2]
            machine_code = (
                opcode
                + "00000"
                + registers[rt]
                + registers[rd]
                + int_to_binary_string(shamt, 5)
                + funct
            )
            return machine_code
        # add $rd, $rs, $rt
        funct = functs[operand]
        rd = instruction[1]
        rs = instruction[2]
        rt = instruction[3]
        shamt = "00000"
        machine_code = (
            opcode + registers[rs] + registers[rt] + registers[rd] + shamt + funct
        )
        return machine_code
    return None
