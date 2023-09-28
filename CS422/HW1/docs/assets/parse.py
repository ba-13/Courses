num_instructions = """
0 74409297
1 447063179
2 121433930
3 9483820
4 0
5 0
6 0
"""

min_max_exp = """
Maximum memory bytes touched by one instruction 8
Average memory bytes touched by one instruction 1371192306/330430323
Max Imm 2147483647
Min Imm -1431655765
Max Disp 139655605
Min Disp -1392
"""


def parse_num_distribution():
    total_operands = 0
    operands_list = []
    num_operands_instr = num_instructions.split("\n")[1:-1]
    for op_instr in num_operands_instr:
        l = op_instr.split(" ")
        num = int(l[-1], 10)
        total_operands += num
        operands_list.append(num)
        # print(num, end=" & ")
    for op in operands_list:
        print(f"{100*op/total_operands:.3f}", end=" & ")


def parse_min_max():
    statistics_list = min_max_exp.split("\n")[1:-1]
    results_list = [stat.split(" ")[-1] for stat in statistics_list]
    results_list[1] = round(eval(results_list[1]), 3)
    for result in results_list:
        print(result, end=" & ")


if __name__ == "__main__":
    flag = True

    if flag:
        parse_num_distribution()
    else:
        parse_min_max()
