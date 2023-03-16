module right_shift_register(
    in, out, clk
);
    parameter k = 8;

    input [k-1:0] in;
    output reg [k-1:0] out;
    input clk;

    always @(posedge clk) begin
        out <= in >> 1;
    end

endmodule