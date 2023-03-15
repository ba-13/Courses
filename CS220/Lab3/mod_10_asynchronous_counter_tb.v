`include "mod_10_asynchronous_counter.v"
`timescale 1ms/1ps

module mod_10_asynchronous_counter_tb();

    wire [3:0] q;
    reg rst;
    reg clk;
    integer i;
    mod_10_asynchronous_counter counter(.q(q), .rst(rst), .clk(clk));

    initial begin
        $dumpfile("vcd/mod_10_asynchronous_counter_tb.vcd");
        $dumpvars(5, mod_10_asynchronous_counter_tb);
        #85
        rst = 0;
        #15;
        rst = 0;
        #40
        $finish;
	end

    initial begin
        $monitor("%b \t %b", rst, q);
    end

	initial begin
		clk = 0;
		for(i = 0; i < 140; i = i + 1)
			#2 clk = ~clk;
	end

endmodule