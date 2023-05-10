module mux2to1 (
    input select,
    input [k-1:0] A,
    input [k-1:0] B,
    output [k-1:0] Out
);

  parameter k = 32;

  assign Out = select ? B : A;

endmodule
