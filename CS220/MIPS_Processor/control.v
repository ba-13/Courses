module control (
    input [5:0] opcode,
    input register_writing_done,
    input [31:0] alu_result,
    output [5:0] alu_op,
    output reg reg_dst,
    output reg jump,
    output reg branch,
    output reg mux_mem_to_registers,
    output reg memory_read,
    output reg alu_source,
    output memory_write,
    output register_write,
    input clk
);

  reg should_write_register;
  reg mem_write;
  reg alu_output_changed;
  assign register_write = should_write_register & ~register_writing_done;
  assign memory_write = alu_output_changed & mem_write;
  // note that register_writing_done and alu_output_changed are resetted in the next clock cycle

  assign alu_op = opcode;

  always @(alu_result) begin
    alu_output_changed <= 1'b1;
  end

  always @(posedge clk) begin
    should_write_register <= 0;
  end

  always @(opcode, posedge clk) begin  // freaking put posedge clk here
    case (opcode)
      6'b000000: begin
        // R-type
        reg_dst <= 1'b1;
        jump <= 1'b0;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b0;
        alu_source <= 1'b0;  // from register to ALU
        should_write_register <= 1'b1;
      end
      6'b000010: begin
        // j
        reg_dst <= 1'b0;
        jump <= 1'b1;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b0;
        alu_source <= 1'b0;  // from register to ALU
        should_write_register <= 1'b0;
      end
      6'b000101: begin
        // bne
        reg_dst <= 1'b0;
        jump <= 1'b0;
        branch <= 1'b1;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b0;
        alu_source <= 1'b0;  // from register to ALU
        should_write_register <= 1'b0;
      end
      6'b001000: begin
        // addi
        reg_dst <= 1'b0;
        jump <= 1'b0;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b0;
        alu_source <= 1'b1;  // from immediate to ALU
        should_write_register <= 1'b1;
      end
      6'b001001: begin
        // addiu
        reg_dst <= 1'b0;
        jump <= 1'b0;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b0;
        alu_source <= 1'b1;  // from immediate to ALU
        should_write_register <= 1'b1;
      end
      6'b001010: begin
        // slti
        reg_dst <= 1'b0;
        jump <= 1'b0;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b0;
        alu_source <= 1'b1;  // from immediate to ALU
        should_write_register <= 1'b1;
      end
      6'b001100: begin
        // andi
        reg_dst <= 1'b0;
        jump <= 1'b0;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b0;
        alu_source <= 1'b1;  // from immediate to ALU
        should_write_register <= 1'b1;
      end
      6'b001101: begin
        // ori
        reg_dst <= 1'b0;
        jump <= 1'b0;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b0;
        alu_source <= 1'b1;  // from immediate to ALU
        should_write_register <= 1'b1;
      end
      6'b001110: begin
        // xori
        reg_dst <= 1'b0;
        jump <= 1'b0;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b0;
        alu_source <= 1'b1;  // from immediate to ALU
        should_write_register <= 1'b1;
      end
      6'b001111: begin
        // nori
        reg_dst <= 1'b0;
        jump <= 1'b0;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b0;
        alu_source <= 1'b1;  // from immediate to ALU
        should_write_register <= 1'b1;
      end
      6'b100011: begin
        // lw
        reg_dst <= 1'b0;
        jump <= 1'b0;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b1;  // from memory to register
        memory_read <= 1'b1;
        mem_write <= 1'b0;
        alu_source <= 1'b1;  // from immediate to ALU
        should_write_register <= 1'b1;
      end
      6'b101011: begin
        // sw
        reg_dst <= 1'b0;
        jump <= 1'b0;
        branch <= 1'b0;
        mux_mem_to_registers <= 1'b0;  // from ALU to register
        memory_read <= 1'b0;
        mem_write <= 1'b1;
        alu_source <= 1'b1;  // from immediate to ALU
        should_write_register <= 1'b0;
      end
      default: begin
        $display("[Control]: Error in opcode field: %b", opcode);
      end
    endcase
  end

endmodule
