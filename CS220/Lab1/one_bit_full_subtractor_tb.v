`timescale 1ps/1ps
`include "one_bit_full_subtractor.v"

module one_bit_full_subtractor_tb;
    
    reg A, B, Bin; // Bin is borrow coming in from lower bits
    wire Bout, D;
    full_subtractor uut(.A(A), .B(B), .Bin(Bin), .Bout(Bout), .D(D));

    initial begin

        $dumpfile("full_subtractor_tb.vcd");
        $dumpvars(0, full_subtractor_tb);

        A <= 0;
        B <= 0;
        Bin <= 0;
        #10;

        A <= 0;
        B <= 1;
        Bin <= 0;
        #10;

        A <= 1;
        B <= 0;
        Bin <= 0;
        #10;

        A <= 1;
        B <= 1;
        Bin <= 0;
        #10;

        A <= 0;
        B <= 0;
        Bin <= 1;
        #10;

        A <= 0;
        B <= 1;
        Bin <= 1;
        #10;

        A <= 1;
        B <= 0;
        Bin <= 1;
        #10;

        A <= 1;
        B <= 1;
        Bin <= 1;
        #10;

        $display("Test complete");
        #10; $finish;
        
    end

    initial begin

        $monitor("A=%b, B=%b, Bin=%b, Bout=%b, D=%b", A, B, Bin, Bout, D); // comment out to reduce output

    end

endmodule