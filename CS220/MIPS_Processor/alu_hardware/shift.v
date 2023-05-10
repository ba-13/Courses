module srl (
    input  [REG_SIZE-1:0] A,
    input  [IMM_SIZE-1:0] B,
    output [REG_SIZE-1:0] out
);

  parameter REG_SIZE = 32;
  parameter IMM_SIZE = 5;  // maximum shift by 31
  assign out = A >> B;

endmodule

module sll (
    input  [REG_SIZE-1:0] A,
    input  [IMM_SIZE-1:0] B,
    output [REG_SIZE-1:0] out
);

  parameter REG_SIZE = 32;
  parameter IMM_SIZE = 5;  // maximum shift by 31
  assign out = A << B;

endmodule
