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

    // Clock generation (clk explicitly initialized here to avoid a race
    // against the reset-sequence initial block leaving it at X)
    initial begin
        clk = 1'b0;
        forever #5 clk = ~clk;  // 10ns period clock (100MHz)
    end

    // Reset sequence
    initial begin
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
        c_i = 32'h40400000;  // 3.0 in FP32 (sign=0, exp=128, mant=0.5)
        
        valid_i = 1'b1;
        
        @(posedge clk);
        #1 valid_i = 1'b0;

        // Wait for output to be valid
        wait (valid_o === 1'b1);
        
        // Check result
        if (d_o == 32'h40A00000) begin
            $display("PASS: Basic operation result = 5.0");
        end else begin
            $display("FAIL: Basic operation result = %h, expected = 40A00000", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    endtask

    // Test 2: Zero multiplication
    task test_zero_multiplication();
        $display("Test 2: Zero multiplication");
        
        // Set one operand to zero
        a_i = 16'h0000;  // 0.0 in FP16
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40000000;  // 2.0 in FP32
        
        valid_i = 1'b1;
        
        @(posedge clk);
        #1 valid_i = 1'b0;

        // Wait for output to be valid
        wait (valid_o === 1'b1);
        
        // Check result (0 * 2.0 + 2.0 = 2.0)
        if (d_o == 32'h40000000) begin
            $display("PASS: Zero multiplication result = 2.0");
        end else begin
            $display("FAIL: Zero multiplication result = %h, expected = 40000000", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    endtask

    // Test 3: Subnormal inputs with FTZ
    task test_subnormal_inputs();
        $display("Test 3: Subnormal inputs with FTZ");
        
        // Set subnormal inputs (should be flushed to zero)
        a_i = 16'h0001;  // Subnormal FP16 value
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40000000;  // 2.0 in FP32
        
        valid_i = 1'b1;
        
        @(posedge clk);
        #1 valid_i = 1'b0;

        // Wait for output to be valid
        wait (valid_o === 1'b1);
        
        // Check result (0 * 2.0 + 2.0 = 2.0)
        if (d_o == 32'h40000000) begin
            $display("PASS: Subnormal input handling result = 2.0");
        end else begin
            $display("FAIL: Subnormal input handling result = %h, expected = 40000000", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    endtask

    // Test 4: NaN propagation
    task test_nan_propagation();
        $display("Test 4: NaN propagation");
        
        // Set one operand to NaN
        a_i = 16'h7E00;  // Quiet NaN in FP16 (exp=all-1s, mantissa!=0)
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40000000;  // 2.0 in FP32
        
        valid_i = 1'b1;
        
        @(posedge clk);
        #1 valid_i = 1'b0;

        // Wait for output to be valid
        wait (valid_o === 1'b1);
        
        // Check that invalid flag is set
        if (invalid_o == 1'b1) begin
            $display("PASS: NaN propagation sets invalid flag");
        end else begin
            $display("FAIL: NaN propagation did not set invalid flag");
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    endtask

    // Test 5: Infinity operations
    task test_infinity_operations();
        $display("Test 5: Infinity operations");
        
        // Set one operand to infinity
        a_i = 16'h7C00;  // Infinity in FP16
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40000000;  // 2.0 in FP32
        
        valid_i = 1'b1;
        
        @(posedge clk);
        #1 valid_i = 1'b0;

        // Wait for output to be valid
        wait (valid_o === 1'b1);
        
        // Check result (infinity * 2.0 + 2.0 = infinity)
        if ((d_o[31] == 1'b1 && d_o[30:23] == 8'hFF) || 
            (d_o[31] == 1'b0 && d_o[30:23] == 8'hFF)) begin
            $display("PASS: Infinity operation result = infinity");
        end else begin
            $display("FAIL: Infinity operation result = %h", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    endtask

    // Test 6: Overflow detection
    task test_overflow_detection();
        $display("Test 6: Overflow detection");
        
        // Set inputs that will cause overflow
        a_i = 16'h7BFF;  // Large positive FP16 value (close to max)
        b_i = 16'h7BFF;  // Large positive FP16 value (close to max)
        c_i = 32'h7F800000;  // Infinity in FP32
        
        valid_i = 1'b1;
        
        @(posedge clk);
        #1 valid_i = 1'b0;

        // Wait for output to be valid
        wait (valid_o === 1'b1);
        
        // Check that overflow flag is set
        if (overflow_o == 1'b1) begin
            $display("PASS: Overflow detection works correctly");
        end else begin
            $display("FAIL: Overflow detection did not set overflow flag");
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    endtask

    // Test 7: FTZ-to-exact-zero is NOT underflow
    task test_underflow_detection();
        $display("Test 7: FTZ result is exact zero, underflow must NOT assert");

        // Both operands subnormal FP16 (flushed to zero by FTZ), C is zero.
        // The mathematically exact result is 0.0 -- IEEE 754 does not flag
        // underflow for an exact-zero result, only for loss of precision.
        a_i = 16'h0001;  // Subnormal FP16 value
        b_i = 16'h0001;  // Subnormal FP16 value
        c_i = 32'h00000000;  // Zero in FP32

        valid_i = 1'b1;
        @(posedge clk);
        #1 valid_i = 1'b0;

        // Wait for output to be valid
        wait (valid_o === 1'b1);

        // Exact zero result: underflow_o must be clear
        if (underflow_o == 1'b0 && d_o == 32'h00000000) begin
            $display("PASS: FTZ exact-zero result does not assert underflow");
        end else begin
            $display("FAIL: underflow_o=%b d_o=%h, expected underflow_o=0, d_o=00000000",
                      underflow_o, d_o);
        end

        valid_i = 1'b0;
        @(posedge clk);
    endtask

    // Test 8: Pipeline stall and resume
    task test_pipeline_stall();
        $display("Test 8: Pipeline stall and resume");

        // Let any prior transaction fully drain (valid_o deassert) before
        // touching ready_i -- clearing it in the same edge as a pending
        // drain would freeze the stale result in place instead.
        @(posedge clk);
        #1;

        // Send data with ready_i = 0 (stall pipeline)
        ready_i = 1'b0;

        a_i = 16'h3C00;  // 1.0 in FP16
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40000000;  // 2.0 in FP32
        
        valid_i = 1'b1;
        @(posedge clk);
        #1 valid_i = 1'b0;

        // Resume pipeline
        ready_i = 1'b1;

        // Wait for output to be valid
        wait (valid_o === 1'b1);
        
        // Check result (1.0 * 2.0 + 2.0 = 4.0)
        if (d_o == 32'h40800000) begin
            $display("PASS: Pipeline stall/resume works correctly");
        end else begin
            $display("FAIL: Pipeline stall/resume result = %h, expected = 40800000", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    endtask

    // Test 9: Back-to-back operations
    task test_back_to_back_operations();
        $display("Test 9: Back-to-back operations");
        
        // First operation: 1.0 * 2.0 + 5.0 = 7.0
        a_i = 16'h3C00;  // 1.0 in FP16
        b_i = 16'h4000;  // 2.0 in FP16
        c_i = 32'h40A00000;  // 5.0 in FP32

        valid_i = 1'b1;
        @(posedge clk);
        #1 valid_i = 1'b0;

        // Wait for output to be valid
        wait (valid_o === 1'b1);

        if (d_o == 32'h40E00000) begin
            $display("PASS: First operation result = 7.0");
        end else begin
            $display("FAIL: First operation result = %h, expected = 40E00000", d_o);
        end

        // Let the first result's valid_o deassert before issuing the
        // second -- otherwise the wait() below would trivially pass on
        // the still-lingering first result instead of the new one.
        wait (valid_o === 1'b0);

        // Second operation: 2.0 * 3.0 + 4.0 = 10.0
        a_i = 16'h4000;  // 2.0 in FP16
        b_i = 16'h4200;  // 3.0 in FP16 (exp=16, mant=0.5)
        c_i = 32'h40800000;  // 4.0 in FP32

        valid_i = 1'b1;
        @(posedge clk);
        #1 valid_i = 1'b0;

        // Wait for output to be valid
        wait (valid_o === 1'b1);

        if (d_o == 32'h41200000) begin
            $display("PASS: Second operation result = 10.0");
        end else begin
            $display("FAIL: Second operation result = %h, expected = 41200000", d_o);
        end
        
        valid_i = 1'b0;
        @(posedge clk);
    endtask

endmodule : tb_fp16_mac