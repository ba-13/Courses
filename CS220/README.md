# Notes

1. Using Non-blocking "<=" assignment in continuous assignment statement is not allowed according to verilog LRM and will result in compilation error.

2. Faced in Lab3:

    ```bash
    mod_10_asynchronous_counter_tb.v:10: error: reg q; cannot be driven by primitives or continuous assignment.
    mod_10_asynchronous_counter_tb.v:10: error: Output port expression must support continuous assignment.
    mod_10_asynchronous_counter_tb.v:10:      : Port 1 (q) of mod_10_asynchronous_counter is connected to q
    2 error(s) during elaboration.
    ```

    in the testbench, the output `q` was declared as reg, converted to a wire.

3. Faced in Lab5:

    Suppose `module M` has an output `Q` of type `reg`
    Top module `module M’` instantiates `M` and passes a reg `R` for `Q`

    > This is illegal, A very common mistake observed in beginners

    *Module M’ can only pass a wire for Q*

    Illegal because this instantiation implicitly connects a register to another register without specifying a clock
    Can connect only a wire to a register through instantiation. This will make sense if you think about the hardware.

4. Keep the module's output as reg, and inputs as wires. This makes sense as a module (block) would only see input as wires, but should save output somewhere.
