module TFF (CLK,T,OUT);
  input CLK,T;
  output reg OUT=0;
  always @ ( posedge CLK ) begin
    if(T) OUT<=~OUT;
    else  OUT<=OUT;
  end
endmodule // TFF

module NINE_YET (IN,OUT);
  input  [3:0] IN;
  output reg [1:0]OUT;
  always @ ( * ) begin
    if(IN==9) OUT<=2'b01; //Q[3] should be toggled the next cycle, Q[2] should not
    else OUT<={~IN[2],~IN[0]}; //just pass Q` of Tff[2] and Tff[0]
  end
endmodule // NINE_YET

module counter (CLK,OUT);
  input CLK;
  output [3:0] OUT;
  wire [1:0]nine_yet_op;
  NINE_YET m(OUT,nine_yet_op);

  TFF Q0(CLK,1'b1,OUT[0]);
  TFF Q1(nine_yet_op[0],1'b1,OUT[1]);
  TFF Q2(~OUT[1],1'b1,OUT[2]);
  TFF Q3(nine_yet_op[1],1'b1,OUT[3]);
endmodule // counter