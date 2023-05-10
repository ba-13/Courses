// `include "memory_hardware/memory.v"

module data_memory (
    input [DATA_MEMORY_ADDR_SIZE-1:0] address,
    input read_enable,
    input write_enable,
    input [31:0] data_in,
    output [31:0] data_out
);

  parameter DATA_MEMORY_ADDR_SIZE = 32;
  wire corrupted;

  memory #(
      .k(DATA_MEMORY_ADDR_SIZE)
  ) mem (
      .address(address),
      .read_enable(read_enable),
      .write_enable(write_enable),
      .data_in(data_in),
      .data_out(data_out),
      .corrupted(corrupted)
  );

endmodule
