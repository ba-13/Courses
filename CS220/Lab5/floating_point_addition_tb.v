`include "floating_point_addition.v"

module floating_point_addition_tb();

    parameter k = 32;
    reg [k-1:0] A;
    reg [k-1:0] B;
    reg clk, reset;
    wire [k-1:0] C;
    wire corrupt, done;

    always #5 clk <= ~clk;
    floating_point_addition #(k) uut(.a(A), .b(B), .clk(clk), .C(C), .corrupt(corrupt), .reset(reset), .done(done));

    initial begin
        $dumpfile("vcd/floating_point_addition_tb.vcd");
        $dumpvars(5, floating_point_addition_tb);
        clk <= 1'b0;
        reset <= 1'b0;
    end

    initial begin
        #20
        reset <= 1;
        A <= 32'b00111110111000000000000000000000;  // 0.4375
        B <= 32'b00111111000000000000000000000000;  // 0.5
        #10
        reset <= 0;
        #200
        $display("Final C: %b", C);
        
        #20
        reset <= 1;
        A <= 32'b11000000001000000000000000000000;  // -2.5
        B <= 32'b11000000011000000000000000000000;  // -3.5
        #10
        reset <= 0;
        #200
        $display("Final C: %b", C);

        #20
        reset <= 1;
        A <= 32'b01000011001010100100000000000000;  // 170.25
        B <= 32'b11000011011001011010000000000000;  // -229.625
        #10
        reset <= 0;
        #200
        $display("Final C: %b", C);
        $finish;
    end

endmodule

/*
works for positives
0 10000111 10001111111000000000000
0 10000111 10001111111000000000000

works for A=pos, B=neg
0 10000100 11011011000000000000000
0 10000100 11011011000000000000000
works for A=neg, B=pos
1 10000100 11011011000000000000000

-399.875
works for negatives
1 10000111 10001111111000000000000
1 10000111 10001111111000000000000
*/