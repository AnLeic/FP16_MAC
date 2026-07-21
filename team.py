import os
import re
import subprocess
from pathlib import Path
from crewai import Crew, LLM, Process, Agent, Task
from crewai.tasks.task_output import TaskOutput
from crewai.tools import tool

os.environ["OPENAI_MODEL_NAME"] = "ollama/qwen3.6:27b" # Ensure we don't use GPT to enforce data security
os.environ["OPENAI_API_KEY"] = "ollama"
os.environ["OPENAI_BASE_URL"] = "http://localhost:11434"
os.environ["OPENAI_API_BASE"] = "http://localhost:11434"

engineer_psyche = "ollama/qwen3.6:27b" # Very promising. Going to stick with this and see where it takes the project

# CrewAI's LLM(model="ollama/...") routes through its OpenAI-compatible client
# (crewai.llms.providers.openai_compatible), whose default timeout=None falls
# back to the underlying openai SDK's own default (600s). Ollama itself has
# no request/generation timeout of any kind to configure (its env vars only
# cover model load/keep-alive, not in-flight generation -- confirmed via
# `ollama serve --help`), so the ~4min cutoff being observed isn't coming
# from Ollama; it has to be enforced somewhere on the client side. Setting an
# explicit, generous value here is the one lever under this script's direct
# control, and 1800s (30 min) gives real headroom for a 27B local model
# generating a multi-hundred-line SV file.
LLM_TIMEOUT_SECONDS = 1800

llm_architect = LLM(model=engineer_psyche, base_url="http://localhost:11434", temperature=0.4, timeout=LLM_TIMEOUT_SECONDS)
llm_engineer = LLM(model=engineer_psyche, base_url="http://localhost:11434", temperature=0.2, timeout=LLM_TIMEOUT_SECONDS)

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


# ---------------------------------------------------------------------------
# File-read tool — lets agents actually see the contents of pre-existing
# output/ files (e.g. the fp_unpacker.sv / fp_rounder.sv submodules produced
# by ip_guy.py) instead of being told to "read" a file with no way to do so.
# ---------------------------------------------------------------------------
OUTPUT_DIR = Path(__file__).parent / "output"


@tool("read_output_file")
def read_output_file(filename: str) -> str:
    """Read and return the full contents of a file in the project's output/
    directory (e.g. 'fp_unpacker.sv', 'fp_rounder.sv', 'fp16_mac.sv').
    Pass just the filename, not a path. Use this whenever you need to see the
    EXACT contents (port names, parameter names, signal widths) of a file
    that has already been generated, rather than guessing.
    """
    safe_name = Path(filename).name  # prevent path traversal outside output/
    target = OUTPUT_DIR / safe_name
    if not target.exists():
        return f"File '{safe_name}' does not exist in output/ yet."
    return target.read_text()


# ---------------------------------------------------------------------------
# Verilator-lint guardrail — validates every generated SystemVerilog file by
# actually compiling it before the crew is allowed to move on, feeding
# compile errors back to the agent for a retry instead of silently accepting
# broken RTL. This also strips markdown code fences (LLMs reliably wrap code
# responses in ``` fences even when asked for a raw file) before linting and
# before the corrected content is persisted, since a fenced file fails to
# compile on the fence syntax alone.
# ---------------------------------------------------------------------------
VERILATOR_BIN = Path(__file__).parent / "venv" / "lib" / "python3.11" / "site-packages" / "verilator" / "bin" / "verilator"
if not VERILATOR_BIN.exists():
    VERILATOR_BIN = Path("verilator")  # fall back to PATH


def _strip_code_fences(text: str) -> str:
    """Strip a single leading/trailing markdown code fence, if present."""
    stripped = text.strip()
    if stripped.startswith("```"):
        stripped = re.sub(r"^```[a-zA-Z]*\n?", "", stripped)
        stripped = re.sub(r"\n?```\s*$", "", stripped)
    return stripped.strip() + "\n"


def _run_verilator_lint(candidate: Path, sibling_files: list[Path]) -> tuple[bool, str]:
    """Lint candidate (plus any sibling .sv files it depends on) with
    Verilator. Returns (ok, message). Fails on hard errors and on
    PINMISSING/PINCONNECTEMPTY warnings, which Verilator otherwise treats as
    non-fatal but which are exactly the signature of a submodule port-name
    mismatch (the class of bug this pipeline previously produced silently).
    """
    files = [str(candidate)] + [str(f) for f in sibling_files if f.exists()]
    cmd = [str(VERILATOR_BIN), "--lint-only", "--timing", "-Wall", "-Wno-fatal", "-Wno-TIMESCALEMOD"] + files
    try:
        proc = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
    except FileNotFoundError:
        return True, "(verilator binary not found; skipping lint check)"
    except subprocess.TimeoutExpired:
        return False, "Verilator lint timed out after 60s (possible infinite loop in generated code)."

    output = proc.stdout + proc.stderr
    fatal_markers = ("%Error", "%Warning-PINMISSING", "%Warning-PINCONNECTEMPTY")
    fatal_lines = [ln for ln in output.splitlines() if any(m in ln for m in fatal_markers)]
    if fatal_lines:
        # Keep the retry prompt short -- this pipeline runs on a local model
        # with a small context window, so a multi-KB error dump is more
        # likely to get truncated/ignored than a short, focused one.
        excerpt = "\n".join(fatal_lines[:20])
        return False, f"Verilator lint failed:\n{excerpt}"
    return True, "Verilator lint passed."


def make_sv_guardrail(output_filename: str, sibling_filenames: list[str] | None = None):
    """Build a guardrail: strip fences, lint, and on success write the
    corrected file to output/<output_filename> directly (bypassing CrewAI's
    own output_file save, which otherwise writes the pre-guardrail raw text).
    On failure, return the lint errors so CrewAI retries the task with that
    feedback appended to the agent's context.
    """
    output_path = OUTPUT_DIR / output_filename
    sibling_paths = [OUTPUT_DIR / f for f in (sibling_filenames or [])]

    def guardrail(task_output: TaskOutput) -> tuple[bool, str]:
        cleaned = _strip_code_fences(task_output.raw)
        candidate_path = output_path.with_suffix(output_path.suffix + ".candidate")
        candidate_path.write_text(cleaned)
        try:
            ok, message = _run_verilator_lint(candidate_path, sibling_paths)
            if not ok:
                return False, message
            OUTPUT_DIR.mkdir(exist_ok=True)
            candidate_path.replace(output_path)
            return True, cleaned
        finally:
            candidate_path.unlink(missing_ok=True)

    return guardrail


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
    tools=[search_ieee_754_standard, read_output_file],
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
    tools=[search_ieee_754_standard, read_output_file],
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
    tools=[read_output_file],
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
    expected_output="A synthesizable SystemVerilog file (.sv) with comments explaining design choices and their reasoning. "
        "Output ONLY raw SystemVerilog source -- no markdown code fences, no prose before or after the code.",
    guardrail=make_sv_guardrail("fp16_mac_v1.sv"),
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
    expected_output="Pragmatic, cleanly structured, and commented SystemVerilog (.sv) code for the module. "
        "Output ONLY raw SystemVerilog source -- no markdown code fences, no prose before or after the code.",
    guardrail=make_sv_guardrail("fp16_mac.sv"),
)

def _read_submodule_or_warn(filename: str) -> str:
    path = OUTPUT_DIR / filename
    if not path.exists():
        print(f"[team.py] WARNING: {path} does not exist -- run ip_guy.py first to "
              f"generate it. rtl_integration_task will not be able to reuse it.")
        return f"(NOT FOUND: {filename} does not exist yet in output/. Do not invent " \
               f"a module by this name -- report this as a blocker instead of guessing its interface.)"
    return path.read_text()


# Inlined verbatim so the RTL engineer sees the ACTUAL port names/parameters
# of these submodules rather than being told to "read" files it has no tool
# access to -- that gap was the direct cause of fp16_mac.sv previously
# instantiating fp_unpacker/fp_rounder with invented, mismatched port lists.
_FP_UNPACKER_SRC = _read_submodule_or_warn("fp_unpacker.sv")
_FP_ROUNDER_SRC = _read_submodule_or_warn("fp_rounder.sv")

rtl_integration_task = Task(
    description=(
        "The fp_unpacker and fp_rounder modules below are pre-verified parameterized submodules "
        "that already exist in output/ (produced separately and reviewed). Refactor fp16_mac.sv to "
        "INSTANTIATE these submodules -- using their EXACT port names and parameter names as shown "
        "below, verbatim, character for character -- instead of reimplementing their logic inline. "
        "Do NOT rename, reorder, or guess at any port. If a port name below differs from what you "
        "would have named it, use the name below.\n\n"
        "=== output/fp_unpacker.sv (verbatim) ===\n"
        f"{_FP_UNPACKER_SRC}\n"
        "=== output/fp_rounder.sv (verbatim) ===\n"
        f"{_FP_ROUNDER_SRC}\n\n"
        "Specifically:\n"
        "  1. Instantiate fp_unpacker twice for A and B (EXP_WIDTH=5, MANT_WIDTH=10) in Stage 0.\n"
        "  2. Instantiate fp_unpacker once for C (EXP_WIDTH=8, MANT_WIDTH=23) in Stage 0.\n"
        "  3. Instantiate fp_rounder (MAN_WIDTH=23, GUARD_WIDTH=3) in Stage 3 for final rounding, using "
        "     the exact width convention shown in the fp_rounder source above for its input port.\n"
        "Remove any duplicated unpacking or rounding logic from fp16_mac.sv after instantiation.\n"
        "The read_output_file tool is also available if you need to re-check fp16_mac.sv's current content."
    ),
    agent=rtl_engineer,
    expected_output="Updated fp16_mac.sv that instantiates fp_unpacker and fp_rounder submodules cleanly, "
        "using their real port names. Output ONLY raw SystemVerilog source -- no markdown code fences.",
    guardrail=make_sv_guardrail("fp16_mac.sv", ["fp_unpacker.sv", "fp_rounder.sv"]),
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
    expected_output="A valid SystemVerilog file (.sv) containing concurrent SVAs, assumptions, and cover points "
        "wrapped in a checker or bind module. The checker module MUST declare an explicit port for every DUT "
        "signal it references (clk, rst_n, a_i, b_i, c_i, valid_i, ready_o, d_o, valid_o, ready_i, overflow_o, "
        "underflow_o, invalid_o, etc.) and the bind statement MUST connect every one of those ports by name "
        "(e.g. bind fp16_mac fp16_mac_sva chk(.clk(clk), .rst_n(rst_n), ...)) -- a checker module with no port "
        "list and an empty bind connection list is never connected to the DUT and will never fire. "
        "Output ONLY raw SystemVerilog source -- no markdown code fences.",
    agent=verif_engineer,
    guardrail=make_sv_guardrail("fp16_mac_sva.sv", ["fp16_mac.sv", "fp_unpacker.sv", "fp_rounder.sv"]),
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
    expected_output="A SystemVerilog (.sv) file that compiles correctly and tests the RTL file against both the "
        "verification plan and specification document. Do NOT declare your own copies of fp_unpacker or "
        "fp_rounder -- those modules already exist in output/ and will be compiled alongside this testbench. "
        "Output ONLY raw SystemVerilog source -- no markdown code fences.",
    agent=testing_agent,
    guardrail=make_sv_guardrail("testbench.sv", ["fp16_mac.sv", "fp_unpacker.sv", "fp_rounder.sv"]),
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
