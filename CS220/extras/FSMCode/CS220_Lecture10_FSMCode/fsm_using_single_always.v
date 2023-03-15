module fsm_using_single_always (
clock      , // clock
reset      , // Active high, syn reset
req_0      , // Request 0
req_1      , // Request 1
gnt_0      , // Grant 0
gnt_1
);

input   clock,reset,req_0,req_1;
output  gnt_0,gnt_1;
wire    clock,reset,req_0,req_1;
reg     gnt_0,gnt_1;
parameter SIZE = 3           ;
parameter IDLE  = 3'b001,GNT0 = 3'b010,GNT1 = 3'b100 ;
//=============Internal Variables======================
reg   [SIZE-1:0]          state        ;// Seq part of the FSM
reg   [SIZE-1:0]          next_state   ;// combo part of FSM
//==========Code startes Here==========================
always @ (posedge clock)
begin : FSM
if (reset == 1'b1) begin
  state <= #1 IDLE;
  gnt_0 <= 0;
  gnt_1 <= 0;
end else
 case(state)
   IDLE : begin if (req_0 == 1'b1) begin
                state <=  GNT0;
                //gnt_0 <= 1;
              end else if (req_1 == 1'b1) begin
                //gnt_1 <= 1;
                state <=  GNT1;
              end else begin
                state <=  IDLE;
              end
            gnt_0<=0;
            gnt_1<=0; end
   GNT0 : begin if (req_0 == 1'b1) begin
                state <=  GNT0;
                end
          else if(req_1==1'b1) begin
                state<= GNT1;
              end else begin

                state <= IDLE;
              end
              gnt_0 <= 1;
              gnt_1 <= 0;  end
   GNT1 : begin if (req_1 == 1'b1) begin
                state <=  GNT1;
                end
          else if(req_1==1'b1) begin
                state<= GNT0;
              end else begin
                state <=  IDLE;
              end
              gnt_0 <= 0;
              gnt_1 <= 1; end
   default : begin state <=  IDLE;
   gnt_0 <= 0;
   gnt_1 <= 0;
   end
endcase
end

endmodule
