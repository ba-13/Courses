module invert (
    input [k-1:0] in,
    output [k-1:0] out
);

    parameter k = 16;
    assign out = ~in + 1;

endmodule

module testbench();

    parameter k = 16;
    reg [k-1:0] in;
    wire [k-1:0] out;
    wire [k:0] out_;

    invert #(k) inv(.in(in), .out(out));

    // initial begin
    //     $monitor("in=%b, out=%b", in, out);
    //     #20
    //     in = 246;
    //     #20
    //     in = 130;
    //     #20
    //     $finish;
    // end

    assign out_ = {in[k-1], 1'b1, in[k-2:0]};
    initial begin
        $monitor("in=%b, out=%b", in, out_);
        #5
        in = 2675;
        #10
        in = 130;
        #10
        $finish;
    end

endmodule