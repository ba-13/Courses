`include "memory_hardware/memory.v"
`include "data_memory.v"

module tb_data_memory;

  parameter DATA_MEMORY_ADDR_SIZE = 32;
  // Testbench inputs
  reg [DATA_MEMORY_ADDR_SIZE-1:0] address;
  reg read_enable;
  reg write_enable;
  reg [31:0] data_in;

  // Testbench outputs
  wire [31:0] data_out;

  // Instantiate the DUT (data_memory module)
  data_memory uut (
      .address(address),
      .read_enable(read_enable),
      .write_enable(write_enable),
      .data_in(data_in),
      .data_out(data_out)
  );

  // Data file read
  integer i;
  initial begin
    $readmemh("test_programs/bubble_sort_data.hex", uut.mem.memory, 0, 10);
    for (i = 0; i < 10; i = i + 1) $display("Data at address %d: %h", i, uut.mem.memory[i]);
  end

  // Testbench stimulus
  initial begin
    // Initialize inputs
    address = 0;
    read_enable = 1;
    write_enable = 0;
    data_in = 0;
    #10;

    // Read data from data memory and display
    $display("Data at address %0d: %h", address, data_out);

    // Change inputs
    address = 1;
    read_enable = 0;
    write_enable = 1;
    data_in = 32'h00000048;

    // Write data to data memory
    #10;  // Wait for 10 time units
    $display("Writing data %h to address %0d", data_in, address);

    // Read data from data memory and display
    read_enable = 1;
    #10;  // Wait for 10 time units
    $display("Data at address %0d: %h", address, data_out);
  end

endmodule
