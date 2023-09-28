#include "pin.H"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <ctime>
#include <set>
#include <unordered_map>

#define instrumentation_duration 1'000'000'000
#if defined(TARGET_IA32)
typedef INT32 CUSTOM_INT;
#define CUSTOM_INT_MAX INT32_MAX
#define CUSTOM_INT_MIN INT32_MIN
#else
typedef INT64 CUSTOM_INT;
#define CUSTOM_INT_MAX INT64_MAX
#define CUSTOM_INT_MIN INT64_MIN
#endif

/**==============================*/
/** Global Variables */
/**==============================*/

UINT64 instruction_count;
UINT64 fast_forward_count;
std::ostream *out = &(std::cerr);
std::ostream *logOut = &(std::cerr);

/** PART A*/

std::vector<UINT64> counts(19, 0);

/** PART C*/

#define G 32
std::set<ADDRINT> instruction_chunks;
std::set<ADDRINT> memory_chunks;
ADDRINT start_chunk, end_chunk;

/** PART D*/

std::vector<UINT32> instruction_sizes(20, 0);
std::vector<UINT32> instruction_num_op(10, 0);
std::vector<UINT32> instruction_num_read_op(10, 0);
std::vector<UINT32> instruction_num_write_op(10, 0);
std::vector<UINT32> instruction_num_mem_op(5, 0);
std::vector<UINT32> instruction_num_rmem_op(5, 0);
std::vector<UINT32> instruction_num_wmem_op(5, 0);
UINT64 total_bytes_accessed = 0;
UINT32 max_bytes_accessed = 0;
CUSTOM_INT max_imm_field = CUSTOM_INT_MIN;
CUSTOM_INT min_imm_field = CUSTOM_INT_MAX;
INT32 max_disp_op = INT32_MIN;
INT32 min_disp_op = INT32_MAX;
UINT32 num_imm_val = 0;

/**==============================*/
/** KNOBS */
/**==============================*/

KNOB<std::string> KnobOutputFile(KNOB_MODE_OVERWRITE, "pintool", "o", "total.out", "specify file name for partA.so output");
KNOB<UINT64> KnobFastForwardTill(KNOB_MODE_OVERWRITE, "pintool", "f", "0", "fast forward by this many instructions");

/**==============================*/
/** Common Functions */
/**==============================*/

ADDRINT FastForward(void)
{
    return ((instruction_count >= fast_forward_count) && (instruction_count));
}

ADDRINT Terminate(void)
{
    return (instruction_count >= fast_forward_count + instrumentation_duration);
}

ADDRINT CheckMilestone(void)
{
    return (instruction_count % (1 << 29)) == 0; // correct at nearly every half a million
}

VOID PrintMilestone(void)
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    *logOut << instruction_count << " done at " << std::ctime(&end_time) << std::flush;
}

INT32 Usage()
{
    PIN_ERROR("This pintool implements Assignment 1\n" + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}

std::string print_ic_line_(UINT64 count, UINT64 total)
{
    std::stringstream ss;
    ss << ": " << count << " " << count * 100.0 / total << "%" << std::endl;
    return ss.str();
}

VOID print_instructions_count()
{
    UINT64 total_instructions = 0; // this will overcount an instruction
    // as set of type-A + loads + stores
    for (int i = 0; i < 17; i++)
        total_instructions += counts[i];

    *out << "1. Loads " << print_ic_line_(counts[0], total_instructions);
    *out << "2. Stores " << print_ic_line_(counts[1], total_instructions);
    *out << "3. NOPs " << print_ic_line_(counts[2], total_instructions);
    *out << "4. Direct calls " << print_ic_line_(counts[3], total_instructions);
    *out << "5. Indirect calls " << print_ic_line_(counts[4], total_instructions);
    *out << "6. Returns " << print_ic_line_(counts[5], total_instructions);
    *out << "7. Unconditional branches " << print_ic_line_(counts[6], total_instructions);
    *out << "8. Conditional branches " << print_ic_line_(counts[7], total_instructions);
    *out << "9. Logical operations " << print_ic_line_(counts[8], total_instructions);
    *out << "10. Rotate and shift " << print_ic_line_(counts[9], total_instructions);
    *out << "11. Flag operations " << print_ic_line_(counts[10], total_instructions);
    *out << "12. Vector instructions " << print_ic_line_(counts[11], total_instructions);
    *out << "13. Conditional moves " << print_ic_line_(counts[12], total_instructions);
    *out << "14. MMX and SSE instructions " << print_ic_line_(counts[13], total_instructions);
    *out << "15. System calls " << print_ic_line_(counts[14], total_instructions);
    *out << "16. Floating-point " << print_ic_line_(counts[15], total_instructions);
    *out << "17. The rest " << print_ic_line_(counts[16], total_instructions);
    *out << "Memory instructions " << print_ic_line_(counts[17], total_instructions);
    *out << "Analysed Instruction Count : " << counts[18] << std::endl;
    *out << "Effective Instruction Count : " << total_instructions << std::endl;
    *out << "Instruction Count : " << instruction_count << std::endl;
}

void print_chunk_data()
{
    *out << "Instruction Memory Accessed Chunks " << instruction_chunks.size() << std::endl;
    *out << "Data Memory Accessed Chunks " << memory_chunks.size() << std::endl;
    // can use number of visits of each chunks as well
}

void print_ia32_features()
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
    *out << "Average memory bytes touched by one instruction " << total_bytes_accessed << "/" << counts[17] << std::endl;

    *out << "Max Imm " << max_imm_field << std::endl;
    *out << "Min Imm " << min_imm_field << std::endl;

    *out << "Max Disp " << max_disp_op << std::endl;
    *out << "Min Disp " << min_disp_op << std::endl;

    *out << "Total Imm Sum " << num_imm_val << std::endl;
}

void MyExitRoutine()
{
    print_instructions_count();
    *out << "====================\n";
    print_chunk_data();
    *out << "====================\n";
    print_ia32_features();
    *out << "====================\n";
    exit(0);
}

VOID Fini(INT32 code, VOID *v)
{
    // called if program gets over without going through instrumentation_duration
    print_instructions_count();
    *out << "====================\n";
    print_chunk_data();
    *out << "====================\n";
    print_ia32_features();
    *out << "====================\n";
    return;
}

/**==============================*/
/** Analysis */
/**==============================*/

ADDRINT chunk_start(ADDRINT addr)
{
    return (addr / G) * G; // PART C
}

void InsCount()
{
    instruction_count++;
}

void FFInsCount()
{
    counts[18]++; // Counts actual number of instructions instrumented
}

void track_instruction_type_except_loadstore(UINT32 idx)
{
    counts[idx]++; // PART A
}

void track_instruction_operand_type_loadstore_and_chunks(UINT32 idx, UINT32 partA_size, ADDRINT addr, UINT32 size)
{
    counts[idx] += partA_size;       // PART A
    instruction_count += partA_size; // Counter increased effectively

    // PART C
    start_chunk = chunk_start(addr);
    end_chunk = chunk_start(addr + size);
    for (ADDRINT chunk = start_chunk; chunk <= end_chunk; chunk += G)
        memory_chunks.insert(chunk);
}

void get_instruction_details(ADDRINT addr, UINT32 partC_size, UINT32 size, UINT32 num_op, UINT32 num_read_op, UINT32 num_write_op)
{
    // PART C
    start_chunk = chunk_start(addr);
    end_chunk = chunk_start(addr + partC_size);
    for (ADDRINT chunk = start_chunk; chunk <= end_chunk; chunk += G)
        instruction_chunks.insert(chunk);

    // PART D
    instruction_sizes[size]++;
    instruction_num_op[num_op]++;
    instruction_num_read_op[num_read_op]++;
    instruction_num_write_op[num_write_op]++;
}

/** PART D*/

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

VOID track_imm_field(CUSTOM_INT min_imm, CUSTOM_INT max_imm)
{
    min_imm_field = std::min(min_imm_field, min_imm);
    max_imm_field = std::max(max_imm_field, max_imm);
}

/**==============================*/
/** Instrumentation */
/**==============================*/

VOID InstrumentationRoutine(INS ins, void *v)
{
    { // Exit Routine beyond 1 trillion instrumentations
        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)Terminate, IARG_END);
        INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)MyExitRoutine, IARG_END);

        // keeping track of progress
        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)CheckMilestone, IARG_END);
        INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)PrintMilestone, IARG_END);

        // count number of instrumented instructions
        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
        INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)FFInsCount, IARG_END);
    }

    UINT32 idx = -1;
    // conditions for Type-A
    {
        if (INS_Category(ins) == XED_CATEGORY_NOP)
        {
            idx = 2;
        }
        else if (INS_Category(ins) == XED_CATEGORY_CALL)
        {
            if (INS_IsDirectCall(ins))
            {
                idx = 3;
            }
            else
            {
                idx = 4;
            }
        }
        else if (INS_Category(ins) == XED_CATEGORY_RET)
        {
            idx = 5;
        }
        else if (INS_Category(ins) == XED_CATEGORY_UNCOND_BR)
        {
            idx = 6;
        }
        else if (INS_Category(ins) == XED_CATEGORY_COND_BR)
        {
            idx = 7;
        }
        else if (INS_Category(ins) == XED_CATEGORY_LOGICAL)
        {
            idx = 8;
        }
        else if ((INS_Category(ins) == XED_CATEGORY_ROTATE) || (INS_Category(ins) == XED_CATEGORY_SHIFT))
        {
            idx = 9;
        }
        else if (INS_Category(ins) == XED_CATEGORY_FLAGOP)
        {
            idx = 10;
        }
        else if ((INS_Category(ins) == XED_CATEGORY_AVX) || (INS_Category(ins) == XED_CATEGORY_AVX2) || (INS_Category(ins) == XED_CATEGORY_AVX2GATHER) || (INS_Category(ins) == XED_CATEGORY_AVX512))
        {
            idx = 11;
        }
        else if (INS_Category(ins) == XED_CATEGORY_CMOV)
        {
            idx = 12;
        }
        else if ((INS_Category(ins) == XED_CATEGORY_MMX) || (INS_Category(ins) == XED_CATEGORY_SSE))
        {
            idx = 13;
        }
        else if (INS_Category(ins) == XED_CATEGORY_SYSCALL)
        {
            idx = 14;
        }
        else if (INS_Category(ins) == XED_CATEGORY_X87_ALU)
        {
            idx = 15;
        }
        else
        {
            idx = 16;
        }
    }

    // PART D 1-4
    // count instruction length
    UINT32 instr_size = INS_Size(ins);
    // count number of operands
    UINT32 instr_num_op = INS_OperandCount(ins);
    // count number of read operands
    UINT32 instr_num_read_op = INS_MaxNumRRegs(ins);
    // count number of write operands
    UINT32 instr_num_write_op = INS_MaxNumWRegs(ins);

    // PART C instr address details
    ADDRINT instr_addr = INS_Address(ins);

    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)get_instruction_details, IARG_ADDRINT, instr_addr, IARG_UINT32, instr_size, IARG_UINT32, instr_size, IARG_UINT32, instr_num_op, IARG_UINT32, instr_num_read_op, IARG_UINT32, instr_num_write_op, IARG_END);

    UINT32 memory_operands = INS_MemoryOperandCount(ins);

    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
    INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)track_instruction_type_except_loadstore, IARG_UINT32, idx, IARG_END);

    if (memory_operands > 0)
    {
        // PART A get number of memory instructions
        // PART D for average
        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
        INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)track_instruction_type_except_loadstore, IARG_UINT32, 17, IARG_END); // 17 is memory instruction count

        UINT32 instr_num_rmem_op = 0;
        UINT32 instr_num_wmem_op = 0;
        UINT32 instr_mem_size = 0;
        ADDRDELTA max_op_disp = INT32_MIN;
        ADDRDELTA min_op_disp = INT32_MAX;
        for (UINT32 memory_operand = 0; memory_operand < memory_operands; memory_operand++)
        {
            // PART A get number of load/stores
            if (INS_MemoryOperandIsRead(ins, memory_operand))
            {
                idx = 0;
                instr_num_rmem_op++; // PART D
            }
            if (INS_MemoryOperandIsWritten(ins, memory_operand))
            {
                idx = 1;
                instr_num_wmem_op++; // PART D
            }

            UINT32 curr_op_size = INS_MemoryOperandSize(ins, memory_operand);

            // PART D track average and max bytes
            instr_mem_size += curr_op_size;
            // PART A check how many 32B accessed by op
            UINT32 mem_access_size = ceil((double)curr_op_size / 4.0);

            // track max and min displacement
            ADDRDELTA op_disp = INS_OperandMemoryDisplacement(ins, memory_operand);
            max_op_disp = std::max(max_op_disp, op_disp);
            min_op_disp = std::min(min_op_disp, op_disp);

            INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
            // and PART C get data memory addr and size
            INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)track_instruction_operand_type_loadstore_and_chunks, IARG_UINT32, idx, IARG_UINT32, mem_access_size, IARG_MEMORYOP_EA, memory_operand, IARG_MEMORYOP_SIZE, memory_operand, IARG_END);
        }

        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
        INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)track_memory_operands_details, IARG_UINT32, memory_operands, IARG_UINT32, instr_num_rmem_op, IARG_UINT32, instr_num_wmem_op, IARG_ADDRINT, min_op_disp, IARG_ADDRINT, max_op_disp, IARG_UINT32, instr_mem_size, IARG_END);
    }

    instr_num_op = INS_OperandCount(ins);
    CUSTOM_INT max_imm_val = CUSTOM_INT_MIN;
    CUSTOM_INT min_imm_val = CUSTOM_INT_MAX;

    // track max and min of immediate
    for (UINT32 op = 0; op < instr_num_op; op++)
    {
        if (INS_OperandIsImmediate(ins, op))
        {
            CUSTOM_INT imm_val = INS_OperandImmediate(ins, op);
            max_imm_val = std::max(max_imm_val, imm_val);
            min_imm_val = std::min(min_imm_val, imm_val);
            num_imm_val++; // dummy variable
        }
    }
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)track_imm_field, IARG_ADDRINT, min_imm_val, IARG_ADDRINT, max_imm_val, IARG_END);

    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)InsCount, IARG_END);
}

int main(int argc, char **argv)
{
    if (PIN_Init(argc, argv))
        return Usage();

    std::string fileName = KnobOutputFile.Value();
    std::string logFileName = "total.log";
    if (!fileName.empty())
        out = new std::ofstream(fileName.c_str());
    logOut = new std::ofstream(logFileName.c_str());

    fast_forward_count = KnobFastForwardTill.Value() * instrumentation_duration;

    INS_AddInstrumentFunction(InstrumentationRoutine, 0);
    PIN_AddFiniFunction(Fini, 0);
    PIN_StartProgram();

    return 0;
}