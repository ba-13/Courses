// `include "alu_hardware/adder.v"
`include "alu_hardware/comparison.v"
`include "alu_hardware/logical.v"
`include "alu_hardware/shift.v"

module alu (
    input [REG_SIZE-1:0] A,
    input [REG_SIZE-1:0] B,
    input [3:0] alu_control,
    output overflow,
    output alu_zero,
    output reg [REG_SIZE-1:0] alu_result
);

  parameter REG_SIZE = 32;
  reg is_signed;
  reg carry_in;

  wire [REG_SIZE-1:0] addition;
  wire [REG_SIZE-1:0] shift_left;
  wire [REG_SIZE-1:0] shift_right;
  wire [REG_SIZE-1:0] andition;
  wire [REG_SIZE-1:0] orition;
  wire [REG_SIZE-1:0] xorition;
  wire [REG_SIZE-1:0] norition;
  wire [REG_SIZE-1:0] sltition;
  wire [REG_SIZE-1:0] seqition;

  adder add (
      .A(A),
      .B(B),
      .is_signed(is_signed),
      .carry_in(carry_in),
      .out(addition),
      .carry_out(overflow)
  );

  sll sll1 (
      .A  (A),
      .B  (B[10:6]),
      .out(shift_left)
  );

  srl srl1 (
      .A  (A),
      .B  (B[10:6]),
      .out(shift_right)
  );

  logical_and and1 (
      .A  (A),
      .B  (B),
      .out(andition)
  );

  logical_or or1 (
      .A  (A),
      .B  (B),
      .out(orition)
  );

  logical_xor xor1 (
      .A  (A),
      .B  (B),
      .out(xorition)
  );

  logical_nor nor1 (
      .A  (A),
      .B  (B),
      .out(norition)
  );

  comparison_slt slt1 (
      .A(A),
      .B(B),
      .is_signed(is_signed),
      .out(sltition)
  );

  comparison_seq seq1 (
      .A  (A),
      .B  (B),
      .out(seqition)
  );


  always @(*) begin
    case (alu_control)
      4'b0000: begin
        alu_result = shift_left;  // sll
      end
      4'b0001: begin
        alu_result = shift_right;  // srl
      end
      4'b0010: begin
        carry_in   = 0;
        is_signed  = 1;
        alu_result = addition;  // add
      end
      4'b0011: begin
        carry_in   = 0;
        is_signed  = 0;
        alu_result = addition;  // addu
      end
      4'b0100: begin
        carry_in   = 1;
        is_signed  = 1;
        alu_result = addition;  // sub
      end
      4'b0101: begin
        carry_in   = 1;
        is_signed  = 0;
        alu_result = addition;  // subu
      end
      4'b0110: begin
        alu_result = andition;  // and
      end
      4'b0111: begin
        alu_result = orition;  // or
      end
      4'b1000: begin
        alu_result = xorition;  // xor
      end
      4'b1001: begin
        alu_result = norition;  // nor
      end
      4'b1010: begin
        is_signed  = 1;
        alu_result = sltition;  // slt
      end
      4'b1011: begin
        is_signed  = 0;
        alu_result = sltition;  // sltu
      end
      4'b1100: begin
        alu_result = seqition;  // seq
      end
      default: begin
        alu_result = 0;
        $display("[ALU]: Error in alu_control field: %b", alu_control);
      end
    endcase
  end

  assign alu_zero = alu_result == 0;

endmodule
