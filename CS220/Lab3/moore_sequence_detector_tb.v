`include "moore_sequence_detector.v"
`timescale 1ns / 1ps

 module moore_sequence_detector_tb();
    // Inputs
    reg in;
    reg clk;
    reg reset;
    // Outputs
    wire out;
    // Instantiate the Unit Under Test (UUT)
    moore_sequence_detector uut (
        .in(in), 
        .clk(clk), 
        .reset(reset), 
        .out(out)
    );

    always #5 clk = ~ clk;  

    initial begin
        $dumpfile("vcd/moore_sequence_detector_tb.vcd");
        $dumpvars(3, moore_sequence_detector_tb);
        // fork                                             // what does fork do
        clk = 1'b0;
        reset = 1'b0;
        // begin 
        in = 1; #10 in = 0; #10 in = 1; #10 in = 0;
        // #10 in = 1; #10 in = 0; #10 in = 1; #10 in = 0;  //! misbehavior
        #10 in = 1; #10 in = 0; #10 in = 1; #10 in = 0;
        #10 in = 1; #10 in = 0; #10 in = 0; #10 in = 1;
        #10 in = 0; #10 in = 1; #10 in = 1; #10 in = 0;
        #10 
        $finish;
        // end
        // join  
    end 

    initial begin
        $monitor("%b %b \t %b", reset, in, out);
        reset = 1'b1;
        #5 reset = 1'b0;
    end
endmodule