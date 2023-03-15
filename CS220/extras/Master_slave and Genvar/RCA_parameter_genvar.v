`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 20.01.2022 05:39:16
// Design Name: 
// Module Name: RCA_parameter_genvar
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module RCA_parameter_genvar(a,b,sum,cout);

parameter W=32;
input [W-1:0] a,b;
output [W-1:0] sum;
output cout;

wire [W:0] c_temp;

assign c_temp[0]=1'd0;

genvar i;
generate

    for(i=0;i<32;i=i+1)
    begin: loop1
        full_adder_structural add0(a[i],b[i],c_temp[i],sum[i],c_temp[i+1]);
    end

endgenerate

assign cout=c_temp[W];


endmodule
