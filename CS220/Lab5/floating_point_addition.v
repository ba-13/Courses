module floating_point_addition(
    input [size-1:0] a,
    input [size-1:0] b,
    input reset,
    input clk,
    output reg [size-1:0] C,
    output reg corrupt,
    output reg done
);

    parameter size = 32;
    parameter exp_base = 23;
    parameter   Ssave = 0, 
                Scompare = 1, 
                SsaveMantissa = 2, 
                Sshift = 3, 
                SadjustMagnitude = 4, 
                Sadd = 5, 
                SbackToC = 6, 
                SinvertMantissaC = 7, 
                SnormalizeC = 8,
                SmantissaBackToC = 9, 
                Sstay = 10;

    // Intermediate variables
    reg [size-1:0] A, B;
    reg [exp_base+2:0] signed_a, signed_b, signed_c;
    reg [3:0] mode, next_mode;

    // update state
    always @(posedge clk, reset) begin
        if(reset) begin
            mode <= Ssave;
            done <= 1'b0;
        end
        else if(done)
            mode <= Sstay;
        else
            mode <= next_mode; 
    end

    always @(mode, clk) begin
        next_mode <= Ssave; // initialises the FSM
        case(mode)

            Ssave: begin
                /**
                Save A and B in intermediate registers for operation
                Exchange A and B if B > A
                */
                A <= a;
                B <= b;
                // $display("[DBG] save:");
                next_mode <= Scompare;
            end

            Scompare: begin
                if (A[size-2:exp_base] < B[size-2:exp_base]) begin
                    // lesser exponent A
                    A <= B;
                    B <= A;
                end
                // $display("[DBG] compare:");
                next_mode <= SsaveMantissa;
            end

            SsaveMantissa: begin
                // 0 0 1 . mantissa
                // ^ ^ ^
                // | | - hidden 1 at start
                // | - for magnitude not to overflow
                // - for sign
                // at the below start, signed_a, signed_b just store the magnitudes of A, B mantissas
                signed_a <= {1'b0, {1'b0, {1'b1, A[exp_base-1:0]}}};    
                signed_b <= {1'b0, {1'b0, {1'b1, B[exp_base-1:0]}}};
                // $display("[DBG] saveMantissa: A=%b %b, B=%b %b", A[size-1], {1'b1, A[exp_base-1:0]}, B[size-1], {1'b1, B[exp_base-1:0]});
                // $display("[DBG] saveMantissa: signed_a=%b, signed_b=%b", signed_a, signed_b);
                next_mode <= Sshift;
            end

            Sshift: begin
                /**
                Shift register B left until exponent matches
                */
                if (A[size-2:exp_base] > B[size-2:exp_base]) begin
                    signed_b[exp_base:0] <= signed_b[exp_base:0] >> 1;
                    B[size-2:exp_base] <= B[size-2:exp_base] + 1;
                    // $display("[DBG] shift: shifted signed_b to %b", signed_b[exp_base-1:0]);
                    next_mode <= Sshift;                                    
                end else begin
                    // $display("[DBG] shift: exponent final value %b", B[size-2:exp_base]);
                    next_mode <= SadjustMagnitude;
                end
            end

            SadjustMagnitude: begin
                //! let the following happen only once
                if (A[size-1] == 1'b1) begin
                    signed_a <= ~signed_a + 1;
                    // $display("[DBG] adjustMagnitude: set inverted magnitude A");
                end
                if (B[size-1] == 1'b1) begin
                    signed_b <= ~signed_b + 1;
                    // $display("[DBG] adjustMagnitude: set inverted magnitude B");
                end
                next_mode <= Sadd;
            end

            Sadd: begin
                /**
                now exponents should match
                put them together
                */
                signed_c <= signed_a + signed_b;                    // this would lead to dropping of Cout
                // $display("signed_a=%b", signed_a);
                // $display("signed_b=%b", signed_b);
                // $display("signed_c=%b", signed_c);
                next_mode <= SbackToC;
            end

            SbackToC: begin
                C[size-1] <= signed_c[exp_base+2];                  // assign sign as the MSB
                C[size-2:exp_base] <= A[size-2:exp_base];           // assign exponent of any number
                // $display("[DBG] backToC: C=%b", C);
                next_mode <= SinvertMantissaC;
            end

            SinvertMantissaC: begin                                 // store the magnitude
                if (signed_c[exp_base+2] == 1'b1) begin
                    signed_c[exp_base+1:0] <= ~signed_c[exp_base+1:0] + 1; // signed_a storing intermediate 2's complement
                    // $display("[DBG] invertMantissaC: Inverting");
                end else begin
                    signed_c[exp_base+1:0] <= signed_c[exp_base+1:0];
                end
                next_mode <= SnormalizeC;
            end

            SnormalizeC: begin
                // ! running even when condition should be false
                if (signed_c[23+1] == 1'b1) begin
                    signed_c[exp_base+1:0] <= signed_c[exp_base+1:0] >> 1;
                    C[size-2:exp_base] <= C[size-2:exp_base] + 1;
                    // $display("[DBG] normalizeC: shift right signed_c=%b", signed_c);
                    next_mode <= next_mode;

                end else if (signed_c[exp_base] == 1'b0) begin                  // else shift right
                    signed_c[exp_base+1:0] <= signed_c[exp_base+1:0] << 1;
                    C[size-2:exp_base] <= C[size-2:exp_base] - 1;
                    // $display("[DBG] normalizeC: shift left signed_a=%b", signed_c);
                    next_mode <= next_mode;

                end else begin
                    C[exp_base-1:0] <= signed_c[exp_base-1:0];
                    // $display("[DBG] normalizeC: saved");
                    // $display("[DBG] normalizeC: signed_c=%b", signed_c);
                    next_mode <= SmantissaBackToC;
                end
            end

            SmantissaBackToC: begin
                C[exp_base-1:0] <= signed_c[exp_base-1:0];    
                next_mode <= Sstay;        
                done <= 1;
            end

            Sstay: begin
                // $display("[DBG] stay:");
                next_mode <= Sstay;
            end
            default: begin
                corrupt <= 1;
            end 
        endcase

    end

endmodule