module mix_assignments();

reg A, B, C;
reg clk;

always @(posedge clk) begin
    C = A ^ ~B;
    A <= C;
    B = ~A;
end

initial begin
    $dumpfile("vcd/mix_assignments");
    $dumpvars(0, mix_assignments);
    #50
    $finish;
end

initial begin
    A <= 0;
    B <= 0;
    C <= 0;
end

initial begin
    $monitor("%b %b %b", A, B, C);
    clk = 1'b0;
end

always #5 clk = ~ clk;

endmodule