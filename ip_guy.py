import os
import asyncio
from autogen_ext.models.ollama import OllamaChatCompletionClient
from autogen_agentchat.agents import AssistantAgent

# 1. Setup a clean, native local Ollama client
ollama_client = OllamaChatCompletionClient(
    model="qwen3-coder:30b-32k",
    base_url="http://localhost:11434",
    model_info={
        "vision": False,
        "function_calling": True,
        "json_output": True,
        "structured_output": True,
        "family": "unknown"
    }
)

# 2. Define a pure Python RAG tool (immune to framework-specific Pydantic crashes)
def search_ieee_754_standard(query: str) -> str:
    """
    Searches the local IEEE_Standard_754-2019.pdf file for floating-point specifications.
    Use this to look up sign layouts, exponent biases, and rounding mechanics (like RNE).
    """
    pdf_path = 'resources/IEEE_Standard_754-2019.pdf'
    if not os.path.exists(pdf_path):
        return f"Error: PDF standard not found at {pdf_path}"
        
    try:
        import pypdf
        reader = pypdf.PdfReader(pdf_path)
        matched_chunks = []
        
        # Simple and highly predictable scan strategy
        for i, page in enumerate(reader.pages):
            text = page.extract_text() or ""
            if any(word.lower() in text.lower() for word in query.split()):
                matched_chunks.append(f"--- Page {i+1} ---\n{text[:1500]}...")
                if len(matched_chunks) >= 4:  # Protect local context window size
                    break
                    
        return "\n\n".join(matched_chunks) if matched_chunks else "No direct matches found in the standard."
    except Exception as e:
        return f"Error reading PDF standard: {str(e)}"


# 3. Create your silicon IP engineer agent
ip_engineer = AssistantAgent(
    name="ip_engineer",
    model_client=ollama_client,
    tools=[search_ieee_754_standard],
    system_message=(
        "You are an expert digital design engineer. Use the IEEE 754 standard tool "
        "to look up specifications, then write clean, production-ready SystemVerilog code."
    ),
    reflect_on_tool_use=True,   # CRUCIAL: Forces the model to read the tool output and generate a text response
    max_tool_iterations=5       # Allows the agent to search multiple times if needed before finalizing
)


async def main():
    # Make sure output directory exists
    os.makedirs("output", exist_ok=True)

    # Execute Task 1: FP Unpacker
    print("--- Generating fp_unpacker.sv ---")
    unpack_result = await ip_engineer.run(
        task=(
            "Use the IEEE 754 standard tool to review the structural layouts of floating-point numbers. "
            "Write a parameterized SystemVerilog module named 'fp_unpacker.sv' that accepts parameters "
            "EXP_WIDTH and MANT_WIDTH. The module must output the sign, the unbiased exponent, and the mantissa "
            "(with the implicit leading bit handled correctly based on normal vs subnormal state)."
        )
    )
    
    with open("output/fp_unpacker.sv", "w") as f:
        f.write(unpack_result.messages[-1].content)
    print("Saved fp_unpacker.sv to output directory.\n")

    # Execute Task 2: FP Rounder
    print("--- Generating fp_rounder.sv ---")
    round_result = await ip_engineer.run(
        task=(
            "Search the IEEE 754 standard document for 'Round to Nearest, Ties to Even' (RNE) implementation rules. "
            "Write a parameterized SystemVerilog module named 'fp_rounder.sv' that takes an expanded mantissa "
            "along with Guard, Round, and Sticky bits, and outputs the rounded mantissa and an exponent increment signal."
        )
    )
    
    with open("output/fp_rounder.sv", "w") as f:
        f.write(round_result.messages[-1].content)
    print("Saved fp_rounder.sv to output directory.")


if __name__ == "__main__":
    # Ensure you have 'pip install autogen-ext[ollama] pypdf' installed in your venv
    asyncio.run(main())