`include "one_bit_full_subtractor.v"

module thirty_two_bits_full_subtractor(
    D, Bout,
    A, B, Bin
);
    input [31:0] A, B;
    input Bin;
    output [31:0] D;
    output Bout;

    wire [30:0] borrow;

    one_bit_full_subtractor fs0(.D(D[0]), .Bout(borrow[0]), .A(A[0]), .B(B[0]), .Bin(Bin));
    one_bit_full_subtractor fs1(.D(D[1]), .Bout(borrow[1]), .A(A[1]), .B(B[1]), .Bin(borrow[0]));
    one_bit_full_subtractor fs2(.D(D[2]), .Bout(borrow[2]), .A(A[2]), .B(B[2]), .Bin(borrow[1]));
    one_bit_full_subtractor fs3(.D(D[3]), .Bout(borrow[3]), .A(A[3]), .B(B[3]), .Bin(borrow[2]));
    one_bit_full_subtractor fs4(.D(D[4]), .Bout(borrow[4]), .A(A[4]), .B(B[4]), .Bin(borrow[3]));
    one_bit_full_subtractor fs5(.D(D[5]), .Bout(borrow[5]), .A(A[5]), .B(B[5]), .Bin(borrow[4]));
    one_bit_full_subtractor fs6(.D(D[6]), .Bout(borrow[6]), .A(A[6]), .B(B[6]), .Bin(borrow[5]));
    one_bit_full_subtractor fs7(.D(D[7]), .Bout(borrow[7]), .A(A[7]), .B(B[7]), .Bin(borrow[6]));
    one_bit_full_subtractor fs8(.D(D[8]), .Bout(borrow[8]), .A(A[8]), .B(B[8]), .Bin(borrow[7]));
    one_bit_full_subtractor fs9(.D(D[9]), .Bout(borrow[9]), .A(A[9]), .B(B[9]), .Bin(borrow[8]));
    one_bit_full_subtractor fs10(.D(D[10]), .Bout(borrow[10]), .A(A[10]), .B(B[10]), .Bin(borrow[9]));
    one_bit_full_subtractor fs11(.D(D[11]), .Bout(borrow[11]), .A(A[11]), .B(B[11]), .Bin(borrow[10]));
    one_bit_full_subtractor fs12(.D(D[12]), .Bout(borrow[12]), .A(A[12]), .B(B[12]), .Bin(borrow[11]));
    one_bit_full_subtractor fs13(.D(D[13]), .Bout(borrow[13]), .A(A[13]), .B(B[13]), .Bin(borrow[12]));
    one_bit_full_subtractor fs14(.D(D[14]), .Bout(borrow[14]), .A(A[14]), .B(B[14]), .Bin(borrow[13]));
    one_bit_full_subtractor fs15(.D(D[15]), .Bout(borrow[15]), .A(A[15]), .B(B[15]), .Bin(borrow[14]));
    one_bit_full_subtractor fs16(.D(D[16]), .Bout(borrow[16]), .A(A[16]), .B(B[16]), .Bin(borrow[15]));
    one_bit_full_subtractor fs17(.D(D[17]), .Bout(borrow[17]), .A(A[17]), .B(B[17]), .Bin(borrow[16]));
    one_bit_full_subtractor fs18(.D(D[18]), .Bout(borrow[18]), .A(A[18]), .B(B[18]), .Bin(borrow[17]));
    one_bit_full_subtractor fs19(.D(D[19]), .Bout(borrow[19]), .A(A[19]), .B(B[19]), .Bin(borrow[18]));
    one_bit_full_subtractor fs20(.D(D[20]), .Bout(borrow[20]), .A(A[20]), .B(B[20]), .Bin(borrow[19]));
    one_bit_full_subtractor fs21(.D(D[21]), .Bout(borrow[21]), .A(A[21]), .B(B[21]), .Bin(borrow[20]));
    one_bit_full_subtractor fs22(.D(D[22]), .Bout(borrow[22]), .A(A[22]), .B(B[22]), .Bin(borrow[21]));
    one_bit_full_subtractor fs23(.D(D[23]), .Bout(borrow[23]), .A(A[23]), .B(B[23]), .Bin(borrow[22]));
    one_bit_full_subtractor fs24(.D(D[24]), .Bout(borrow[24]), .A(A[24]), .B(B[24]), .Bin(borrow[23]));
    one_bit_full_subtractor fs25(.D(D[25]), .Bout(borrow[25]), .A(A[25]), .B(B[25]), .Bin(borrow[24]));
    one_bit_full_subtractor fs26(.D(D[26]), .Bout(borrow[26]), .A(A[26]), .B(B[26]), .Bin(borrow[25]));
    one_bit_full_subtractor fs27(.D(D[27]), .Bout(borrow[27]), .A(A[27]), .B(B[27]), .Bin(borrow[26]));
    one_bit_full_subtractor fs28(.D(D[28]), .Bout(borrow[28]), .A(A[28]), .B(B[28]), .Bin(borrow[27]));
    one_bit_full_subtractor fs29(.D(D[29]), .Bout(borrow[29]), .A(A[29]), .B(B[29]), .Bin(borrow[28]));
    one_bit_full_subtractor fs30(.D(D[30]), .Bout(borrow[30]), .A(A[30]), .B(B[30]), .Bin(borrow[29]));
    one_bit_full_subtractor fs31(.D(D[31]), .Bout(Bout), .A(A[31]), .B(B[31]), .Bin(borrow[30]));

endmodule