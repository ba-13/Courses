module four_to_one_mux_32b (
    output [3:0] out,
    input [3:0] a,
    input [3:0] b,
    input [3:0] c,
    input [3:0] d,
    input [1:0] select
);

    assign out = select[1] ? (select[0] ? d : c) : (select[0] ? b : a);

endmodule