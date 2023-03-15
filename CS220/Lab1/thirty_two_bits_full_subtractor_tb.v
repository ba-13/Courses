`timescale 1ps/1ps
`include "thirty_two_bits_full_subtractor.v"

module thirty_two_bits_full_subtractor_tb;

    reg [31:0] A, B;
    wire [31:0] D;
    wire Bout;
    thirty_two_bits_full_subtractor uut(.A(A), .B(B), .D(D), .Bout(Bout), .Bin(1'b0));

    initial begin

        $dumpfile("thirty_two_bits_full_subtractor_tb.vcd");
        $dumpvars(0, thirty_two_bits_full_subtractor_tb);

        repeat(20) begin
            A = ($random) % 2**10;
            B = ($random) % 2**10; // random numbers generated like (A =4294967293, B =4294967295, S =4294967292, Cout =1)
            #5;
        end

        $display("Edge case: A = 2**32 - 1, B = 1");
        A = 45;
        B = 50;

        $display("Test complete.");
        #100; $finish;

    end

    initial begin

        $monitor("A =%d, B =%d, D =%d, Bout =%b", A, B, D, Bout);
        
    end
    
endmodule