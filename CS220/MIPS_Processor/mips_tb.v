// `timescale 1ps / 1ps
`include "top_level.v"

module mips_tb;

  // Inputs
  parameter steps = 22;
  parameter size_array = 10;
  reg clock;
  reg rst;
  wire done;
  integer i;

  // Instantiate the Unit Under Test (UUT)
  top_level #(
      .steps(steps)
  ) uut (
      .clk (clock),
      .rst (rst),
      .done(done)
  );

  initial begin
    $dumpfile("mips_tb.vcd");
    $dumpvars(0, mips_tb);
  end

  initial begin
    $readmemb("test_programs/bin/out.bin", uut.inst_mem.mem.memory, 0, steps - 1);
    $readmemh("test_programs/bubble_sort_data.hex", uut.data_mem.mem.memory, 0, size_array - 1);
    $display("--------------------------------------------");
    for (i = 0; i < 10; i = i + 1)
      $display("Data Memory at address %0d: %h", i, uut.data_mem.mem.memory[i]);
    $display("--------------------------------------------");
    for (i = 0; i < steps; i = i + 1)
      $display("Instruction Memory at address %0d: %b", i, uut.inst_mem.mem.memory[i]);
    $display("--------------------------------------------");
    // $monitor("[ALU] Result: %h", uut.alu_result);
  end

  // always #0.5 clock = ~clock;

  initial begin
    // Initialize Inputs  
    #1;
    clock <= 0;
    rst   <= 0;
    #1;
    while (!done) begin
      #0.5 clock <= ~clock;
    end
    #150 $display("--------------------------------------------");
    $display("Final state:");
    $display("--------------------------------------------");
    for (i = 0; i < 32; i = i + 1) begin
      $display("Registers at address %0d: \t%h", i, uut.rfile.registers[i]);
    end
    $display("--------------------------------------------");
    for (i = 0; i < 10; i = i + 1) begin
      $display("Data Memory at address %0d: \t%h", i, uut.data_mem.mem.memory[i]);
    end
    $display("--------------------------------------------");
    $finish;
  end

endmodule
