module t_flipflop(
    q, q_bar,
    toggle, clk, rst
);

    input toggle, clk, rst;
    output reg q, q_bar;

    initial begin
        q = 0;
        q_bar = 1;
    end

    always@(negedge clk or posedge rst) begin // needs a negedge here
        if (rst) begin
            q = 0;
        end else begin
            case(toggle)
                1'b0:
                    q = q;

                1'b1:
                    q = q_bar;
            endcase
        end
        q_bar = ~q;
    end 


endmodule