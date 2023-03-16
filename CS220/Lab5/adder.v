`include "floating_point_addition.v"
`include "veda.v"

module adder(
    input clk,
    input reset,
    output reg flag
);
    reg write_enable;
    reg mode;
    reg [4:0] addr;
    reg [31:0] datain;
    wire [31:0] dataout;
    reg [31:0] A, B;
    wire [31:0] answer;
    wire corrupt, done;
    reg rst_;
    reg [3:0] counter;

    parameter Aval = 32'b01000011001010100100000000000000;
    parameter Bval = 32'b11000011011001011010000000000000;
    parameter Cval = 32'b11000010011011011000000000000000;

    parameter Aaddr = 5'b00001;
    parameter Baddr = 5'b00010;
    parameter Caddr = 5'b00011;

    veda storage(.clk(clk),
                 .rst(reset),
                 .write_enable(write_enable),
                 .addr(addr), 
                 .mode(mode), 
                 .datain(datain), 
                 .dataout(dataout));
    floating_point_addition add(.a(A), 
                                .b(B), 
                                .C(answer), 
                                .reset(rst_), 
                                .clk(clk), 
                                .corrupt(corrupt), 
                                .done(done));

    parameter S0 = 0, S1 = 1, S2 = 2, S3 = 3, S4 = 4, S5 = 5, Soff = 6;
    reg [3:0] PS, NS;
    always @(posedge clk, reset) begin
        if (reset) begin
            counter <= 0;
            rst_ <= 1;
            PS <= S0;
        end else begin
            rst_ <= 0;
            PS <= NS;
        end
    end

    always @(PS, posedge clk) begin
        NS <= S0;
        case (PS)
            S0: begin
                write_enable <= 1;
                addr <= Aaddr;
                datain <= Aval;
                mode <= 0;
                counter <= counter + 1;
                if (counter == 3) begin    // waiting till value has been set
                    NS <= S1;
                    counter <= 0;
                end
                else
                    NS <= S0;
            end
            S1: begin
                addr <= Baddr;
                datain <= Bval;
                mode <= 0;
                counter <= counter + 1;
                if (counter == 3) begin
                    NS <= Soff;
                    counter <= 0;
                end
                else
                    NS <= S1;
            end
            Soff: begin
                write_enable <= 0;
                NS <= S2;
            end
            S2: begin
                addr <= Aaddr;
                A <= dataout;
                counter <= counter + 1;
                if (counter == 3) begin
                    NS <= S3;
                    counter <= 0;
                end
                else
                    NS <= S2;
            end
            S3: begin
                addr <= Baddr;
                B <= dataout;
                counter <= counter + 1;
                if (counter == 3) begin
                    NS <= S4;
                    counter <= 0;
                end
                else
                    NS <= S3;
            end
            S4: begin
                rst_ <= 1;
                if (done) begin
                    write_enable <= 1;
                    addr = Caddr;
                    datain <= answer;
                    mode <= 0;
                    counter <= counter + 1;
                    if (counter == 3) begin
                        NS <= S5;
                        counter <= 0;
                    end else begin
                        NS <= S4;
                    end
                end else begin
                    NS <= S4;
                end
            end
            S5: begin
                if (answer == Cval) begin
                    flag <= 1'b1;
                end else begin
                    flag <= 1'b0;
                end
                NS <= S5;
            end
            default: begin
                NS <= S0;
            end
        endcase
    end

endmodule