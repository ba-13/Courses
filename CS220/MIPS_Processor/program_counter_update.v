// `include "alu_hardware/adder.v"
// `include "accesory_hardware/mux2to1.v"

module program_counter_update (
    input [INSTR_ADDR_SIZE-1:0] current_pc,
    input [INSTR_ADDR_SIZE-1:0] imm_extended,  // this is offset, sign extended
    input [25:0] jump,
    input shouldBranch,
    input shouldJump,
    input alu_zero,
    output [INSTR_ADDR_SIZE-1:0] next_pc
);

  parameter INSTR_ADDR_SIZE = 32;
  wire address_overflow;
  wire [28:0] jump_extended_shifted;
  wire [INSTR_ADDR_SIZE-1:0] imm_extended_shifted;
  wire [INSTR_ADDR_SIZE-1:0] pc_add_1; // we need to shift by only 1, because 1 here signifies the next index in instruction memory, each idx pointing to 32bits
  wire [INSTR_ADDR_SIZE-1:0] pc_with_offset;
  wire [INSTR_ADDR_SIZE-1:0] pc_with_jump;
  wire [INSTR_ADDR_SIZE-1:0] pc_with_or_without_offset;
  wire mustBranch;

  assign mustBranch = shouldBranch & alu_zero;
  //   assign imm_extended_shifted = imm_extended << 2;
  assign imm_extended_shifted = imm_extended;
  //   assign jump_extended_shifted = {2'b00, jump} << 2;
  assign jump_extended_shifted = {2'b00, jump};
  assign pc_with_jump = {pc_add_1[31:28], jump_extended_shifted};

  adder add_1 (
      .A(current_pc),
      .B(1),
      .is_signed(1'b0),
      .carry_in(1'b0),
      .carry_out(address_overflow),
      .out(pc_add_1)
  );

  adder add_offset (
      .A(current_pc),
      .B(imm_extended_shifted),
      .is_signed(1'b1),
      .carry_in(1'b0),
      .carry_out(address_overflow),
      .out(pc_with_offset)
  );

  mux2to1 pc_offset_or_not (
      .select(mustBranch),
      .A(pc_add_1),
      .B(pc_with_offset),
      .Out(pc_with_or_without_offset)
  );

  mux2to1 pc_jump_or_not (
      .select(shouldJump),
      .A(pc_with_or_without_offset),
      .B(pc_with_jump),
      .Out(next_pc)
  );

endmodule
