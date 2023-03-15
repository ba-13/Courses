`include "delay_one_cycle.v"

module mod_veda(
    input clk,
    input rst,
    input write_enable,
    input [4:0] addr_a,
    input [4:0] addr_b,
    input [31:0] datain,
    input mode,
    output [31:0] dataout
);

    integer i;
    reg [31:0] cells[31:0]; // first signifies data bits, second is number of such cells

    reg [31:0] out;
    delay_one_cycle delay(.clk(clk), .original_signal(out), .delayed_signal(dataout));

    initial begin

        out <= 0;
        for (i = 0; i < 32; i = i + 1)
            cells[i] <= 0;

    end

    always @(posedge clk) begin
        if (rst == 1) begin

            for (i = 0; i < 32; i = i + 1) begin : reset
                cells[i] <= 0;
            end
            
        end
        if (write_enable) begin

            cells[addr_a] = datain;
            out = cells[addr_b];

        end else if (mode == 1) begin

            out = cells[addr_b];
            
        end
    end

endmodule