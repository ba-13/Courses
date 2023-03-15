# Lab 1 Notes

<https://www.fpga4student.com/2017/07/n-bit-adder-design-in-verilog.html>

---

> always@( \* ) blocks are used to describe Combinational Logic, or Logic Gates. Only = (blocking) assignments should be used in an always@( \* ) block. Never use <= (non-blocking) assignments in. always@( \* ) blocks

---

An N-dimensional array in Verilog is numbered like this:

```v
reg [15:0] a [3:0][2:0]  ... [12345:0];
    (N+1)th   1st  2nd  ...     Nth
```

---

<https://computationstructures.org/exercises/alu/lab.html>
<https://www.chipverify.com/verilog/verilog-4to1-mux>
