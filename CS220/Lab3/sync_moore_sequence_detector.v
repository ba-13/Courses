module sync_moore_sequence_detector(rst, clk, ip, op);

	output reg op;
 	input clk, rst, ip;

    reg [2:0] state;
    reg [2:0] next_state;

    parameter [2:0] s0=3'b000;
    parameter [2:0] s1=3'b001;
    parameter [2:0] s2=3'b010;
    parameter [2:0] s3=3'b011;
    parameter [2:0] s4=3'b100;

    initial begin
        state <= s0;
    end

    always @(posedge clk, posedge rst)
    begin
    if (rst)
        state = s0;
    else
        state = next_state;

    case (state)
        s0: op <= 0;
        s1: op <= 0;
        s2: op <= 0;
        s3: op <= 0;
        s4: op <= 1;
        default: op <= 0;
    endcase
    end

    always @(state, ip) begin
    case (state)
        s0:
            if (ip) next_state <= s1;
            else next_state <= s0;
        s1:
            if (ip) next_state <= s1;
            else next_state <= s2;
        s2:
            if (ip) next_state <= s3;
            else next_state <= s0;
        s3:
            if (ip) next_state <= s1;
            else next_state <= s4;
        s4:
            if (ip) next_state <= s3;
            else next_state <= s0;
        default:
            next_state <= s0;
    endcase
    end

endmodule