`include "two_to_four_decoder.v"

module three_to_eight_decoder (
    output [7:0] out,
    input [2:0] in,
    input enable
);

    wire enable1;
    and(enable1, in[2], enable);
    two_to_four_decoder t1(.out(out[7:4]), .in(in[1:0]), .enable(enable1));

    wire enable0, in2_not;
    not(in2_not, in[2]);
    and(enable0, in2_not, enable);
    two_to_four_decoder t0(.out(out[3:0]), .in(in[1:0]), .enable(enable0));

endmodule