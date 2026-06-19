```systemverilog
// File: tb_fp16_mac.sv
// Description: Testbench for FP16 MAC unit verification
//              Tests the RTL implementation against specification and verification plan

`default_nettype none

module tb_fp16_mac;

    // Clock and reset signals
    logic clk;
    logic rst_n;

    // Input signals
    logic [15:0] a_i;
    logic [15:0] b_i;
    logic [31:0] c_i;
    logic valid_i;
    logic ready_o;

    // Output signals
    logic [31:0] d_o;
    logic valid_o;
    logic ready_i;

    // Exception flags
    logic overflow_o;
    logic underflow_o;
    logic invalid_o;

    // Instantiate the DUT
    fp16_mac uut (
        .clk(clk),
        .rst_n(rst_n),
        .a_i(a_i),
        .b_i(b_i),
        .c_i(c_i),
        .valid_i(valid_i),
        .ready_o(ready_o),
        .d_o(d_o),
        .valid_o(valid_o),
        .ready_i(ready_i),
        .overflow_o(overflow_o),
        .underflow_o(underflow_o),
        .invalid_o(invalid_o)
    );

    // Clock generation
    always #5 clk = ~clk;  // 10ns period clock (100MHz)

    // Reset sequence
    initial begin
        clk = 0;
        rst_n = 0;
        
        // Apply reset for 20ns
        #20 rst_n = 1;
        
        // Wait for a few cycles to ensure stable state
        #50;
        
        $display("Testbench initialized at time %t", $time);
    end

    // Test sequence
    initial begin
        // Initialize signals
        a_i = 16'h0000;
        b_i = 16'h0000;
        c_i = 32'h00000000;
        valid_i = 1'b0;
        ready_i = 1'b1;

        // Wait for reset to be released
        #50;

        $display("Starting test sequence at time %t", $time);

        // Test 1: Basic operation - 1.0 * 2.0 + 3.0 = 5.0
        test_basic_operation();
        
        // Test 2: Zero multiplication
        test_zero_multiplication();
        
        // Test 3: Subnormal inputs with FTZ
        test_subnormal_inputs();
        
        // Test 4: NaN propagation
        test_nan_propagation();
        
        // Test 5: Infinity operations
        test_infinity_operations();
        
        // Test 6: Overflow detection
        test_overflow_detection();
        
        // Test 7: Underflow detection
        test_underflow_detection();
        
        // Test 8: Pipeline stall and resume
        test_pipeline_stall();
        
        // Test 9: Back-to-back operations
        test_back_to_back_operations();
        
        $display("All tests completed at time %t", $time);
        $finish;
    end

    // Test 1: Basic operation - 1.0 * 2.0 + 3.0 = 5.0
    task test_basic_operation();
        $display("Test 1: Basic operation - 1.0 * 2.0 + 3.0 = 5.0");
        
        // Set inputs for 1.0 * 2.0 + 3.0
        a_i = 16'h3C00;  // 1.0 in FP16 (sign=0, exp=15, mant=0)
        b_i = 16'h4000;  // 2.0 in FP16 (sign=0, exp=15, mant=0) 
        c_i = 32'h40A00000;  // 5.0 in FP32 (sign=0, exp=128, mant=512)
        
        valid_i = 1'b1;
        
        // Wait for input to be accepted
        @(posedge clk);
        while (!ready_o) @(posedge clk);
        
        // Wait for output to be valid
        @(posedge clk);
        while (!valid_o) @(posedge clk);
        
        // Check result
        if (d_o == 32'h40A00000) begin
            $display("PASS: Basic operation result = 5.0");
        end else begin
            $display("FAIL: Basic operation result = %h, expected = 40A00000", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    end

    // Test 2: Zero multiplication
    task test_zero_multiplication();
        $display("Test 2: Zero multiplication");
        
        // Set one operand to zero
        a_i = 16'h0000;  // 0.0 in FP16
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40000000;  // 2.0 in FP32
        
        valid_i = 1'b1;
        
        // Wait for input to be accepted
        @(posedge clk);
        while (!ready_o) @(posedge clk);
        
        // Wait for output to be valid
        @(posedge clk);
        while (!valid_o) @(posedge clk);
        
        // Check result (0 * 2.0 + 2.0 = 2.0)
        if (d_o == 32'h40000000) begin
            $display("PASS: Zero multiplication result = 2.0");
        end else begin
            $display("FAIL: Zero multiplication result = %h, expected = 40000000", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    end

    // Test 3: Subnormal inputs with FTZ
    task test_subnormal_inputs();
        $display("Test 3: Subnormal inputs with FTZ");
        
        // Set subnormal inputs (should be flushed to zero)
        a_i = 16'h0001;  // Subnormal FP16 value
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40000000;  // 2.0 in FP32
        
        valid_i = 1'b1;
        
        // Wait for input to be accepted
        @(posedge clk);
        while (!ready_o) @(posedge clk);
        
        // Wait for output to be valid
        @(posedge clk);
        while (!valid_o) @(posedge clk);
        
        // Check result (0 * 2.0 + 2.0 = 2.0)
        if (d_o == 32'h40000000) begin
            $display("PASS: Subnormal input handling result = 2.0");
        end else begin
            $display("FAIL: Subnormal input handling result = %h, expected = 40000000", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    end

    // Test 4: NaN propagation
    task test_nan_propagation();
        $display("Test 4: NaN propagation");
        
        // Set one operand to NaN
        a_i = 16'h7C00;  // NaN in FP16
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40000000;  // 2.0 in FP32
        
        valid_i = 1'b1;
        
        // Wait for input to be accepted
        @(posedge clk);
        while (!ready_o) @(posedge clk);
        
        // Wait for output to be valid
        @(posedge clk);
        while (!valid_o) @(posedge clk);
        
        // Check that invalid flag is set
        if (invalid_o == 1'b1) begin
            $display("PASS: NaN propagation sets invalid flag");
        end else begin
            $display("FAIL: NaN propagation did not set invalid flag");
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    end

    // Test 5: Infinity operations
    task test_infinity_operations();
        $display("Test 5: Infinity operations");
        
        // Set one operand to infinity
        a_i = 16'h7C00;  // Infinity in FP16
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40000000;  // 2.0 in FP32
        
        valid_i = 1'b1;
        
        // Wait for input to be accepted
        @(posedge clk);
        while (!ready_o) @(posedge clk);
        
        // Wait for output to be valid
        @(posedge clk);
        while (!valid_o) @(posedge clk);
        
        // Check result (infinity * 2.0 + 2.0 = infinity)
        if ((d_o[31] == 1'b1 && d_o[30:23] == 8'hFF) || 
            (d_o[31] == 1'b0 && d_o[30:23] == 8'hFF)) begin
            $display("PASS: Infinity operation result = infinity");
        end else begin
            $display("FAIL: Infinity operation result = %h", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    end

    // Test 6: Overflow detection
    task test_overflow_detection();
        $display("Test 6: Overflow detection");
        
        // Set inputs that will cause overflow
        a_i = 16'h7BFF;  // Large positive FP16 value (close to max)
        b_i = 16'h7BFF;  // Large positive FP16 value (close to max)
        c_i = 32'h7F800000;  // Infinity in FP32
        
        valid_i = 1'b1;
        
        // Wait for input to be accepted
        @(posedge clk);
        while (!ready_o) @(posedge clk);
        
        // Wait for output to be valid
        @(posedge clk);
        while (!valid_o) @(posedge clk);
        
        // Check that overflow flag is set
        if (overflow_o == 1'b1) begin
            $display("PASS: Overflow detection works correctly");
        end else begin
            $display("FAIL: Overflow detection did not set overflow flag");
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    end

    // Test 7: Underflow detection
    task test_underflow_detection();
        $display("Test 7: Underflow detection");
        
        // Set inputs that will cause underflow
        a_i = 16'h0001;  // Subnormal FP16 value
        b_i = 16'h0001;  // Subnormal FP16 value  
        c_i = 32'h00000000;  // Zero in FP32
        
        valid_i = 1'b1;
        
        // Wait for input to be accepted
        @(posedge clk);
        while (!ready_o) @(posedge clk);
        
        // Wait for output to be valid
        @(posedge clk);
        while (!valid_o) @(posedge clk);
        
        // Check that underflow flag is set (due to FTZ)
        if (underflow_o == 1'b1) begin
            $display("PASS: Underflow detection works correctly");
        end else begin
            $display("FAIL: Underflow detection did not set underflow flag");
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    end

    // Test 8: Pipeline stall and resume
    task test_pipeline_stall();
        $display("Test 8: Pipeline stall and resume");
        
        // Send data with ready_i = 0 (stall pipeline)
        ready_i = 1'b0;
        
        a_i = 16'h3C00;  // 1.0 in FP16
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40000000;  // 2.0 in FP32
        
        valid_i = 1'b1;
        
        // Wait for input to be accepted (should not happen due to stall)
        @(posedge clk);
        while (!ready_o) @(posedge clk);
        
        // Resume pipeline
        ready_i = 1'b1;
        
        // Wait for output to be valid
        @(posedge clk);
        while (!valid_o) @(posedge clk);
        
        // Check result (1.0 * 2.0 + 2.0 = 4.0)
        if (d_o == 32'h40800000) begin
            $display("PASS: Pipeline stall/resume works correctly");
        end else begin
            $display("FAIL: Pipeline stall/resume result = %h, expected = 40800000", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    end

    // Test 9: Back-to-back operations
    task test_back_to_back_operations();
        $display("Test 9: Back-to-back operations");
        
        // First operation: 1.0 * 2.0 + 3.0 = 5.0
        a_i = 16'h3C00;  // 1.0 in FP16
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40A00000;  // 5.0 in FP32
        
        valid_i = 1'b1;
        
        // Wait for input to be accepted
        @(posedge clk);
        while (!ready_o) @(posedge clk);
        
        // Wait for output to be valid
        @(posedge clk);
        while (!valid_o) @(posedge clk);
        
        if (d_o == 32'h40A00000) begin
            $display("PASS: First operation result = 5.0");
        end else begin
            $display("FAIL: First operation result = %h, expected = 40A00000", d_o);
        end
        
        // Second operation: 2.0 * 3.0 + 1.0 = 7.0
        a_i = 16'h4000;  // 2.0 in FP16
        b_i = 16'h4400;  // 3.0 in FP16
        c_i = 32'h40800000;  // 4.0 in FP32
        
        valid_i = 1'b1;
        
        // Wait for input to be accepted
        @(posedge clk);
        while (!ready_o) @(posedge clk);
        
        // Wait for output to be valid
        @(posedge clk);
        while (!valid_o) @(posedge clk);
        
        if (d_o == 32'h41C00000) begin
            $display("PASS: Second operation result = 7.0");
        end else begin
            $display("FAIL: Second operation result = %h, expected = 41C00000", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    end

endmodule : tb_fp16_mac
```