import os
from pathlib import Path
from crewai import Crew, LLM, Process, Agent, Task
from crewai.tools import tool

os.environ["OPENAI_MODEL_NAME"] = "ollama/qwen3.6:27b" # Ensure we don't use GPT to enforce data security
os.environ["OPENAI_API_KEY"] = "ollama"
os.environ["OPENAI_BASE_URL"] = "http://localhost:11434"
os.environ["OPENAI_API_BASE"] = "http://localhost:11434"

engineer_psyche = "ollama/qwen3.6:27b" # Very promising. Going to stick with this and see where it takes the project

llm_architect = LLM(model=engineer_psyche, base_url="http://localhost:11434", temperature=0.4)
llm_engineer = LLM(model=engineer_psyche, base_url="http://localhost:11434", temperature=0.2)

local_embedder = { # Again, enforce data security
    "provider": "ollama",
    "config": {
        "model": "nomic-embed-text",
        "base_url": "http://localhost:11434"
    }
}

# ---------------------------------------------------------------------------
# IEEE 754 RAG Tool — grounded standard lookup to prevent floating-point bugs
# ---------------------------------------------------------------------------
IEEE_PDF_PATH = Path(__file__).parent / "resources" / "IEEE_Standard_754-2019.pdf"

@tool("search_ieee_754_standard")
def search_ieee_754_standard(query: str) -> str:
    """Search the IEEE 754-2019 standard document for relevant clauses.
    Use this tool when you need authoritative information about floating-point
    formats, rounding modes, special values (NaN, Inf), or exception handling.
    Pass a descriptive query string (e.g. 'round to nearest even', 'subnormal numbers').
    """
    if not IEEE_PDF_PATH.exists():
        return f"IEEE 754 PDF not found at {IEEE_PDF_PATH}. Rely on training knowledge."
    try:
        import pypdf
        reader = pypdf.PdfReader(str(IEEE_PDF_PATH))
        keywords = [k.lower() for k in query.split()]
        results = []
        for page_num, page in enumerate(reader.pages):
            text = page.extract_text() or ""
            if any(kw in text.lower() for kw in keywords):
                results.append(f"--- Page {page_num + 1} ---\n{text[:1500]}")
            if len(results) >= 4:
                break
        return "\n\n".join(results) if results else "No matching pages found for query."
    except Exception as e:
        return f"Error reading IEEE 754 PDF: {e}"


lead_architect = Agent(
    role="Principle RTL Architect",
    goal="Define specifications for a high-performance FP16 Multiply-Accumulate (MAC) unit.",
    backstory="You are a seasoned silicon architect specializing in arithmetic compute blocks for AI accelerators."
        "You understand IEEE 754 half-precision limitations, rounding modes (round-to-nearest-even),"
        "overflow/underflow exception handling, and pipelining structures for maximum frequency.",
    llm=llm_architect,
    verbose=True,
    allow_delegation=False,
    memory=False, # Hardcoded to use a GPT model when enabled
)

rtl_engineer = Agent(
    role="Senior Digital Design Engineer",
    goal="Implement clean and synthesizable SystemVerilog RTL conforming to industry standards.",
    backstory="You write pristine, industry-standard SystemVerilog. You avoid common synthesis traps,"
        "properly implement synchronous resets, pipeline data paths smoothly, and comment code meticulously "
        "so verification teams can easily read it.",
    tools=[search_ieee_754_standard],
    llm=llm_architect,
    verbose=True,
    allow_delegation=False,
    memory=False,
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
    memory=False,
)

verif_engineer = Agent(
    role="Senior Formal Verification Engineer",
    goal="Write rigorous SystemVerilog Assertions (SVA) to mathematically prove design correctness.",
    backstory=(
        "You are an expert in SVA and Formal Verification methodologies. You write concurrent assertions, "
        "clocking blocks, and complex safety/liveness properties. You turn verification plans into concrete "
        "mathematical constraints (assume) and checks (assert)."
    ),
    tools=[search_ieee_754_standard],
    llm=llm_engineer,
    verbose=True,
    allow_delegation=False,
    memory=False,
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
    memory=False,
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
        "Use the search_ieee_754_standard tool to look up: FP16 format fields, bias constants, "
        "subnormal handling, and RNE rounding rules BEFORE writing any code.\n"
        "CRITICAL IMPLEMENTATION RULES:\n"
        "1. DO NOT use raw arithmetic operators ('*', '+') directly on the floating-point vectors.\n"
        "2. You MUST explicitly unpack the FP16/FP32 fields (sign, exponent, mantissa) into separate local logic signals.\n"
        "   FP16: 1 sign bit [15], 5 exponent bits [14:10], 10 mantissa bits [9:0], bias=15.\n"
        "   FP32: 1 sign bit [31], 8 exponent bits [30:23], 23 mantissa bits [22:0], bias=127.\n"
        "3. Implement explicit logic for: 11x11 mantissa multiplication (10 mantissa bits + 1 implicit bit = 11 bits each), "
        "   exponent addition with bias removal (exp_product = exp_a + exp_b - 15), "
        "   and an alignment barrel-shifter for the C operand.\n"
        "4. Synchronize the input handshake: ready_o must deassert when the pipeline stage 0 is occupied.\n"
        "5. Propagate C (FP32) through ALL pipeline stages so it is available at the addition stage.\n"
        "6. Properly pass data through the pipeline arrays sequentially (e.g., stage[i] <= stage[i-1])."
    ),
    agent=rtl_engineer,
    expected_output="A synthesizable SystemVerilog file (.sv) with comments explaining design choices and their reasoning.",
    output_file="output/fp16_mac_v1.sv"
)

rtl_secondpass_task = Task(
    description=(
        "Begin by inspecting the file output/fp16_mac_v1.sv. "
        "Act as a strict Silicon Sign-off Reviewer. Use search_ieee_754_standard for any IEEE 754 rules you are unsure about. "
        "Look for and fix the following critical bugs:\n"
        "- Any instance where an output or internal signal is multi-driven by both an always_comb and always_ff block.\n"
        "- Any instance where floating-point inputs are directly multiplied or added like integers.\n"
        "- Wrong bit-width extraction: FP16 exponent must be [14:10] (5 bits), mantissa [9:0] (10 bits).\n"
        "- Multiplication must be 11×11 bits (with implicit leading 1), NOT wider.\n"
        "- C (FP32 accumulator) must flow through every pipeline stage register.\n"
        "- ready_o must be driven from a single combinational source (not feedback of valid_pipeline).\n"
        "If these bugs exist, rewrite the mathematical datapaths to perform true IEEE 754 steps: "
        "unpacking, subnormal flushing (FTZ), 11x11 mantissa multiplication, exponent debiasing, "
        "barrel-shift alignment of C mantissa, signed mantissa addition, Leading Zero Count normalization, "
        "Round-to-Nearest-Even (RNE) rounding with guard/round/sticky bits, and repacking."
    ),
    agent=rtl_engineer,
    expected_output="Pragmatic, cleanly structured, and commented SystemVerilog (.sv) code for the module.",
    output_file="output/fp16_mac.sv"
)

rtl_integration_task = Task(
    description=(
        "Read the files output/fp16_mac.sv, output/fp_unpacker.sv, and output/fp_rounder.sv.\n"
        "The fp_unpacker and fp_rounder modules are pre-verified parameterized submodules:\n"
        "  - fp_unpacker #(.EXP_WIDTH(E), .MANT_WIDTH(M)) unpacks a packed FP value into sign/exponent/mantissa.\n"
        "  - fp_rounder  #(.MAN_WIDTH(M), .GUARD_WIDTH(3)) performs IEEE 754 RNE rounding.\n"
        "Your task: refactor fp16_mac.sv to INSTANTIATE these submodules instead of reimplementing their logic inline.\n"
        "Specifically:\n"
        "  1. Instantiate fp_unpacker twice for A (EXP_WIDTH=5, MANT_WIDTH=10) and B (EXP_WIDTH=5, MANT_WIDTH=10) in Stage 0.\n"
        "  2. Instantiate fp_unpacker once for C (EXP_WIDTH=8, MANT_WIDTH=23) in Stage 0.\n"
        "  3. Instantiate fp_rounder (MAN_WIDTH=23, GUARD_WIDTH=3) in Stage 3 for final rounding.\n"
        "Remove any duplicated unpacking or rounding logic from fp16_mac.sv after instantiation.\n"
        "Ensure the top-level file list compiles: fp_unpacker.sv, fp_rounder.sv, fp16_mac.sv."
    ),
    agent=rtl_engineer,
    expected_output="Updated fp16_mac.sv that instantiates fp_unpacker and fp_rounder submodules cleanly.",
    output_file="output/fp16_mac.sv",
    context=[rtl_secondpass_task]
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
    context=[specification_task, rtl_integration_task]
)

verification_task = Task(
    description=(
        "Using the formal verification plan from output/verification_plan.md and the RTL from output/fp16_mac.sv, "
        "write a complete SystemVerilog assertion file or bind module. "
        "Use search_ieee_754_standard to verify corner case rules (e.g., Inf×0 = NaN, NaN propagation).\n"
        "CRITICAL ASSERTION RULES:\n"
        "1. The pipeline has exactly 4 stages. Use ##4 or $past(signal, 4) for latency checks — NOT 3.\n"
        "2. ready_o must be: assign ready_o = !valid_pipeline[0]. Assume this behavior.\n"
        "3. FP16 subnormal detection: (a_i[14:10] == 5'b0 && a_i[9:0] != 10'b0), not a hardcoded value.\n"
        "4. Reset assertions: do NOT use 'disable iff (!rst_n)' on a property that fires during reset — restructure.\n"
        "Include properties to check:"
        "- Protocol compliance (valid/ready handshake stability properties)."
        "- Reset behavior (all registers cleared, outputs zeroed/invalid)."
        "- Flush-to-Zero verification."
        "- Overflow targeting positive/negative infinity bit-patterns."
    ),
    expected_output="A valid SystemVerilog file (.sv) containing concurrent SVAs, assumptions, and cover points wrapped in a checker or bind module.",
    agent=verif_engineer,
    output_file="output/fp16_mac_sva.sv",
    context=[rtl_integration_task, plan_verification_task]
)

write_testbench_task = Task(
    description=(
        "Thoroughly inspect the following files: output/spec.md, output/fp16_mac.sv, output/verification_plan.md and output/fp16_mac_sva.sv. "
        "Write at least one testbench file that implements the verification plan and tests that the RTL correctly implements the behavior outlined in the specification.\n"
        "CRITICAL TESTBENCH RULES:\n"
        "1. Use the EXACT port names from fp16_mac.sv — do NOT invent new names (e.g., A/B/C not a_i/b_i/c_i).\n"
        "2. The pipeline has 4-cycle latency. After asserting valid_i, wait 4 clock cycles before sampling d_o.\n"
        "3. Verify each expected output using real FP arithmetic: 1.0×2.0+3.0=5.0 is FP32 32'h40A00000.\n"
        "4. Apply actual non-zero stimulus — every test must drive input signals.\n"
        "5. Apply reset (rst_n=0) for at least 5 cycles, then deassert before starting tests."
    ),
    expected_output="A SystemVerilog (.sv) file that compiles correctly and tests the RTL file against both the verification plan and specification document.",
    agent=testing_agent,
    output_file="output/testbench.sv",
    context=[specification_task, rtl_integration_task, plan_verification_task, verification_task]
)


if __name__ == "__main__":
    crew = Crew(
        agents=[lead_architect, rtl_engineer, lead_verification, verif_engineer, testing_agent],
        tasks=[
            specification_task,
            rtl_firstpass_task,
            rtl_secondpass_task,
            rtl_integration_task,
            plan_verification_task,
            verification_task,
            write_testbench_task,
        ],
        verbose=True,
        process=Process.sequential,
        embedder=local_embedder,
        memory=False,
    )

    os.makedirs("output", exist_ok=True)
    try:
        result = crew.kickoff()
        print("\n=== Crew finished successfully ===")
        print(result)
    except Exception as e:
        print(f"\n=== Crew failed: {e} ===")
        raise
