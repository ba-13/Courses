module counter
(
	clk,
	reset,
	result,
	ena
);

	input clk;
	input reset;
	input ena;
	output [7:0] result;

	reg [7:0] result;

	always @(posedge clk or posedge reset)
	begin
		if (reset) 
			result = 0;		
		else if (ena) 
			result = result + 1;
	end
endmodule		
