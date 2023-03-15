`timescale 1ps/1ps
`include "three_to_eight_decoder.v"

module three_to_eight_decoder_tb;
    
    reg [2:0] in;
    wire E = 1;             // Enable is put to 1 by default
    wire [7:0] out;

    three_to_eight_decoder decoder(.out(out), .enable(E), .in(in));

    initial begin
        $dumpfile("three_to_eight_decoder_tb.vcd");
        $dumpvars(0, three_to_eight_decoder_tb);

        #5 // checking for don't care state
        in = 0;
        repeat(7) begin
            #5 in++;
        end

        #20 $finish;
    end

    initial begin
        $monitor("Input=%b (In binary), %d (In decimal); Output=%b (In binary)", in, in, out);
        // Prints the values of Input and Output for each case
    end

endmodule