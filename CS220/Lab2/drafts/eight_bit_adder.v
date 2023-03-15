module one_bit_adder (
    output Cout, 
    output S,
    input A, 
    input B, 
    input Cin
);
    assign S = (A ^ B ^ Cin);
    assign Cout = (A|B) & (B|Cin) & (Cin|A);
endmodule

module eight_bit_adder (
  output Cout, 
  output [7:0] S,
  input [7:0] A, 
  input [7:0] B, 
  input Cin
);

  wire [8:0] carry;

  one_bit_adder adder (
    .Cout(carry[0]), 
    .S(S[0]), 
    .A(A[0]), 
    .B(B[0]), 
    .Cin(Cin)
  );

  genvar i;
  generate

  for (i=1; i<8; i=i+1) begin: ADD_LOOP
    one_bit_adder adder (
      .Cout(carry[i]), 
      .S(S[i]), 
      .A(A[i]), 
      .B(B[i]), 
      .Cin(carry[i-1])
    );
  end

  endgenerate

  assign Cout = carry[7];

endmodule
