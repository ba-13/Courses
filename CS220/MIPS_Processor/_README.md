# CS220 MIPS Architecture

Run `mips_tb.v` using the following:

```bash
iverilog -o ./bin/vvp/mips_tb.vvp mips_tb.v
vvp ./bin/vvp/mips_tb.vvp
mv ./mips_tb.vcd ./bin/vcd/mips_tb.vcd
```

To visualise, use:

```bash
gtkwave ./bin/vcd/mips_tb.vcd ./bin/vcd/register_file_debugging.gtkw
```

## Problem faced right now

```bash
[register_file]: writing 00000008 to register 17
step_couter: 40
[register_file]: writing 0000000a to register 21
step_couter: 41
[register_file]: writing 00000012 to register 15
[register_file]: writing 0000000b to register 15
[register_file]: writing 00000009 to register 15 # should not happen multiple times
step_couter: 42
[register_file]: writing 00000009 to register 15
step_couter: 43
[register_file]: writing 00000013 to register  8
step_couter: 44
[register_file]: writing 00000000 to register  9
step_couter: 45
[register_file]: writing 00000000 to register 10
step_couter: 46
[memory]: write 00000000 to address          0 # corrupted address 0 as well
[memory]: write 00000000 to address          9
```

Multiple writes are occuring in both, data memory AND register file in a single clock cycle, despite taking care of enabling write_enable from control unit, upon completion of alu_output (yielding correct address in case of data memory write) and data_read (in case of register file write)

## Problems

- At the start of a CLK, aka posedge, no idea why values of wires reset to values which they should not be in
