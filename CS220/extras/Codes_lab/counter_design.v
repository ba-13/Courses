`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:27:11 01/06/2023 
// Design Name: 
// Module Name:    counter_design 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module counter_design(clk,out,reset);

parameter W=50000000;

input clk,reset;
output reg [3:0] out;
reg [31:0] int_count;


always@(posedge clk)
begin
if(reset)
out<=0;
else if(int_count==W)
out<=out+1;
else
out<=out;
end

always@(posedge clk)
begin
if(reset)
int_count<=0;
else if(int_count==W)
int_count<=0;
else
int_count<=int_count+1;
end

endmodule


