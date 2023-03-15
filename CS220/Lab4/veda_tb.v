`include "veda.v"
`timescale 1ns / 1ps

module veda_tb();

    reg clk;
    reg [4:0] addr;
    reg [31:0] datain;
    reg rst;
    reg write_enable;
    reg mode;
    wire [31:0] dataout;

    veda v(
        .clk(clk),
        .rst(rst),
        .write_enable(write_enable),
        .addr(addr),
        .datain(datain),
        .mode(mode),
        .dataout(dataout)
    );

    always #5 clk = ~ clk;

    initial begin
        write_enable = 1;
        mode = 0;
        addr = 4'ha;
        datain = 2000;
        #20
        mode = 0;
        addr = 4'hd;
        datain = 3000;
        #20
        mode = 1;
        addr = 4'hc;
        datain = 3000;
        #20
        mode = 1;
        addr = 4'ha;
        datain = 4000;
        #20
        mode = 0;
        addr = 4'hd;
        datain = 56;
        #20
        mode = 1;
        addr = 4'ha;
        datain = 5000;
        #20
        $finish;
    end

    initial begin
        $dumpfile("vcd/veda_tb.vcd");
        $dumpvars(3, veda_tb);
        clk = 1'b0;
    end

    initial begin

        $monitor("rst=%b mode=%b addr=%d datain=%d \t dataout=%d", rst, mode, addr, datain, dataout);
        rst = 1'b1;
        #5 rst = 1'b0;
        
    end

endmodule