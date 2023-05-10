`include "top_level.v"

module mips_tb;

  parameter steps = 9;
  parameter size_array = 10;

  reg clk;
  reg rst;
  wire done;
  integer i;

  top_level #(
      .steps(steps)
  ) uut (
      .clk (clk),
      .rst (rst),
      .done(done)
  );

  initial begin
    $dumpfile("mips_tb.vcd");
    $dumpvars(0, mips_tb);
  end

  initial begin
    $readmemb("test_programs/bin/loop.bin", uut.inst_mem.mem.memory, 0, steps - 1);
    $display("--------------------------------------------");
    for (i = 0; i < steps; i = i + 1)
      $display("Instruction Memory at address %0d: \t%b", i, uut.inst_mem.mem.memory[i]);
    $display("--------------------------------------------");
  end

  always #0.5 clk = ~clk;

  initial begin
    // Initialize Inputs  
    #1;
    clk <= 0;
    rst <= 0;
    #1;
    // while (!done) begin
    //   #0.5 clk <= ~clk;
    // end
    #50;
    $display("--------------------------------------------");
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
