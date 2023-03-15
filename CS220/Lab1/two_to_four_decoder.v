module two_to_four_decoder (
    output [3:0] out,
    input [1:0] in,
    input enable
);
    
    wire [1:0] in_not;
    not n [1:0] (in_not, in);

    and(out[3], in[1], in[0], enable);
    and(out[2], in[1], in_not[0], enable);
    and(out[1], in_not[1], in[0], enable);
    and(out[0], in_not[1], in_not[0], enable);

endmodule