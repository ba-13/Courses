`include "adder.v"

module adder_tb();

    reg clk, reset;
    wire flag;

    adder a(.clk(clk), .reset(reset), .flag(flag));

    always #5 clk <= ~clk;

    initial begin
        $dumpfile("vcd/adder_tb.vcd");
        $dumpvars(5, adder_tb);
        clk <= 1'b0;
        reset <= 1'b0;
    end

    initial begin
        reset <= 1;
        #10
        reset <= 0;
        #500
        $finish;
    end

    initial begin
        $monitor("reset=%b, flag=%b", reset, flag);
    end

endmodule