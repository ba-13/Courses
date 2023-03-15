module delay_one_cycle(
    input clk,
    input[31:0] original_signal,
    output reg[31:0] delayed_signal
);

    always @(posedge clk) begin
        delayed_signal <= original_signal;
    end

endmodule