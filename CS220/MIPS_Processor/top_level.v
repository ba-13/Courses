`include "accesory_hardware/mux2to1.v"
`include "alu_hardware/adder.v"
`include "memory_hardware/memory.v"
`include "registers/register_file.v"

`include "alu_control.v"
`include "alu.v"
`include "control.v"
`include "data_memory.v"
`include "instruction_memory.v"
`include "program_counter_update.v"
`include "sign_extend.v"

module top_level (
    input clk,
    input rst,
    output reg done
);

  parameter INSTR_MEMORY_ADDR_SIZE = 32;
  parameter REG_SIZE = 32;
  parameter steps = 20;
  reg [31:0] step_counter;

  // program counter
  reg [INSTR_MEMORY_ADDR_SIZE-1:0] PC;
  wire [INSTR_MEMORY_ADDR_SIZE-1:0] next_pc;

  // instruction and its parts
  wire [REG_SIZE-1:0] instruction;
  wire [25:0] jump;
  wire [5:0] opcode;
  wire [4:0] rs;
  wire [4:0] rt;
  wire [4:0] rd;
  wire [4:0] shamt;
  wire [5:0] funct;
  wire [15:0] imm;
  wire [31:0] imm_extended;

  // control signals
  wire [5:0] alu_op;
  wire reg_dst;
  wire shouldJump;
  wire shouldBranch;
  wire mux_mem_to_registers;
  wire memory_read;
  wire memory_write;
  wire alu_source;
  wire write_register_enable;

  // register file wiring
  wire [4:0] write_register;
  wire [REG_SIZE-1:0] read_data_1;
  wire [REG_SIZE-1:0] read_data_2;
  wire [REG_SIZE-1:0] write_data;
  wire register_writing_done;

  // alu wiring
  wire [REG_SIZE-1:0] alu_input_1;
  wire [REG_SIZE-1:0] alu_input_2;
  wire [3:0] alu_ctrl;
  wire overflow;
  wire alu_zero;
  wire [REG_SIZE-1:0] alu_result;

  // data memory wiring
  wire [REG_SIZE-1:0] data_mem_out;

  assign jump = instruction[25:0];
  assign opcode = instruction[31:26];
  assign rs = instruction[25:21];
  assign rt = instruction[20:16];
  assign rd = instruction[15:11];
  assign shamt = instruction[10:6];
  assign funct = instruction[5:0];
  assign imm = instruction[15:0];

  program_counter_update pc_update (
      .current_pc(PC),
      .imm_extended(imm_extended),
      .jump(jump),
      .shouldBranch(shouldBranch),
      .shouldJump(shouldJump),
      .alu_zero(alu_zero),
      .next_pc(next_pc)
  );

  instruction_memory inst_mem (
      .read_address(PC),
      .instruction (instruction)
  );

  control cnt (
      .opcode(opcode),
      .register_writing_done(register_writing_done),
      .alu_result(alu_result),
      .alu_op(alu_op),
      .reg_dst(reg_dst),
      .jump(shouldJump),
      .branch(shouldBranch),
      .mux_mem_to_registers(mux_mem_to_registers),
      .memory_read(memory_read),
      .memory_write(memory_write),
      .alu_source(alu_source),
      .register_write(write_register_enable),
      .clk(clk)
  );

  mux2to1 #(
      .k(5)
  ) decide_write_register (
      .A(rt),
      .B(rd),
      .select(reg_dst),
      .Out(write_register)
  );

  register_file rfile (
      .read_register_1(rs),
      .read_register_2(rt),
      .write_register(write_register),
      .write_data(write_data),
      .write_register_enable(write_register_enable),
      .read_data_1(read_data_1),
      .read_data_2(read_data_2),
      .register_writing_done(register_writing_done)
  );

  sign_extend se (
      .in (imm),
      .out(imm_extended)
  );

  assign alu_input_1 = read_data_1;

  mux2to1 decide_alu_input_2 (
      .A(read_data_2),
      .B(imm_extended),
      .select(alu_source),
      .Out(alu_input_2)
  );

  alu_control alu_control_ (
      .alu_op(alu_op),
      .funct(funct),
      .alu_control(alu_ctrl)
  );

  alu alu_ (
      .A(alu_input_1),
      .B(alu_input_2),
      .alu_control(alu_ctrl),
      .overflow(overflow),
      .alu_zero(alu_zero),
      .alu_result(alu_result)
  );

  data_memory data_mem (
      .address(alu_result),
      .read_enable(memory_read),
      .write_enable(memory_write),
      .data_in(read_data_2),
      .data_out(data_mem_out)
  );

  mux2to1 decide_write_data (
      .A(alu_result),
      .B(data_mem_out),
      .select(mux_mem_to_registers),
      .Out(write_data)
  );

  initial begin
    PC <= 0;
    step_counter <= 0;
    done <= 0;
  end

  always @(posedge clk) begin
    step_counter = step_counter + 1;
    $display("step_couter: %0d", step_counter);
    if (rst) begin
      PC <= 0;
    end else if (done) begin
      ;
    end else begin
      PC <= next_pc;
      rfile.register_writing_done <= 0;
    end

    if (instruction == 32'h00000000) begin
      done <= 1;
    end
  end

endmodule
