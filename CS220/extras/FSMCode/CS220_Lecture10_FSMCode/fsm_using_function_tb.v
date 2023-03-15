`include "fsm_using_function.v"

module test_fsm;
// Inputs
reg clock,reset,req_0,req_1;
wire [2:0] state;
// Outputs
wire gnt_0,gnt_1;

// Instantiate the Unit Under Test (UUT)
fsm_using_function m1(clock,reset,req_0,req_1,gnt_0,gnt_1);

always #1 clock=~clock;
initial
    begin
    $dumpfile("test_fsm_function.vcd");
    $dumpvars(0,test_fsm);
       clock=1;
        #10;
        reset=1'b1;
      //  state=3'b001;//idle
        req_0=1'b0;
        req_1=1'b0;
        #10;
        reset=1'b0;
      //  state=3'b001;
        req_0=1'b1;
        req_1=1'b0;
        #10;
        reset=1'b0;
      //  state=3'b010;
        req_0=1'b0;
        req_1=1'b0;
        #10;
        reset=1'b0;
      //  state=3'b001;
        req_0=1'b0;
        req_1=1'b1;
        #10;
        reset=1'b0;
      //  state=3'b100;
        req_0=1'b0;
        req_1=1'b0;
        #10;
        reset=1'b0;
    //    state=3'b001;
        req_0=1'b1;
        req_1=1'b1;
        #10;

        $finish;
    end
endmodule
