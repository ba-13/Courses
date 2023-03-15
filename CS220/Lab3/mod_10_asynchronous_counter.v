`include "t_flipflop.v"

module mod_10_asynchronous_counter(
    output [3:0] q,
    input clk,
    input rst
);
    wire reset;
    reg RESET;
    always @(rst or reset) begin
        RESET <= rst | reset;
    end
    check_for_number check(.q(q), .reset(reset));
    t_flipflop ff0(.rst(RESET), .q(q[0]), .toggle(1'b1), .clk(clk));
    t_flipflop ff1(.rst(RESET), .q(q[1]), .toggle(1'b1), .clk(q[0]));
    t_flipflop ff2(.rst(RESET), .q(q[2]), .toggle(1'b1), .clk(q[1]));
    t_flipflop ff3(.rst(RESET), .q(q[3]), .toggle(1'b1), .clk(q[2]));

endmodule

module check_for_number(
    output reg reset,
    input [3:0] q
);

    always @(*) begin
        if (q == 4'b1010) reset <= 1'b1;
        else reset <= 1'b0;
    end

endmodule