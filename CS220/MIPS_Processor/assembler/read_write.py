import re
from env import NEXT_INSTRUCTION_JUMP


def read_data(line: str):
    line = line.split("#")[0].strip()  # remove comment part
    t = line.split(":")
    tag = t[0]
    value = t[1].strip()
    return tag, value


def read_instruction(line: str):
    line = line.split("#")[0].strip()
    tokens = re.split(r"[,]*\s+", line)
    return tokens


def read_instructions(filepath: str):
    data = {}
    raw_instructions = []
    ORG = 0x00
    with open(filepath, "r") as f:
        read_text_ = False
        read_data_ = False
        for line in f.readlines():
            line = line.strip()
            if line == "":  # handle empty lines
                continue
            if line[0] == ".":  # handle directives
                if ".text" in line:
                    read_text_ = True
                    read_data_ = False
                    continue
                elif ".data" in line:
                    read_data_ = True
                    read_text_ = False
                    continue

            if read_data_:
                tag, value = read_data(line)
                data[tag] = value
            if read_text_:
                tokens = read_instruction(line)
                raw_instructions.append(tokens)

    curr_addr = ORG
    labels = {}
    instructions = []
    for raw_instruction in raw_instructions:
        if "nop" in raw_instruction[0]:
            raw_instruction = ["sll", "$zero", "$zero", "0x0"]
        elif len(raw_instruction) == 1:
            labels[raw_instruction[0][:-1]] = curr_addr
            continue
        pair = (curr_addr, raw_instruction)
        instructions.append(pair)
        curr_addr += NEXT_INSTRUCTION_JUMP

    return data, instructions, labels


def write_instructions(binary_strings, filepath):
    with open(filepath, "w") as f:
        for instruction in binary_strings:
            f.write(instruction + "\n")
