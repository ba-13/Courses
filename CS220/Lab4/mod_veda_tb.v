`include "mod_veda.v"
`timescale 1ns / 1ps

module mod_veda_tb();

    reg clk;
    reg [4:0] addr_a;
    reg [4:0] addr_b;
    reg [31:0] datain;
    reg rst;
    reg write_enable;
    reg mode;
    wire [31:0] dataout;

    mod_veda v(
        .clk(clk),
        .rst(rst),
        .write_enable(write_enable),
        .addr_a(addr_a),
        .addr_b(addr_b),
        .datain(datain),
        .mode(mode),
        .dataout(dataout)
    );

    always #5 clk = ~ clk;

    initial begin
        write_enable = 1;
        mode = 0;
        addr_a = 4'hc;
        addr_b = 4'ha;
        datain = 55;
        #20
        mode = 0;
        addr_a = 4'ha;
        addr_b = 4'hd;
        datain = 56;
        #20
        mode = 1;
        addr_a = 4'h2;
        addr_b = 4'hc;
        #20
        mode = 1;
        addr_a = 4'hc;
        addr_b = 4'ha;
        datain = 55;
        #20
        mode = 0;
        addr_a = 4'ha;
        addr_b = 4'hd;
        datain = 56;
        #20
        mode = 1;
        addr_a = 4'h2;
        addr_b = 4'ha;
        #20
        $finish;
    end

    initial begin
        $dumpfile("vcd/mod_veda_tb.vcd");
        $dumpvars(5, mod_veda_tb);
        clk = 1'b0;
    end

    initial begin

        $monitor("rst=%b mode=%b addr_a=%d, addr_b=%d datain=%d \t dataout=%d", rst, mode, addr_a, addr_b, datain, dataout);
        rst = 1'b1;
        #5 rst = 1'b0;
        
    end

endmodule