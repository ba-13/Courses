`timescale 1ns / 1ps



module full_adder_structural(a,b,cin,s,cout);

input a,b,cin;
output s,cout;

wire s_temp,c_temp,c_temp_2;

/// half adder 1
xor(s_temp,a,b); 
and(c_temp,a,b);
/// half adder 2
xor(s,s_temp,cin); 
and(c_temp_2,s_temp,cin);
/// full adder
or(cout,c_temp,c_temp_2);

endmodule
