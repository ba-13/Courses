module mealy_sequence_detector(rst, ip, op);

	output reg op;
 	input clk, rst, ip;

	 reg [2:0] state;

	 parameter [2:0] s0=3'b000;
	 parameter [2:0] s1=3'b001;
	 parameter [2:0] s2=3'b010;
	 parameter [2:0] s3=3'b011;
	 parameter [2:0] s4=3'b100;

     initial begin
        state <= s0;
     end

     always @(ip or rst) begin
        if (rst)
            state <= s0;
        else begin
            case (state)
                s0: begin
                        if (ip) begin
                            state <= s1;
                        end else begin
                            state <= s0;
                        end
                        op <= 0;
                    end
                s1: begin
                        if (ip) begin
                            state <= s1;
                        end else begin
                            state <= s2;
                        end
                        op <= 0;
                    end
                s2: begin
                        if (ip) begin
                            state <= s3;
                        end else begin
                            state <= s0;
                        end
                        op <= 0;
                    end
                s3: begin
                        if (ip) begin
                            state <= s1;
                        end else begin
                            state <= s4;
                        end
                        op <= 0;
                    end
                s4: begin
                        if (ip) begin
                            state <= s3;
                        end else begin
                            state <= s0;
                        end
                        op <= 1;
                    end
                default: begin
                        state <= s0;
                        op <= 0;
                end
            endcase
        end
     end

endmodule