`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:51:32 01/18/2023 
// Design Name: 
// Module Name:    master_slave_ff 
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
module master_slave_ff(ce,clr,j,k,q);

input ce,clr, j, k;
output q;
wire q_temp;
wire qm_out;

// master connection

level_triggered_jk ffm(j & (~q_temp), k & q_temp, ce, clr, qm_out);

level_triggered_jk ffs(qm_out, ~qm_out, ~ce, clr, q_temp);

assign q= q_temp;



endmodule
