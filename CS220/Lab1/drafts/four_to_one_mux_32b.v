`include "four_to_one_mux.v"

module four_to_one_mux_32b (
    output [31:0] out,
    input [31:0] in [3:0],
    input [1:0] select
);
    
    // four_to_one_mux m0(.out(out[0]), .in(in[3:0][0]), .select(select));
    // four_to_one_mux m1(.out(out[1]), .in(in[3:0][1]), .select(select));
    // four_to_one_mux m2(.out(out[2]), .in(in[3:0][2]), .select(select));
    // four_to_one_mux m3(.out(out[3]), .in(in[3:0][3]), .select(select));
    // four_to_one_mux m4(.out(out[4]), .in(in[3:0][4]), .select(select));
    // four_to_one_mux m5(.out(out[5]), .in(in[3:0][5]), .select(select));
    // four_to_one_mux m6(.out(out[6]), .in(in[3:0][6]), .select(select));
    // four_to_one_mux m7(.out(out[7]), .in(in[3:0][7]), .select(select));
    // four_to_one_mux m8(.out(out[8]), .in(in[3:0][8]), .select(select));
    // four_to_one_mux m9(.out(out[9]), .in(in[3:0][9]), .select(select));
    // four_to_one_mux m10(.out(out[10]), .in(in[3:0][10]), .select(select));
    // four_to_one_mux m11(.out(out[11]), .in(in[3:0][11]), .select(select));
    genvar i;
    for (i=0; i<32; i=i+1) begin
        four_to_one_mux m0(.out(out[i]), .in({in[0][i], in[1][i], in[2][i], in[3][i]}), .select(select));
    end

endmodule