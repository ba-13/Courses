`timescale 1ns / 1ps

module moore_sequence_detector(
input in, clk, reset,
output reg out
);

parameter S0 = 0 , S1 = 1 , S2 = 2 , S3 = 3 , S4 = 4;
reg [2:0] PS, NS;

always @(posedge clk, posedge reset) begin
    if(reset)
        PS <= S0;   
    else    
        PS <= NS ; 
end

always @(PS, in) begin          // you need to update next state on the basis of new present state
    case(PS)
        S0 : begin
                    NS = in ? S1 : S0;
                    $display(PS);
                end
        S1 : begin 
                    NS = in ? S1 : S2;
                    $display(PS);
                end
        S2 : begin 
                    NS = in ? S3 : S0;
                    $display(PS);
                end 
        S3 : begin 
                    NS = in ? S1 : S4;
                    $display(PS);
                end
        S4 : begin 
                    NS = in ? S3 : S0;
                    $display(PS);
                end
        default: NS = S0; 
    endcase
end

always @(PS) begin
  case(PS)
    S4: out = 1;
    default: out = 0;
  endcase 
end 

endmodule