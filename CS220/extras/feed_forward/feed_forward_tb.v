`timescale 1ns/1ns
`include "feed_forward.v"

module feed_forward_tb;

    reg A;
    wire B;
    feed_forward uut(A, B);

    initial begin
        
        $dumpfile("feed_forward.vcd");
        $dumpvars(0, feed_forward_tb);

        A = 1;
        #20;
        A = 0;
        #20;
        A = 1;
        #20;
        A = 1;
        #20;
        A = 0;
        #20;
        A = 1;
        #20;
        A = 1;
        #20;

        $display("Test complete");
        
    end
    
endmodule
