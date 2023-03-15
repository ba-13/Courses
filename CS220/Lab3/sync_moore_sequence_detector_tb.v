`include "sync_moore_sequence_detector.v"

module sync_moore_sequence_detector_tb();

    reg ip, rst, clk;
    wire op;
    integer i;
    sync_moore_sequence_detector sd(.ip(ip), .op(op), .rst(rst), .clk(clk));

    initial begin
        $dumpfile("vcd/sync_moore_sequence_detector_tb.vcd");
        $dumpvars(5, sync_moore_sequence_detector_tb);
        #10 ip = 0;
        #10 ip = 0;
        #10 ip = 1;
        #10 ip = 0;
        #10 ip = 1;
        #10 ip = 0;
        #10 ip = 0;
        #10 ip = 1;
        #10 ip = 0;
        #10 ip = 1;
        #10 ip = 0;
        #10 ip = 0;
        #10 ip = 0;
        #10 ip = 0;
        $finish;
	end

    initial begin
        $monitor("%b %b \t %b", rst, ip, op);
    end

    initial begin
		clk = 0;
        #10
		for(i = 0; i < 140; i = i + 1)
			#5 clk = ~clk;
	end

endmodule