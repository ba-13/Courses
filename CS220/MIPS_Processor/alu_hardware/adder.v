module adder (
    input [REG_SIZE-1:0] A,
    input [REG_SIZE-1:0] B,
    input is_signed,
    input carry_in,
    output carry_out,
    output [REG_SIZE-1:0] out
);

  parameter REG_SIZE = 32;
  wire [REG_SIZE:0] sum;
  assign sum = is_signed ? $signed(A) + $signed(B) + carry_in : A + B + carry_in;
  assign carry_out = sum[REG_SIZE];
  assign out = sum[REG_SIZE-1:0];

endmodule
