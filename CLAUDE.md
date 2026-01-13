# LLVM Backend - IMCFlow ISA

This is the LLVM compiler backend extended to support the imcflow ISA (inode and imce instructions).

## Agent Usage

**ALWAYS use the `imcflow-compiler-expert` agent when working in this directory.**

This agent has deep expertise in:
- LLVM backend development for custom ISAs
- inode/imce instruction encoding and selection
- TableGen definitions and instruction patterns
- LLVM IR lowering and optimization passes
- Integration with TVM codegen output

## Key Areas

- **Target Definition**: TableGen files defining imcflow ISA
- **Instruction Selection**: Patterns for mapping LLVM IR to inode/imce instructions
- **Code Generation**: Assembly emission and object file generation
- **Backend Passes**: Custom optimization and lowering passes
- **ABI/Calling Convention**: Function call handling and register allocation

## LLVM Components

- **Target Description**: `.td` files with instruction definitions
- **Backend Classes**: C++ implementation of target-specific logic
- **Test Cases**: LLVM IR and assembly tests for validation

## When to Use the Agent

Use `imcflow-compiler-expert` for ANY task including:
- Adding new ISA instructions
- Debugging instruction selection failures
- Analyzing generated assembly code
- Modifying TableGen patterns
- Understanding IR lowering transformations
- Integration issues with TVM or gem5
- Compiler crashes or assertion failures
