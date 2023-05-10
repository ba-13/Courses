module logical_and (
    input  [REG_SIZE-1:0] A,
    input  [REG_SIZE-1:0] B,
    output [REG_SIZE-1:0] out
);

  parameter REG_SIZE = 32;
  assign out = A & B;

endmodule

module logical_or (
    input  [REG_SIZE-1:0] A,
    input  [REG_SIZE-1:0] B,
    output [REG_SIZE-1:0] out
);

  parameter REG_SIZE = 32;
  assign out = A | B;

endmodule

module logical_nor (
    input  [REG_SIZE-1:0] A,
    input  [REG_SIZE-1:0] B,
    output [REG_SIZE-1:0] out
);

  parameter REG_SIZE = 32;
  assign out = ~(A | B);

endmodule

module logical_xor (
    input  [REG_SIZE-1:0] A,
    input  [REG_SIZE-1:0] B,
    output [REG_SIZE-1:0] out
);

  parameter REG_SIZE = 32;
  assign out = (A ^ B);

endmodule
