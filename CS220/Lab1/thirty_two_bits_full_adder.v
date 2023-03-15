`include "one_bit_full_adder.v"

module thirty_two_bits_full_adder(
    S, Cout,
    A, B, Cin
);

    parameter bits_ = 32;
    input [bits_-1:0] A, B;
    output [bits_-1:0] S;
    input Cin;
    output Cout;

    // intermediate wires
    wire [bits_-2:0] carry;

    one_bit_full_adder fa0 (.S(S[0]), .Cout(carry[0]), .A(A[0]), .B(B[0]), .Cin(Cin));
    one_bit_full_adder fa1 (.S(S[1]), .Cout(carry[1]), .A(A[1]), .B(B[1]), .Cin(carry[0]));
    one_bit_full_adder fa2 (.S(S[2]), .Cout(carry[2]), .A(A[2]), .B(B[2]), .Cin(carry[1]));
    one_bit_full_adder fa3 (.S(S[3]), .Cout(carry[3]), .A(A[3]), .B(B[3]), .Cin(carry[2]));
    one_bit_full_adder fa4 (.S(S[4]), .Cout(carry[4]), .A(A[4]), .B(B[4]), .Cin(carry[3]));
    one_bit_full_adder fa5 (.S(S[5]), .Cout(carry[5]), .A(A[5]), .B(B[5]), .Cin(carry[4]));
    one_bit_full_adder fa6 (.S(S[6]), .Cout(carry[6]), .A(A[6]), .B(B[6]), .Cin(carry[5]));
    one_bit_full_adder fa7 (.S(S[7]), .Cout(carry[7]), .A(A[7]), .B(B[7]), .Cin(carry[6]));
    one_bit_full_adder fa8 (.S(S[8]), .Cout(carry[8]), .A(A[8]), .B(B[8]), .Cin(carry[7]));
    one_bit_full_adder fa9 (.S(S[9]), .Cout(carry[9]), .A(A[9]), .B(B[9]), .Cin(carry[8]));
    one_bit_full_adder fa10 (.S(S[10]), .Cout(carry[10]), .A(A[10]), .B(B[10]), .Cin(carry[9]));
    one_bit_full_adder fa11 (.S(S[11]), .Cout(carry[11]), .A(A[11]), .B(B[11]), .Cin(carry[10]));
    one_bit_full_adder fa12 (.S(S[12]), .Cout(carry[12]), .A(A[12]), .B(B[12]), .Cin(carry[11]));
    one_bit_full_adder fa13 (.S(S[13]), .Cout(carry[13]), .A(A[13]), .B(B[13]), .Cin(carry[12]));
    one_bit_full_adder fa14 (.S(S[14]), .Cout(carry[14]), .A(A[14]), .B(B[14]), .Cin(carry[13]));
    one_bit_full_adder fa15 (.S(S[15]), .Cout(carry[15]), .A(A[15]), .B(B[15]), .Cin(carry[14]));
    one_bit_full_adder fa16 (.S(S[16]), .Cout(carry[16]), .A(A[16]), .B(B[16]), .Cin(carry[15]));
    one_bit_full_adder fa17 (.S(S[17]), .Cout(carry[17]), .A(A[17]), .B(B[17]), .Cin(carry[16]));
    one_bit_full_adder fa18 (.S(S[18]), .Cout(carry[18]), .A(A[18]), .B(B[18]), .Cin(carry[17]));
    one_bit_full_adder fa19 (.S(S[19]), .Cout(carry[19]), .A(A[19]), .B(B[19]), .Cin(carry[18]));
    one_bit_full_adder fa20 (.S(S[20]), .Cout(carry[20]), .A(A[20]), .B(B[20]), .Cin(carry[19]));
    one_bit_full_adder fa21 (.S(S[21]), .Cout(carry[21]), .A(A[21]), .B(B[21]), .Cin(carry[20]));
    one_bit_full_adder fa22 (.S(S[22]), .Cout(carry[22]), .A(A[22]), .B(B[22]), .Cin(carry[21]));
    one_bit_full_adder fa23 (.S(S[23]), .Cout(carry[23]), .A(A[23]), .B(B[23]), .Cin(carry[22]));
    one_bit_full_adder fa24 (.S(S[24]), .Cout(carry[24]), .A(A[24]), .B(B[24]), .Cin(carry[23]));
    one_bit_full_adder fa25 (.S(S[25]), .Cout(carry[25]), .A(A[25]), .B(B[25]), .Cin(carry[24]));
    one_bit_full_adder fa26 (.S(S[26]), .Cout(carry[26]), .A(A[26]), .B(B[26]), .Cin(carry[25]));
    one_bit_full_adder fa27 (.S(S[27]), .Cout(carry[27]), .A(A[27]), .B(B[27]), .Cin(carry[26]));
    one_bit_full_adder fa28 (.S(S[28]), .Cout(carry[28]), .A(A[28]), .B(B[28]), .Cin(carry[27]));
    one_bit_full_adder fa29 (.S(S[29]), .Cout(carry[29]), .A(A[29]), .B(B[29]), .Cin(carry[28]));
    one_bit_full_adder fa30 (.S(S[30]), .Cout(carry[30]), .A(A[30]), .B(B[30]), .Cin(carry[29]));
    one_bit_full_adder fa31 (.S(S[31]), .Cout(Cout), .A(A[31]), .B(B[31]), .Cin(carry[30]));

    // integer i;
    // one_bit_full_adder fa0(.S(S[0]), .Cout(carry[0]), .A(A[0]), .B(B[0]), .Cin(Cin));
    // for (i=1; i<31; i+=1) begin
    //     one_bit_full_adder fa(.S(S[i]), .Cout(carry[i]), .A(A[i]), .B(B[i]), .Cin(carry[i-1]));
    // end
    // one_bit_full_adder fa31(.S(S[31]), .Cout(Cout), .A(A[31]), .B(B[31]), .Cin(carry[30]));

endmodule

//? how to know the minimum clock period now, seeing such a cascade?
//? why are these running sequentially, should have been concurrent.