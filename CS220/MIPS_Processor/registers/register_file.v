module register_file (
    input [4:0] read_register_1,
    input [4:0] read_register_2,
    input [4:0] write_register,
    input [REG_SIZE-1:0] write_data,
    input write_register_enable,
    output reg [REG_SIZE-1:0] read_data_1,
    output reg [REG_SIZE-1:0] read_data_2,
    output reg register_writing_done
);

  parameter REG_SIZE = 32;
  parameter REG_COUNT = 32;
  reg [REG_SIZE-1:0] registers[0:REG_COUNT-1];
  integer i;

  initial begin
    $readmemh("registers/registers_init.hex", registers);
    register_writing_done <= 0;
    $display("--------------------------------------------");
    for (i = 0; i < 32; i = i + 1)
      $display("[register_file]: registers[%0d] = \t%h", i, registers[i]);
    $display("--------------------------------------------");
  end

  always @(*) read_data_1 <= registers[read_register_1];
  always @(*) read_data_2 <= registers[read_register_2];
  always @(write_register_enable, write_register, write_data)
    if (write_register_enable) begin
      registers[write_register] <= write_data;
      register_writing_done <= 1;  // disable write register, enable at next clock cycle
      $display("[register_file]: writing %h to register %d", write_data, write_register);
    end

endmodule
