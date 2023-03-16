`include "left_shift_register.v"
`include "right_shift_register.v"

module shift_register_tb();

    parameter k = 8;
    reg [k-1:0] in;
    wire [k-1:0] out, out2;
    reg clk;

    left_shift_register #(k) lsr(.in(in), .out(out), .clk(clk));
    right_shift_register #(k) rsr(.in(in), .out(out2), .clk(clk));

    initial begin

        $dumpfile("vcd/shift_register_tb.vcd");
        $dumpvars(5, shift_register_tb);
        clk = 1'b0;
        $monitor("clk=%b, in=%b, out=%b, out2=%b", clk, in, out, out2);

    end

    always #5 clk <= ~clk;

    initial begin

        #10
        in <= $random;
        #10
        in <= out;
        #10
        in <= out;
        #10
        in <= out;
        #10
        in <= out;
        #10
        in <= out;
        #10
        in <= out;
        #10
        $finish;

    end


endmodule