module one_bit_full_subtractor(
    D, Bout, 
    A, B, Bin
);

    input A, B, Bin;
    output D, Bout;
    wire t1, t2, t3, A_not;

    not(A_not, A);
    xor(D, A, B, Bin);      // D
    and(t1, B, Bin);
    and(t2, A_not, Bin);
    and(t3, A_not, B);
    or(Bout, t1, t2, t3);

endmodule