`timescale 1ps/1ps
`include "four_to_one_mux_32b.v"

module four_to_one_mux_32b_tb;

    reg [3:0] a, b, c, d;
    reg [1:0] select;
    wire [3:0] out;

    four_to_one_mux_32b mux(.out(out), .a(a), .b(b), .c(c), .d(d), .select(select));

    initial begin

        $dumpfile("four_to_one_mux_32b_tb.vcd");
        $dumpvars(0, four_to_one_mux_32b_tb);

        a <= ($random) % 256;
        b <= ($random) % 256;
        c <= ($random) % 256;
        d <= ($random) % 256;

        #10 select = 0;
        #10 select = 1;
        #10 select = 2;
        #10 select = 3;

        #10 $finish;

    end

    initial begin

        $monitor("select = %b, a = %d, b = %d, c = %d, d = %d, out = %d", select, a, b, c, d, out);

    end

endmodule