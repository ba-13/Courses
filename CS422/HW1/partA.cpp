#include "pin.H"
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>

#define instrumentation_duration 1'000'000'000
UINT64 counts[19] = {0};
UINT64 instruction_count = 0;
UINT64 fast_forward_count = 0;
std::ostream *out = &(std::cerr);
std::ostream *logOut = &(std::cerr);

KNOB<std::string> KnobOutputFile(KNOB_MODE_OVERWRITE, "pintool", "o", "partA.out", "specify file name for partA.so output");
KNOB<UINT64> KnobFastForwardTill(KNOB_MODE_OVERWRITE, "pintool", "f", "0", "fast forward by this many instructions");

INT32 Usage()
{
    PIN_ERROR("This pintool implements categorisation of instructions\n" + KNOB_BASE::StringKnobSummary() + "\n");
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
    *out << "Analysed Instruction Count " << print_ic_line_(counts[18], total_instructions);
    *out << "Effective Instruction Count : " << total_instructions << std::endl;
    *out << "Instruction Count : " << instruction_count << std::endl;
}

ADDRINT FastForward(void)
{
    return ((instruction_count >= fast_forward_count) && (instruction_count));
}

ADDRINT Terminate(void)
{
    return (instruction_count >= (fast_forward_count + instrumentation_duration));
}

ADDRINT CheckMilestone(void)
{
    return (instruction_count % (1 << 30)) == 0;
}

void MyExitRoutine()
{
    // Do an exit system call to exit the application.
    // As we are calling the exit system call PIN would not be able to instrument application end.
    // Because of this, even if you are instrumenting the application end, the Fini function would not
    // be called. Thus you should report the statistics here, before doing the exit system call.

    // Results etc
    print_instructions_count();
    exit(0);
}

VOID Fini(INT32 code, VOID *v)
{
    // called if program gets over without going through instrumentation_duration
    print_instructions_count();
    return;
}

void InsCount()
{
    instruction_count++;
}

void FFInsCount()
{
    counts[18]++;
}

void PrintMilestone()
{
    *logOut << instruction_count << " done" << std::endl;
}

void MyPredicatedAnalysis(UINT32 idx)
{
    // memory analysis code
    counts[idx]++;
}

void MyPredicatedAnalysis2(UINT32 idx, UINT32 size)
{
    counts[idx] += size;
    instruction_count += size;
}

VOID InstrumentationRoutine(INS ins, void *v)
{
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)Terminate, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)MyExitRoutine, IARG_END);

    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)CheckMilestone, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)PrintMilestone, IARG_END);

    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)FFInsCount, IARG_END);

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

    UINT32 memOperands = INS_MemoryOperandCount(ins);

    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
    INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)MyPredicatedAnalysis, IARG_UINT32, idx, IARG_END);

    if (memOperands > 0)
    {
        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
        INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)MyPredicatedAnalysis, IARG_UINT32, 17, IARG_END); // 17 is memory instruction count
        for (UINT32 memOperand = 0; memOperand < memOperands; memOperand++)
        {
            UINT32 mem_access_size = ceil((double)INS_MemoryOperandSize(ins, memOperand) / 4.0);
            if (INS_MemoryOperandIsRead(ins, memOperand))
            {
                idx = 0;
                INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
                INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)MyPredicatedAnalysis2, IARG_UINT32, idx, IARG_UINT32, mem_access_size, IARG_END);
            }
            if (INS_MemoryOperandIsWritten(ins, memOperand))
            {
                idx = 1;
                INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
                INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)MyPredicatedAnalysis2, IARG_UINT32, idx, IARG_UINT32, mem_access_size, IARG_END);
            }
        }
    }

    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)InsCount, IARG_END);
}

int main(int argc, char **argv)
{
    if (PIN_Init(argc, argv))
        return Usage();

    std::string fileName = KnobOutputFile.Value();
    std::string logFileName = "partA.log";
    if (!fileName.empty())
        out = new std::ofstream(fileName.c_str());
    logOut = new std::ofstream(logFileName.c_str());

    fast_forward_count = KnobFastForwardTill.Value();

    INS_AddInstrumentFunction(InstrumentationRoutine, 0);
    PIN_AddFiniFunction(Fini, 0);
    PIN_StartProgram();

    return 0;
}