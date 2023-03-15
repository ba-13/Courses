`include "fsm_using_single_always.v"

module fsm_using_single_always_tb;
// Inputs
reg clock,reset,req_0,req_1;
wire [2:0] state;
// Outputs
wire gnt_0,gnt_1;

// Instantiate the Unit Under Test (UUT)
fsm_using_single_always m1(clock,reset,req_0,req_1,gnt_0,gnt_1);

always #1 clock=~clock;
initial
    begin
    $dumpfile("fsm_using_single_always.vcd");
    $dumpvars(0,fsm_using_single_always_tb);
        clock=1;
        #10;
        reset=1'b1;
        //state=3'b001;//idle
        req_0=1'b0;
        req_1=1'b0;
        #10;
        reset=1'b0;
        //state=3'b001;
        req_0=1'b1;
        req_1=1'b0;
        #10;
        reset=1'b0;
        //state=3'b010;
        req_0=1'b0;
        req_1=1'b0;
        #10;
        reset=1'b0;
        //state=3'b001;
        req_0=1'b0;
        req_1=1'b1;
        #10;
        reset=1'b0;
        //state=3'b100;
        req_0=1'b0;
        req_1=1'b0;
        #10;
        reset=1'b0;
        //state=3'b001;
        req_0=1'b1;
        req_1=1'b1;
        #10;

        $finish;
    end
endmodule
