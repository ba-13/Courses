module alu_control (
    input [5:0] funct,
    input [5:0] alu_op,
    output reg [3:0] alu_control
);

  always @(*) begin
    case (alu_op)
      6'b000000:
      case (funct)
        6'b000000: alu_control = 4'b0000;  // sll
        6'b000010: alu_control = 4'b0001;  // srl
        6'b100000: alu_control = 4'b0010;  // add
        6'b100001: alu_control = 4'b0011;  // addu
        6'b100010: alu_control = 4'b0100;  // sub
        6'b100011: alu_control = 4'b0101;  // subu
        6'b100100: alu_control = 4'b0110;  // and
        6'b100101: alu_control = 4'b0111;  // or
        6'b100110: alu_control = 4'b1000;  // xor
        6'b100111: alu_control = 4'b1001;  // nor
        6'b101010: alu_control = 4'b1010;  // slt
        6'b101011: alu_control = 4'b1011;  // sltu
        6'b101100: alu_control = 4'b1100;  // seq
        default: begin
          alu_control = 4'b1111;
          $display("[ALU]: Error in funct field: %b", funct);
        end
      endcase
      6'b000101: alu_control = 4'b1100;  // bne
      6'b001000: alu_control = 4'b0010;  // addi
      6'b001001: alu_control = 4'b0011;  // addiu
      6'b001010: alu_control = 4'b1010;  // slti
      6'b001100: alu_control = 4'b0110;  // andi
      6'b001101: alu_control = 4'b0111;  // ori
      6'b001110: alu_control = 4'b1000;  // xori
      6'b001111: alu_control = 4'b1001;  // nori
      6'b100011: alu_control = 4'b0011;  // lw
      6'b101011: alu_control = 4'b0011;  // sw
    endcase
  end

endmodule
