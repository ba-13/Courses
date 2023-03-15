module four_to_one_mux (
    output out,
    input [3:0] in,
    input [1:0] select
);

    wire [3:0] tmp;
    wire [1:0] select_not;
    not n [1:0] (select_not, select);

    and(tmp[3], in[3], select[1], select[0]);
    and(tmp[2], in[2], select[1], select_not[0]);
    and(tmp[1], in[1], select_not[1], select[0]);
    and(tmp[0], in[0], select_not[1], select_not[0]);

    or(out, tmp[3], tmp[2], tmp[1], tmp[0]);

endmodule