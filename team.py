import os
from crewai import Crew, LLM, Process, Agent, Task

os.environ["OPENAI_MODEL_NAME"] = "ollama/qwen3-coder:30b-32k" # Ensure we don't use GPT to enforce data security
os.environ["OPENAI_API_KEY"] = "ollama"
os.environ["OPENAI_BASE_URL"] = "http://localhost:11434"
os.environ["OPENAI_API_BASE"] = "http://localhost:11434"

#manager_psyche = "ollama/deepseek-r1:32b-16k" # Used for complex reasoning and planning
engineer_psyche = "ollama/qwen3-coder:30b-32k" # Very promising. Going to stick with this and see where it takes the project

#llm_architect = LLM(model=manager_psyche, base_url="http://localhost:11434", temperature=0.4)
llm_architect = LLM(model=engineer_psyche, base_url="http://localhost:11434", temperature=0.4)
llm_engineer = LLM(model=engineer_psyche, base_url="http://localhost:11434", temperature=0.2)

local_embedder = { # Again, enforce data security
    "provider": "ollama",
    "config": {
        "model": "nomic-embed-text",
        "base_url": "http://localhost:11434"
    }
}

lead_architect = Agent(
    role="Principle RTL Architect",
    goal="Define specifications for a high-performance FP16 Multiply-Accumulate (MAC) unit.",
    backstory="You are a seasoned silicon architect specializing in arithmetic compute blocks for AI accelerators."
        "You understand IEEE 754 half-precision limitations, rounding modes (round-to-nearest-even),"
        "overflow/underflow exception handling, and pipelining structures for maximum frequency.",
    llm=llm_architect,
    verbose=True,
    allow_delegation=False,
    memory=False # This seems hardcoded to use a GPT model when enabled
)

rtl_engie = Agent(
    role="Senior Digital Design Engineer",
    goal="Implement clean and synthesizable Systemverilog RTL conforming to industry standards.",
    backstory="You write pristine, industry-standard SystemVerilog. You avoid common synthesis traps,"
        "properly implement synchronous resets, pipeline data paths smoothly, and comment code meticulously "
        "so verification teams can easily read it.",
    llm=llm_architect, # Higher reasoning might be needed
    verbose=True,
    allow_delegation=False,
    memory=False
)

lead_verification = Agent(
    role="Principle Verification Engineer",
    goal="Analyze RTL designs and design rigorous formal verification plans.",
    backstory="You look at hardware designs through a destructive lens. You find the dark corners, the hidden mathematical "
        "overflows, and state-machine deadlocks. You create verification plans outlining all assumptions, "
        "assertions, and cover properties needed to achieve 100 percent formal proof boundedness.",
    llm=llm_architect,
    verbose=True,
    allow_delegation=False,
    memory=False
)

verif_engie = Agent(
    role="Senior Formal Verification Engineer",
    goal="Write rigorous SystemVerilog Assertions (SVA) to mathematically prove design correctness.",
    backstory=(
        "You are an expert in SVA and Formal Verification methodologies. You write concurrent assertions, "
        "clocking blocks, and complex safety/liveness properties. You turn verification plans into concrete "
        "mathematical constraints (assume) and checks (assert)."
    ),
    llm=llm_engineer,
    verbose=True,
    allow_delegation=False,
    memory=False
)

testing_agent = Agent(
    role="Junior Verification Engineer",
    goal="Inspect RTL and SVA designs and write one or more testbenches that implements their plans and intent.",
    backstory=(
        "You are a fresh and eager employee assigned to write testbenches."
        "You want to earn your place and make a name for yourself, so you rigorously analyze the RTL and Verification files to ensure you write quality testbenches."
    ),
    llm=llm_engineer,
    verbose=True,
    allow_delegation=False,
    memory=False
)

specification_task = Task(
    description=(
        "Generate a comprehensive microarchitectural and functional specification for an AI accelerator "
        "Multiply-Accumulate (MAC) sub-module that computes the operation: D = (A * B) + C."
        "The design must strictly adhere to the following hardware constraints:"
        "1. FORMAT: Inputs A and B are IEEE 754 FP16. Input C and Output D are IEEE 754 FP32 (Mixed-Precision)."
        "2. UNDERFLOW: Handle subnormal inputs via a 'Flush-to-Zero' (FTZ) approach."
        "3. ROUNDING: Implement Round-to-Nearest-Even (RNE) for normalization and truncation phases."
        "4. PROTOCOL: Implement a standard Valid/Ready handshake protocol for both inputs and outputs."
        "5. PIPELINE: Fully pipeline the module with a depth of 3 to 4 stages to maximize clock frequency."
        "6. EXCEPTIONS: Explicitly define how the design detects and flags Overflow, Underflow, and Invalid Operations (NaN)."
    ),
    expected_output="A comprehensive Markdown specification document detailing interface, pipeline stages, and mathematical edge cases.",
    agent=lead_architect,
    output_file="output/spec.md"
)

rtl_firstpass_task = Task(
    description=(
        "Read the specification document about an FP16 MAC module in output/spec.md. "
        "CRITICAL IMPLEMENTATION RULES:\n"
        "1. DO NOT use raw arithmetic operators ('*', '+') directly on the floating-point vectors.\n"
        "2. You MUST explicitly unpack the FP16/FP32 fields (sign, exponent, mantissa) into separate local logic signals.\n"
        "3. Implement explicit logic for: Mantissa multiplication (handling the implicit bit), exponent addition/subtraction with bias, and an alignment barrel-shifter for the C operand.\n"
        "4. Synchronize the input handshakes: a_ready, b_ready, and c_ready must assert simultaneously only when all three valids are high and the input stage is free.\n"
        "5. Properly pass data through the pipeline arrays sequentially (e.g., stage[i] <= stage[i-1])."
    ),
    agent=rtl_engie,
    expected_output="A synthesizable Systemverilog file (.sv) with comments explaining design choices and their reasoning.",
    output_file="output/fp16_mac.sv"
)

rtl_secondpass_task = Task(
    description=(
        "Begin by inspecting the file output/fp16_mac.sv. "
        "Act as a strict Silicon Sign-off Reviewer. Look for the following critical bugs:\n"
        "- Any instance where an output or internal signal is multi-driven by both an always_comb and always_ff block.\n"
        "- Any instance where floating-point inputs are directly multiplied or added like integers.\n"
        "If these bugs exist, rewrite the mathematical datapaths to perform true IEEE 754 steps: unpacking, subnormal flushing (FTZ), mantissa multiplication, exponent alignment, addition, Round-to-Nearest-Even (RNE) rounding, and repacking."
    ),
    agent=rtl_engie,
    expected_output="Pragmatic, cleanly structured, and commented SystemVerilog (.sv) code for the module.",
    output_file="output/fp16_mac.sv"
)

plan_verification_task = Task(
    description=(
        "Review the generated SystemVerilog design at output/fp16_mac.sv and its spec at output/spec.md. "
        "Identify critical corner cases (e.g., multiplying by zero, NaN propagation, extreme exponents, "
        "back-to-back pipeline stalls, reset during valid data). Write a formal verification plan tracking what needs to be verified."
    ),
    expected_output="A structured formal verification plan in Markdown mapping required assertions, assumptions, and cover points.",
    agent=lead_verification,
    output_file="output/verification_plan.md",
    context=[specification_task, rtl_secondpass_task]
)

verification_task = Task(
    description=(
        "Using the formal verification plan from output/verification_plan.md and the RTL from output/fp16_mac.sv, "
        "write a complete SystemVerilog assertion file or bind module."
        "Include properties to check:"
        "- Protocol compliance (valid/ready handshake stability properties)."
        "- Reset behavior (all registers cleared, outputs zeroed/invalid)."
        "- Flush-to-Zero verification."
        "- Overflow targeting positive/negative infinity bit-patterns."
    ),
    expected_output="A valid SystemVerilog file (.sv) containing concurrent SVAs, assumptions, and cover points wrapped in a checker or bind module.",
    agent=verif_engie,
    output_file="output/fp16_mac_sva.sv",
    context=[rtl_secondpass_task, plan_verification_task]
)

write_testbench_task = Task(
    description=(
        "Thoroughly inspect the following files: output/spec.md, output/fp16_mac.sv, output/verification_plan.md and output/fp16_mac_sva.sv. "
        "Write at least one testbench file that implements the verification plan and tests that the RTL correctly implements the behavior outlined in the specification."
    ),
    expected_output="A Systemverilog (.sv) file that compiles correctly and tests the RTL file against both the verification plan and specification document.",
    agent=testing_agent,
    output_file="output/testbench.sv",
    context=[specification_task, rtl_secondpass_task, plan_verification_task, verification_task]
)


if __name__ == "__main__":
    crew = Crew(
    agents=[lead_architect, rtl_engie, lead_verification, verif_engie, testing_agent],
    tasks=[specification_task, rtl_firstpass_task, rtl_secondpass_task, plan_verification_task, verification_task, write_testbench_task],
    verbose=True,
    process=Process.sequential,
    embedder=local_embedder,
    memory=False
    )

    os.makedirs("output", exist_ok=True)
    result = crew.kickoff()