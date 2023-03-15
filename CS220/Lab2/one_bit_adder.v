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