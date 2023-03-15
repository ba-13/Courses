`timescale 1ps/1ps
`include "thirty_two_bits_full_adder.v"

module thirty_two_bits_full_adder_tb;

    reg [31:0] A, B;
    wire [31:0] S;
    wire Cout;
    thirty_two_bits_full_adder uut(.A(A), .B(B), .S(S), .Cout(Cout), .Cin(1'b0));

    initial begin

        $dumpfile("thirty_two_bits_full_adder_tb.vcd");
        $dumpvars(0, thirty_two_bits_full_adder_tb);

        // $display("Assuming Cin to be 0 and testing 20 random cases.");

        repeat(20) begin
            A = ($random) % (2**12);
            B = ($random) % (2**12); // !random numbers generated include (A =4294967293, B =4294967295, S =4294967292, Cout =1)
            #5;
        end

        $display("Edge case: A = 2**32 - 1, B = 1");
        A = 2**32 - 1;
        B = 1;

        $display("Test complete.");
        #100; $finish;

    end

    initial begin

        $monitor("A =%d, B =%d, S =%d, Cout =%b", A, B, S, Cout);
        
    end
    
endmodule