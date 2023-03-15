module one_bit_full_adder (
    Cout, S,
    Cin, A, B
);

    input A, B, Cin;
    output S, Cout;
    wire t1, t2, t3;

    xor(S, A, B, Cin);      // S
    and(t1, Cin, A);
    and(t2, A, B);
    and(t3, B, Cin);
    or(Cout, t1, t2, t3);  // Cout

endmodule