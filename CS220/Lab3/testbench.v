`include "moore_sequence_detector.v"

module testbench;
  reg clk;
  reg data_in;
//   wire [1:0] state;
  wire reset;
  wire detect;

  moore_sequence_detector dut (.clk(clk), .in(data_in), .reset(reset), .out(detect));

  initial begin
    clk = 0;
    data_in = 1'b0;

    // Generate a clock with a 50% duty cycle
    forever #5 clk = ~clk;
  end

  // Provide inputs to the DUT
  initial begin
    data_in = 1'b1; #10 data_in = 1'b0;
    #10 data_in = 1'b0; #10 data_in = 1'b1;
    #10 data_in = 1'b0; #10 data_in = 1'b0;
    #10 data_in = 1'b1; #10 data_in = 1'b0;
    #10 data_in = 1'b1; #10 $finish;
  end

  // Check the output
  initial begin
    $monitor("%b \t %b", data_in, detect); 
    // #15 $finish;
  end
endmodule
