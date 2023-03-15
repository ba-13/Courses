`timescale 1ps/1ps
`include "k_bit_mjolnir_adder.v"

module k_bit_mjolnir_adder_tb ();

    parameter k = 128; // top level parameter

    reg [k-1:0] A, B;
    reg Cin;
    
    wire [k-1:0] S;
    wire Cout;

    k_bit_mjolnir_adder #(k) adder(.S(S), .Cout(Cout), .A(A), .B(B), .Cin(Cin));
    
    initial begin
        $dumpfile("k_bit_mjolnir_adder_tb.vcd");
        $dumpvars(0, k_bit_mjolnir_adder_tb);
    end

    initial begin
        // $monitor ("A = %b (%d), B = %b (%d), Cin = %b (%d); S = %b (%d), Cout = %b (%d)", A, A, B, B, Cin, Cin, S, S, Cout, Cout);
        $monitor ("A = %d, B = %d, Cin = %d; S = %d, Cout = %d", A, B, Cin, S, Cout);
    end

    initial begin
        A <= $random;
        B <= $random;
        Cin <= 0;

        #10
        A <= $random;
        B <= $random;
        Cin <= 0;

        #10
        A <= $random;
        B <= $random;
        Cin <= 1;

        #10
        A <= 2**8-1;
        B <= 1;
        Cin <= 0;
    end

endmodule