# Autonomous Generation and Verification of an FP16 Multiply-Accumulate Model via a Team of AI Models

## Objectives
* Using the CrewAI library in Python, create and run a Mixture-of-Experts style workflow utilizing AI Models that will, after given a project, will generate a specification plan and both RTL and verification designs with appropriate plan documents for each.

* Explore the benefits and risks of "out-sourcing" the workflow to near or fully-autonomous AI Agents.

## Machine Hardware
The following hardware and software has been disclosed with the intent on tempering expectations on others wishing to run this project themselves.

* CPU -> Ryzen 9 7950x (16-cores, 32-threads)
* GPU -> 7900 XTX (24 GB VRAM)
* RAM -> 64 GB DDR5 @ 5600 MHz
* OS -> Linux Fedora (44) 7.0.12-cachyos1.fc44.x86_64
* Software -> Ollama (ROCm Backend), Python 3.11, CrewAI

With the usage of the 7900 XTX, a 22GB quantized model of Deepseek-R1:32b and an as-is Qwen2.5-coder:14b can comfortably run with Ollama handling hot-loading. With the project, the time elapsed from command execution to completion is roughly 3 minutes and 35 seconds.

## The Model

* ~~**Deepseek-R1:32B**. Although quantized to 22B, this model serves as an excellent method to take advantage of the 7900 XTX's 24 GB of VRAM, allowing the use of excellent reasoning ability.~~

* ~~**Qwen2.5:14B (Coder)**. This variant of the Qwen2.5 model is specialized for coding, giving much higher confidence in its ability to conform to expected syntax and standards with any language it's directed at.~~

    * Qwen3-30B (Coder): After some experimentation, this model contains an excellent blend of reasoning and coding ability due to its Mixture-of-Experts architecture.

* Nomic-embed-text: Certain pipelines/functionality appear hardcoded to use a GPT model. Nomic has been used to handle token passing between agents as a bypass.

## The (AI) Team

* Principle RTL Architect (lead_architect): Handles the drafting of specifications for a designated project.

* Senior Digital Design Engineer (rtl_engie): Responsible for reading the specification plan made by the Principle Architect and the subsequent RTL design in accordance with the specification.

* Principle Verification Engineer (lead_verification): Given the responsibility of inspecting the overall specification document and the RTL design to then create a verification plan. This agent has been designated to favor Formal Verification over traditional or UVM-based methods.

* Senior Formal Verification Engineer (verif_engie): Analyzes the verification plan and designs a formal verification file using Systemverilog Assertions (SVA).

## The Hardware Goal: Mixed-Precision MAC

The hardware module used as a test for this project will be an AI-Accelerator Multiply-Accumulate (MAC) that computes the floating-point formula:

D = (A x B) + C

### Constraints Implemented

1) Format & Precision: Inputs **A** and **B** are IEEE 754 Half-Precision Floating-Point (FP16). Accumulator Input **C** and Output **D** are IEEE 754 Single-Precision Floating-Point (FP32). Using this approach mimics industry-standard design of tensor core blocks that limit truncation error during accumulation.

2) Underflow Mitigation: Subnormal inputs are handled via a strict Flush-to-Zero (FTZ) technique.

3) Rounding Mode: Implements Round-to-Nearest-Even (RNE) for internal normalization and truncation phases.

4) Interface Protocol: Flow control is driven by a standard synchronous Valid/Ready handshake mechanism supporting full downstream backpressure on both inputs and outputs.

5) Pipelining: Fully pipelined structure across a depth of 3 to 4 stages to increase maximum clock frequency.

6) Exception Logic: Explicit detection, tagging and registration of IEEE 754 exceptions including Overflow, Underflow and Invalid Operation (NaN).

# Setup and Local Execution

## Required
* Python 3.11 (CrewAI does not support 3.14)

* Ollama backend engine

* **NOTE**: At bare minimum, if you do not possess 24GB of VRAM, this project will have significantly different results.

## Handling Context

For efficiency purposes on a 24GB 7900 XTX graphics card, the context has been expanded to 16384. On Linux, do the following steps to increase the context window:

```
ollama pull qwen3-coder:30b
ollama pull nomic-embed-text

ollama run qwen3-coder:30b
>>> /set parameter num_ctx 16384
>>> /save qwen3-coder:30b-16k
>>> /bye
```

## Software Setup

It is recommended to setup a virtual Python environment to keep things contained and isolated:

```
sudo dnf install python3.11
python3.11 -m venv .venv
```

Next, to get the library and dependencies:

```
pip install uv
uv pip install crewai crewai-tools
```

# TODO

* **DONE (6/16)** Manually check the design with Verilator and see how accurate the AI models were.
    * **(CHANGED 6/16)** ~~Lots of major design flaws, both implementation and architecturally.~~ **(DONE 6/17)** ~~Moving towards a single model.~~

* Give the AI Agents the IEEE 754 Standard. A lot of the bugs seem to revolve around misunderstanding floating-point arithmetic.

* Find and install an open-source Formal Verification tool to manually check the AI's generated SVA code.

* Refine and modify each agent and task as needed

* Figure out how to spin up an MCP to enhance team capability

# Errors Encountered

1) When running Verilator on fp16_mac.sv, there are already numerous errors.
    * Incorrect ``` usage.
    * The RTL agent mixed up naming conventions. The Ready_in signal was made an input when it should be an output, and vice-versa with Ready_out signal.
    * Despite specifying the agents to use Systemverilog, they still wrote the file as traditional Verilog, which could cause more subtle bugs later on when synthesizing.
    * Verification Engineer agent appeared to have written incorrect range specifiers for a property.
    * Verification Agent did not match the file name to the module name.
    * Verification agent completely forgot to define the stages in the sva file.
    * Verification agent incorrectly placed the bind method-call inside the module.
    * Verification agent made a huge architecture flaw in that the SVA model drives outputs, causing a situation where each model checks itself with no meaningful results.
    * After fixing all syntax and immediate architecture problems, assertion on Line 155 in the SVA file was triggered, implying reset doesn't behave correctly.

2) Even swapping to Qwen3-coder:30b didn't yield much improvement. On the surface the Systemverilog code looks fine, but a deeper dive reveals it's still riddled with bugs everywhere.

# Experiments
* Swapped Qwen2.5-coder:14b for Qwen2.5-coder:32b. Already seeing promising results in that the code written is actually Systemverilog and not just Verilog with a .sv extension. However conversational responses are starting to appear in files. In the long run, this just a minor nuisance requiring vigilance.

    * Using Qwen3-coder:30b instead seems much more promising. Actual Systemverilog, blocking statements aren't used within always_ff blocks on first-go. The junior engineer forgot to close off the testbench module and a couple tasks. (Is it emulating a junior employee too well?) A deeper dive behind the model reveals it utilizes an MoE architecture, resulting in much faster inference at less VRAM cost.

* Expanding the context window within the script itself and see how much lack of context is affecting generation and thinking. Going to use 32k instead of 16k.

    * Didn't seem to change much.

* Making another "team" that's all about nailing down the floating-point arithmetic. Perhaps the core issue is doing things too monolithic-like.

    * Experimented with Autogen, a library from Microsoft. Doesn't seem to need so much customization and bypassing as CrewAI which is promising. However it's becoming clear that, even when provided the IEEE-754 standard, the AI agents are completely incapable of creating functional floating-point arithmetic code. This might be an area where a human mind is still required.

* Experimented with expanding context window to 32k. Not seeing much difference if I'm being honest.

* The models seem to be quantized which impacts their precision which then affects their ability to reason about floating point arithmetic. I think I'm hitting the limit with open-source models, at least for this application, so let's see how Claude Code does.