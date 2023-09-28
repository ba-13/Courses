#include "common.h"

std::vector<UINT32> instruction_sizes(20, 0);
std::vector<UINT32> instruction_num_op(10, 0);
std::vector<UINT32> instruction_num_read_op(10, 0);
std::vector<UINT32> instruction_num_write_op(10, 0);
std::vector<UINT32> instruction_num_mem_op(5, 0);
std::vector<UINT32> instruction_num_rmem_op(5, 0);
std::vector<UINT32> instruction_num_wmem_op(5, 0);
UINT64 total_bytes_accessed = 0;
UINT32 max_bytes_accessed = 0;
UINT32 num_mem_instructions = 0;
INT32 max_imm_field = INT32_MIN;
INT32 min_imm_field = INT32_MAX;
INT32 max_disp_op = INT32_MIN;
INT32 min_disp_op = INT32_MAX;
UINT32 num_imm_val = 0;

KNOB<std::string> KnobOutputFile(KNOB_MODE_OVERWRITE, "pintool", "o", "partD.out", "specify file name for partC.so output");
KNOB<UINT64> KnobFastForwardTill(KNOB_MODE_OVERWRITE, "pintool", "f", "0", "fast forward by this many instructions");

INT32 Usage()
{
    PIN_ERROR("This pintool implements data-collection for ia32 ISA\n" + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}

void print_results()
{
    *out << "Instruction Size:\n";
    for (unsigned int i = 0; i < instruction_sizes.size(); i++)
    {
        *out << i << " " << instruction_sizes[i] << std::endl;
    }
    *out << "Number of Operands\n";
    for (unsigned int i = 0; i < instruction_num_op.size(); i++)
    {
        *out << i << " " << instruction_num_op[i] << std::endl;
    }
    *out << "Number of Register Read Operands\n";
    for (unsigned int i = 0; i < instruction_num_read_op.size(); i++)
    {
        *out << i << " " << instruction_num_read_op[i] << std::endl;
    }
    *out << "Number of Register Write Operands\n";
    for (unsigned int i = 0; i < instruction_num_write_op.size(); i++)
    {
        *out << i << " " << instruction_num_write_op[i] << std::endl;
    }
    *out << "Number of Memory Operands\n";
    for (unsigned int i = 0; i < instruction_num_mem_op.size(); i++)
    {
        *out << i << " " << instruction_num_mem_op[i] << std::endl;
    }
    *out << "Number of Memory Read Operands\n";
    for (unsigned int i = 0; i < instruction_num_rmem_op.size(); i++)
    {
        *out << i << " " << instruction_num_rmem_op[i] << std::endl;
    }
    *out << "Number of Memory Write Operands\n";
    for (unsigned int i = 0; i < instruction_num_wmem_op.size(); i++)
    {
        *out << i << " " << instruction_num_wmem_op[i] << std::endl;
    }
    *out << "Maximum memory bytes touched by one instruction " << max_bytes_accessed << std::endl;
    *out << "Average memory bytes touched by one instruction " << total_bytes_accessed << "/" << num_mem_instructions << std::endl;

    *out << "Max Imm " << max_imm_field << std::endl;
    *out << "Min Imm " << min_imm_field << std::endl;

    *out << "Max Disp " << max_disp_op << std::endl;
    *out << "Min Disp " << min_disp_op << std::endl;

    *out << "Total Imm Sum " << num_imm_val << std::endl;
}

VOID Fini(INT32 code, void *v)
{
    // insert end results
    print_results();
    return;
}

void MyExitRoutine()
{
    print_results();
    exit(0);
}

VOID track_instruction_part1To4(UINT32 size, UINT32 num_op, UINT32 num_read_op, UINT32 num_write_op)
{
    instruction_sizes[size]++;
    instruction_num_op[num_op]++;
    instruction_num_read_op[num_read_op]++;
    instruction_num_write_op[num_write_op]++;
}

VOID track_memory_operands_details(UINT32 num_mem_op, UINT32 num_rmem_op, UINT32 num_wmem_op, INT32 min_disp, INT32 max_disp, UINT32 size)
{
    instruction_num_mem_op[num_mem_op]++;
    instruction_num_rmem_op[num_rmem_op]++;
    instruction_num_wmem_op[num_wmem_op]++;
    min_disp_op = std::min(min_disp_op, min_disp);
    max_disp_op = std::max(max_disp_op, max_disp);
    total_bytes_accessed += size;
    max_bytes_accessed = std::max(max_bytes_accessed, size);
}

VOID track_memory_instructions()
{
    num_mem_instructions++;
}

VOID track_imm_field(INT32 imm)
{
    max_imm_field = std::max(max_imm_field, imm);
    min_imm_field = std::min(min_imm_field, imm);
}

VOID InstrumentationRoutine(INS ins, void *v)
{
    // Exit Routine beyond 1 trillion instrumentations
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)Terminate, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)MyExitRoutine, IARG_END);

    // keeping track of progress
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)CheckMilestone, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)PrintMilestone, IARG_END);

    // count instruction length
    UINT32 instr_size = INS_Size(ins);
    // count number of operands
    UINT32 instr_num_op = INS_OperandCount(ins);
    // count number of read operands
    UINT32 instr_num_read_op = INS_MaxNumRRegs(ins);
    // count number of write operands
    UINT32 instr_num_write_op = INS_MaxNumWRegs(ins);

    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)track_instruction_part1To4, IARG_UINT32, instr_size, IARG_UINT32, instr_num_op, IARG_UINT32, instr_num_read_op, IARG_UINT32, instr_num_write_op, IARG_END);

    UINT32 instr_num_mem_op = INS_MemoryOperandCount(ins);
    if (instr_num_mem_op > 0)
    {
        // for average number of bytes per instruction
        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
        INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)track_memory_instructions, IARG_END);
    }
    UINT32 instr_num_rmem_op = 0;
    UINT32 instr_num_wmem_op = 0;
    UINT32 instr_num_rwmem_op = 0;
    UINT32 instr_mem_size = 0;
    ADDRDELTA max_op_disp = INT32_MIN;
    ADDRDELTA min_op_disp = INT32_MAX;
    for (UINT32 memory_operand = 0; memory_operand < instr_num_mem_op; memory_operand++)
    {
        // count number of memory operands
        if (INS_MemoryOperandIsRead(ins, memory_operand))
            instr_num_rmem_op++;
        if (INS_MemoryOperandIsWritten(ins, memory_operand))
            instr_num_wmem_op++;
        instr_num_rwmem_op++;

        // track average and max bytes
        instr_mem_size += INS_MemoryOperandSize(ins, memory_operand);

        // track max and min displacement
        ADDRDELTA op_disp = INS_OperandMemoryDisplacement(ins, memory_operand);
        max_op_disp = std::max(max_op_disp, op_disp);
        min_op_disp = std::min(min_op_disp, op_disp);
    }

    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
    INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)track_memory_operands_details, IARG_UINT32, instr_num_rwmem_op, IARG_UINT32, instr_num_rmem_op, IARG_UINT32, instr_num_wmem_op, IARG_ADDRINT, min_op_disp, IARG_ADDRINT, max_op_disp, IARG_UINT32, instr_mem_size, IARG_END);

    instr_num_op = INS_OperandCount(ins);
    // track max and min of immediate
    for (UINT32 op = 0; op < instr_num_op; op++)
    {
        if (INS_OperandIsImmediate(ins, op))
        {
            // ADDRINT imm_val = INS_OperandImmediate(ins, op);
            // INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
            // INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)track_imm_field, IARG_ADDRINT, imm_val, IARG_END);
            num_imm_val++; // dummy variable
        }
    }

    // count instructions
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)InsCount, IARG_END);
}

int main(int argc, char **argv)
{
    if (PIN_Init(argc, argv))
        return Usage();

    fast_forward_count = KnobFastForwardTill.Value();
    std::string outFileName = KnobOutputFile.Value();
    std::string logFileName = "partD.log";
    out = new std::ofstream(outFileName.c_str());
    logOut = new std::ofstream(logFileName.c_str());

    INS_AddInstrumentFunction(InstrumentationRoutine, 0);
    PIN_AddFiniFunction(Fini, 0);
    PIN_StartProgram();

    return 0;
}