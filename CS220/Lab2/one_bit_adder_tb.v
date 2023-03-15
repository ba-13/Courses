`timescale 1ps/1ps
`include "one_bit_adder.v"

module one_bit_adder_tb;

    reg A, B, Cin;
    wire Cout, S;
    one_bit_adder uut(.Cout(Cout), .S(S), .A(A), .B(B), .Cin(Cin));

    initial begin

        $dumpfile("one_bit_adder_tb.vcd");
        $dumpvars(0, one_bit_adder_tb);

        A <= 0;
        B <= 0;
        Cin <= 0;
        #10;

        A <= 0;
        B <= 1;
        Cin <= 0;
        #10;

        A <= 1;
        B <= 0;
        Cin <= 0;
        #10;

        A <= 1;
        B <= 1;
        Cin <= 0;
        #10;

        A <= 0;
        B <= 0;
        Cin <= 1;
        #10;

        A <= 0;
        B <= 1;
        Cin <= 1;
        #10;

        A <= 1;
        B <= 0;
        Cin <= 1;
        #10;

        A <= 1;
        B <= 1;
        Cin <= 1;
        #10;

        $display("Test complete");
        
    end

    initial begin
        
        $monitor("A = %b, B = %b, Cin = %b, Cout = %b, S = %b", A, B, Cin, Cout, S);

    end

endmodule