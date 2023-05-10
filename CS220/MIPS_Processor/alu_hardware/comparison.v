module comparison_slt (
    input [REG_SIZE-1:0] A,
    input [REG_SIZE-1:0] B,
    input is_signed,
    output [REG_SIZE-1:0] out
);

  parameter REG_SIZE = 32;
  assign out = is_signed ? $signed(A) < $signed(B) : A < B;

endmodule

module comparison_seq (
    input  [REG_SIZE-1:0] A,
    input  [REG_SIZE-1:0] B,
    output [REG_SIZE-1:0] out
);

  parameter REG_SIZE = 32;
  assign out = A == B;

endmodule

module comparison_sne (
    input  [REG_SIZE-1:0] A,
    input  [REG_SIZE-1:0] B,
    output [REG_SIZE-1:0] out
);

  parameter REG_SIZE = 32;
  assign out = A != B;

endmodule
