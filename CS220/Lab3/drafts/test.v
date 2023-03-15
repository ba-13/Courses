`include "counter.v"
`timescale 1ms/1ps

module counter_tb();

    wire [3:0] q;
    reg clk;
    integer i;
    counter counter(.OUT(q), .CLK(clk));

    initial begin
        $dumpfile("vcd/counter_tb.vcd");
        $dumpvars(5, counter_tb);
        #85
        #15;
        #40
        $finish;
	end

    initial begin
        $monitor("%b", q);
    end

	initial begin
		clk = 0;
		for(i = 0; i < 140; i = i + 1)
			#2 clk = ~clk;
	end

endmodule