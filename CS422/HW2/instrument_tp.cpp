/*
 * SPDX-License-Identifier: MIT
 */

/*! @file
 *  This is implements target prediction of indirect control flow
 *  using BTB
 */

#include "target_predictor.hpp"
#include "pin.H"

/* ================================================================== */
// Global variables
/* ================================================================== */

UINT64 insCount = 0;
UINT64 fastForwardCount = 0;
UINT64 totalIndirectFlowCount = 0;
int control_flow_not_indirect = 0;
int how_many_branches_not_taken = 0;
std::unique_ptr<BTB> btb;
std::unique_ptr<BTB_pro> btb_pro;
ADDRINT ghr = 0;

/* ===================================================================== */
// Command line switches
/* ===================================================================== */

KNOB<string> KnobOutputFile(KNOB_MODE_OVERWRITE, "pintool", "o", "target_predictor.out", "specify file name for MyPinTool output");
KNOB<UINT64> KnobFastForwardUntil(KNOB_MODE_OVERWRITE, "pintool", "f", "0", "specify number of instructions to fast-forward to until instrumentation starts");

/* ===================================================================== */
// Utilities
/* ===================================================================== */

/*!
 *  Print out help message.
 */
INT32 Usage()
{
    PIN_ERROR("This pintool implements branch target buffer for indirect control flow\n" + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}

void print_results()
{
    int name_width = 20;
    int number_width = 15;
    char seperator = ' ';
    *out << std::left << std::setw(name_width) << std::setfill(seperator) << "#Branches";
    *out << std::left << std::setw(number_width) << std::setfill(seperator) << totalIndirectFlowCount << endl;

    *out << std::left << std::setw(name_width) << std::setfill(seperator) << "BTB";
    *out << std::left << std::setw(number_width) << std::setfill(seperator) << (double)(btb->show_correct_predictions()) * 100.0 / totalIndirectFlowCount;
    *out << std::left << std::setw(number_width) << std::setfill(seperator) << (double)(btb->show_missed_predictions()) * 100.0 / totalIndirectFlowCount << std::endl;

    *out << std::left << std::setw(name_width) << std::setfill(seperator) << "BTB_pro";
    *out << std::left << std::setw(number_width) << std::setfill(seperator) << (double)(btb_pro->show_correct_predictions()) * 100.0 / totalIndirectFlowCount;
    *out << std::left << std::setw(number_width) << std::setfill(seperator) << (double)(btb_pro->show_missed_predictions()) * 100.0 / totalIndirectFlowCount << std::endl;
}

/**
 * @brief Routine run once required number of instructions have been instrumented
 *
 */
void MyExitRoutine()
{
    print_results();
    exit(0);
}

/*!
 * Print out analysis results.
 * This function is called when the application exits.
 * @param[in]   code            exit code of the application
 * @param[in]   v               value specified by the tool in the
 *                              PIN_AddFiniFunction function call
 */
VOID Fini(INT32 code, VOID *v)
{
    print_results();
}

/* ===================================================================== */
// Analysis routines
/* ===================================================================== */

VOID InstructionCount(void)
{
    insCount++;
}

ADDRINT FastForward(void)
{
    return ((insCount >= fastForwardCount) && (insCount));
}

ADDRINT Terminate(void)
{
    return (insCount >= fastForwardCount + BILLION);
}

ADDRINT CheckMilestone(void)
{
    return (insCount % (1 << 29)) == 0; // correct at nearly every half a million
}

VOID PrintMilestone(void)
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    *logOut << std::setw(20) << std::setfill(' ') << insCount << " done by ";
    *logOut << std::setw(30) << std::setfill(' ') << std::ctime(&end_time) << std::flush;
}

VOID IndirectAnalysisRoutine(ADDRINT pc, ADDRINT next_pc, ADDRINT target)
{
    totalIndirectFlowCount++;
    // predict and update
    btb->predict(pc, next_pc, target);
    btb_pro->predict(pc, next_pc, target, ghr);
}

VOID ConditionalAnalysisRoutine(bool branch_taken)
{
    ghr = (ghr << 1) + branch_taken;
}

/* ===================================================================== */
// Instrumentation callbacks
/* ===================================================================== */

VOID InstrumentationRoutine(INS ins, void *v)
{
    // Exit Routine beyond 1 billion instrumentations
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)Terminate, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)MyExitRoutine, IARG_END);
    // keeping track of progress
    INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)CheckMilestone, IARG_END);
    INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)PrintMilestone, IARG_END);

    if (INS_IsBranch(ins) && INS_HasFallThrough(ins))
    {
        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
        INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)ConditionalAnalysisRoutine, IARG_BRANCH_TAKEN, IARG_END);
    }

    if (INS_IsIndirectControlFlow(ins))
    {
        if (!INS_IsControlFlow(ins))
            control_flow_not_indirect++;
        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
        INS_InsertThenCall(ins, IPOINT_BEFORE, (AFUNPTR)IndirectAnalysisRoutine, IARG_ADDRINT, INS_Address(ins), IARG_ADDRINT, INS_NextAddress(ins), IARG_BRANCH_TARGET_ADDR, IARG_END);
    }

    // increase counter of instructions
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)InstructionCount, IARG_END);
}

/*!
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet started.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments,
 *                              including pin -t <toolname> -- ...
 */
int main(int argc, char *argv[])
{
    // Initialize PIN library. Print help message if -h(elp) is specified
    // in the command line or the command line is invalid
    if (PIN_Init(argc, argv))
    {
        return Usage();
    }

    string fileName = KnobOutputFile.Value();
    string logFileName = "target_predictor.log";
    fastForwardCount = KnobFastForwardUntil.Value() * BILLION;

    out = std::make_unique<std::ofstream>(fileName.c_str());
    logOut = std::make_unique<std::ofstream>(logFileName.c_str());
    *logOut << "Fast-forwarding by: " << fastForwardCount << std::endl;

    btb = std::make_unique<BTB>();
    btb_pro = std::make_unique<BTB_pro>();

    // Register function to be called to instrument traces
    INS_AddInstrumentFunction(InstrumentationRoutine, 0);

    // Register function to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    cerr << "===============================================" << endl;
    cerr << "This application is instrumented by PIN" << endl;
    if (!fileName.empty())
    {
        cerr << "See file " << fileName << " for analysis results" << endl;
    }
    cerr << "See file " << logFileName << " for logs" << endl;
    cerr << "===============================================" << endl;

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
