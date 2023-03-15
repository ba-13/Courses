`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:54:25 01/18/2023 
// Design Name: 
// Module Name:    level_triggered_jk 
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
module level_triggered_jk(j,k,ce,clr,q);

input j,k,ce,clr;
output reg q;

always@(*) begin
    if(clr) begin
        q=0;
    end
    else if(ce==1 && j==1 && k==0) begin
    q=1;
    //q_bar=0;
    end 
    else if(ce==1 &&j==0 && k==1) begin
    q=0;
    //q_bar=1;
    end 
    else if(ce==1 && j==1 && k==1) begin
    q=~q;
    //q_bar=~q_bar;
    end 
    else if(ce==1 && j==0 && k==0) begin
    q=q;
    //q_bar=q_bar;
    end 
    else if(ce==0) begin
    q=q;
    //q_bar=q_bar;
    end 
    else begin
    q=q;
    //q_bar=q_bar;
    end
end
endmodule
