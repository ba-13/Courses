`include "k_bit_ripple_adder.v"
`include "two_to_one_mux.v"

module k_bit_mjolnir_adder (
    output Cout,
    output [k-1:0] S,
    input [k-1:0] A,
    input [k-1:0] B,
    input Cin
);

    parameter k = 16;

    wire Clower, Cupper0, Cupper1;
    wire [k/2-1:0] S0, S1;

    k_bit_ripple_adder #(k/2) lower (
        .Cout(Clower), .S(S[k/2-1:0]), 
        .A(A[k/2-1:0]), .B(B[k/2-1:0]), .Cin(Cin)
    );

    // parallelly compute upper half with 0 and 1 carrys
    k_bit_ripple_adder #(k/2) upper0 (
        .Cout(Cupper0), .S(S0),
        .A(A[k-1:k/2]), .B(B[k-1:k/2]), .Cin(1'b0)
    );  
    k_bit_ripple_adder #(k/2) upper1 (
        .Cout(Cupper1), .S(S1), 
        .A(A[k-1:k/2]), .B(B[k-1:k/2]), .Cin(1'b1)
    );  

    two_to_one_mux #(k/2) muxSum (
        .out(S[k-1:k/2]), 
        .a(S0), .b(S1), .select(Clower)
    );
    two_to_one_mux #(1) muxCarry (
        .out(Cout), 
        .a(Cupper0), .b(Cupper1), .select(Clower)
    );

endmodule
