`timescale 1ps/1ps
`include "two_to_one_mux.v"

module two_to_one_mux_tb();

    parameter k = 16;
    reg [k-1:0] a, b;
    reg select;
    wire [k-1:0] out;

    two_to_one_mux mux(.out(out), .a(a), .b(b), .select(select));

    initial begin

        $dumpfile("two_to_one_mux_tb.vcd");
        $dumpvars(0, two_to_one_mux_tb);

        a <= 65;
        b <= 66;

        #10 select = 0;
        #10 select = 1;
        #10 select = 1;
        #10 select = 0;
        #10 select = 0;
        #10 select = 0;
        #10 select = 1;
        
    end

    initial begin

        $monitor("select = %b, a = %d, b = %d, out = %d", select, a, b, out);

    end


endmodule