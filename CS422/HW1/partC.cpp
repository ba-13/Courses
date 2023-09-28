#include "common.h"
#include <set>
#include <unordered_map>

#define G 32

std::set<ADDRINT> instruction_chunks;
// std::unordered_map<ADDRINT, UINT32> instruction_chunks_visits;
std::set<ADDRINT> memory_chunks;
// std::unordered_map<ADDRINT, UINT32> memory_chunks_visits;
ADDRINT start_chunk, end_chunk;

ADDRINT chunk_start(ADDRINT addr)
{
    return (addr / G) * G;
}

KNOB<std::string> KnobOutputFile(KNOB_MODE_OVERWRITE, "pintool", "o", "partC.out", "specify file name for partC.so output");
KNOB<UINT64> KnobFastForwardTill(KNOB_MODE_OVERWRITE, "pintool", "f", "0", "fast forward by this many instructions");

INT32 Usage()
{
    PIN_ERROR("This pintool implements categorisation of instructions\n" + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}

void print_results()
{
    *out << "Instruction Memory Accessed Chunks " << instruction_chunks.size() << std::endl;
    *out << "Data Memory Accessed Chunks " << memory_chunks.size() << std::endl;
    // can use number of visits of each chunks as well
}

VOID Fini(INT32 code, void *v)
{
    // insert end results
    print_results();
    return;
}

void MyExitRoutine()
{
    // Do an exit system call to exit the application.
    // As we are calling the exit system call PIN would not be able to instrument application end.
    // Because of this, even if you are instrumenting the application end, the Fini function would not
    // be called. Thus you should report the statistics here, before doing the exit system call.

    // Results etc
    print_results();
    exit(0);
}

void get_instruction_details(ADDRINT addr, UINT32 size)
{
    // *logOut << std::endl
    //         << "(" << addr << ", " << size << "):";

    start_chunk = chunk_start(addr);
    end_chunk = chunk_start(addr + size);
    for (ADDRINT chunk = start_chunk; chunk <= end_chunk; chunk += G)
    {
        instruction_chunks.insert(chunk);
        // if (instruction_chunks_visits.find(chunk) != instruction_chunks_visits.end())
        //     instruction_chunks_visits[chunk]++;
        // else
        //     instruction_chunks_visits[chunk] = 1;
    }
}

void get_memory_details(ADDRINT addr, UINT32 size)
{
    // *logOut << "(" << addr << ", " << size << ")";

    start_chunk = chunk_start(addr);
    end_chunk = chunk_start(addr + size);
    for (ADDRINT chunk = start_chunk; chunk <= end_chunk; chunk += G)
    {
        memory_chunks.insert(chunk);
        // if (memory_chunks_visits.find(chunk) != memory_chunks_visits.end())
        //     memory_chunks_visits[chunk]++;
        // else
        //     memory_chunks_visits[chunk] = 1;
    }
}

VOID InstrumentationRoutine(INS ins, void *v)
{
    // Exit Routine beyond 1 trillion instrumentations
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)Terminate, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)MyExitRoutine, IARG_END);

    // keeping track of progress
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)CheckMilestone, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)PrintMilestone, IARG_END);

    // instr address details
    ADDRINT instr_addr = INS_Address(ins);
    UINT32 instr_size = INS_Size(ins);
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)get_instruction_details, IARG_ADDRINT, instr_addr, IARG_UINT32, instr_size, IARG_END);

    // Iterate through memory operands of the instruction
    UINT32 memory_operands = INS_MemoryOperandCount(ins);
    for (UINT32 memory_operand = 0; memory_operand < memory_operands; memory_operand++)
    {
        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
        INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)get_memory_details, IARG_MEMORYOP_EA, memory_operand, IARG_MEMORYOP_SIZE, memory_operand, IARG_END);
    }

    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)InsCount, IARG_END);
}

int main(int argc, char **argv)
{
    if (PIN_Init(argc, argv))
        return Usage();

    fast_forward_count = KnobFastForwardTill.Value();
    std::string outFileName = KnobOutputFile.Value();
    std::string logFileName = "partC.log";
    out = new std::ofstream(outFileName.c_str());
    logOut = new std::ofstream(logFileName.c_str());

    INS_AddInstrumentFunction(InstrumentationRoutine, 0);
    PIN_AddFiniFunction(Fini, 0);
    PIN_StartProgram();

    return 0;
}