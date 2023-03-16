module left_shift_register(
    in, out, clk
);
    parameter k = 8;

    input [k-1:0] in;
    output reg [k-1:0] out;
    input clk;

    always @(posedge clk) begin
        // out[k-1:k-5] <= in[k-1:k-5] >> 1;
        // out[k-6:0] <= in[k-6:0] << 1;
        out <= in << 1;
    end

endmodule