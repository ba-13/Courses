module memory (
    input [k-1:0] address,
    input read_enable,
    input write_enable,
    input [REG_SIZE-1:0] data_in,
    output reg corrupted,
    output reg [REG_SIZE-1:0] data_out
);

  parameter k = 5;
  // parameter MEM_SIZE = (1 << k) > (1 << 15) ? (1 << 15) : (1 << k);
  parameter MEM_SIZE = 1024;  // fixed memory, don't access outside
  parameter REG_SIZE = 32;

  reg [REG_SIZE-1:0] memory[0:MEM_SIZE-1];

  always @(*) begin
    if (write_enable) begin
      memory[address] <= data_in;
      $display("[memory]: write %h to address %d", data_in, address);
    end
  end

  always @(*) begin
    corrupted <= 1'b0;
    if (read_enable) begin
      if (address > MEM_SIZE) begin
        $display("[memory]: address %d is out of range", address);
        corrupted <= 1'b1;
        // $finish;
      end
      data_out <= memory[address];
    end else begin
      data_out <= 0;
    end
  end

endmodule
