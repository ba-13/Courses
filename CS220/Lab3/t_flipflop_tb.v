`include "t_flipflop.v"
`timescale 1ns/1ps

module t_flipflop_tb();

    reg toggle, clk, rst;
	wire q,q_bar;
	integer i;

	t_flipflop ff(.toggle(toggle), .clk(clk), .q(q), .q_bar(q_bar), .rst(rst));

	initial begin
        $monitor("%b %b %b \t %b %b", rst, toggle, clk, q, q_bar);
        $dumpfile("vcd/t_flipflop_tb.vcd");
        $dumpvars(0, t_flipflop_tb);
            rst = 0;
            toggle = 0;
            #10
            toggle = 1;
            #10
            toggle = 0;
            rst = 1;
            #10
            toggle = 1;
            rst = 0;
            #5
        $finish;
	end

	initial begin
		clk = 0;
		for(i = 0; i < 20; i = i + 1)
			#2 clk = ~clk;
	end

endmodule