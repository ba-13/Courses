module two_to_one_mux (
    out,
    select, a, b
);
    parameter k = 16;

    output [k-1:0] out;
    input select;
    input [k-1:0] a, b;

    assign out = select ? b : a;

endmodule
