`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   11:58:35 01/06/2023
// Design Name:   counter_test
// Module Name:   /home/ise/vm_share/counter/test_counter.v
// Project Name:  counter
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: counter_test
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test_counter;

	// Inputs
	reg clk1;
	reg reset1;

	// Outputs
	wire [3:0] out1;

	// Instantiate the Unit Under Test (UUT)
	counter_design #(.W(10))uut (
		.clk(clk1), 
		.out(out1), 
		.reset(reset1)
	);
	
	initial begin
	clk1<=0;
	forever #10 clk1<=~clk1;
	end

	initial begin
		// Initialize Inputs
		//clk = 0;
		$dumpfile("test.vcd");
		$dumpvars(0,test_counter);
		reset1 = 1;

		// Wait 100 ns for global reset to finish
		#100;
		reset1 = 0;
		
		#2000 $finish;
        
		// Add stimulus here

	end
      
endmodule

