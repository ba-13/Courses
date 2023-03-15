`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   22:00:22 01/18/2023
// Design Name:   level_triggered_jk
// Module Name:   /home/ise/vm_share/one_bit_full_adder/test_level_jk.v
// Project Name:  one_bit_full_adder
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: level_triggered_jk
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test_level_jk;

	// Inputs
	reg j;
	reg k;
	reg clk;
	reg clr;

	// Outputs
	wire q;
	wire q_bar;
	
	initial begin
	clk<=0;
	forever #1000 clk<=~clk;
  end

initial begin

$dumpfile("test.vcd");
$dumpvars(0,test_level_jk);

clr<=1;
j<=0;
k<=1;

#100 
clr<=0;

#3001 

j<=1;

#100000
$finish;
end

	// Instantiate the Unit Under Test (UUT)
/*level_triggered_jk uut (
		.j(j), 
		.k(k), 
		.ce(clk), 
		.q(q),
		.clr(clr)
	);*/

	master_slave_ff uut (
		.j(j), 
		.k(k), 
		.ce(clk), 
		.q(q),
		.clr(clr)
	//	.q_bar(q_bar)
	);


endmodule

