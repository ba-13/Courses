`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:25:38 01/06/2023 
// Design Name: 
// Module Name:    d_flipflop 
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
module d_flipflop(clk, reset,d,q);
input clk, reset,d;
output q;
reg q;
wire clk, reset, d;

always @(posedge clk or posedge reset)
    if (reset) begin 
        q<=1'b0;
        end
        else begin 
           q<=d;
    end        
endmodule

