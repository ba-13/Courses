`include "one_bit_adder.v"

module k_bit_ripple_adder (
    output Cout,
    output [k-1:0] S,
    input [k-1:0] A,
    input [k-1:0] B,
    input Cin
);
    parameter k = 8; // not used, just a default 8-bit parameter

    wire [k-1:0] carry;
    one_bit_adder adder0(.Cout(carry[0]), .S(S[0]), .A(A[0]), .B(B[0]), .Cin(Cin));

    genvar i;
    generate

    for (i = 1; i < k; i = i + 1) begin
        one_bit_adder adder(.Cout(carry[i]), .S(S[i]), .A(A[i]), .B(B[i]), .Cin(carry[i-1]));
    end

    endgenerate

    assign Cout = carry[k-1];

endmodule