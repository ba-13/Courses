// `include "memory_hardware/memory.v"

module instruction_memory (
    input [INSTR_MEMORY_ADDR_SIZE-1:0] read_address,
    output [31:0] instruction
);

  localparam read_enable = 1'b1;
  parameter INSTR_MEMORY_ADDR_SIZE = 32;
  wire corrupted;

  memory #(
      .k(INSTR_MEMORY_ADDR_SIZE)
  ) mem (
      .address(read_address),
      .read_enable(read_enable),
      .write_enable(1'b0),
      .data_out(instruction),
      .corrupted(corrupted)
  );

endmodule
