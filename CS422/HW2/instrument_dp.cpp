/*
 * SPDX-License-Identifier: MIT
 */

/*! @file
 *  This is serves to demonstrate accuracies of various nuances in direction predictors.
 */

#include "direction_predictor.hpp"
#include "pin.H"

/* ================================================================== */
// Global variables
/* ================================================================== */

UINT64 insCount = 0;
UINT64 fastForwardCount = 0;
UINT64 totalConBranchCount = 0;
UINT64 forwardConBranchCount = 0;
UINT64 backwardConBranchCount = 0;

std::vector<DirectionPredictor *> dps(8, NULL);

/* ===================================================================== */
// Command line switches
/* ===================================================================== */

KNOB<string> KnobOutputFile(KNOB_MODE_OVERWRITE, "pintool", "o", "direction_predictor.out", "specify file name for MyPinTool output");
KNOB<UINT64> KnobFastForwardUntil(KNOB_MODE_OVERWRITE, "pintool", "f", "0", "specify number of instructions to fast-forward to until instrumentation starts");

/* ===================================================================== */
// Utilities
/* ===================================================================== */

/*!
 *  Print out help message.
 */
INT32 Usage()
{
    PIN_ERROR("This pintool implements direction predictors of conditional branches\n" + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}

VOID PrintMilestone(void)
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    *logOut << std::setw(20) << std::setfill(' ') << insCount << " done by ";
    *logOut << std::setw(30) << std::setfill(' ') << std::ctime(&end_time) << std::flush;
}

void print_results()
{
    int name_width = 20;
    int number_width = 10;
    char seperator = ' ';
    *out << std::left << std::setw(name_width) << std::setfill(seperator) << "#Branches";
    *out << std::right << std::setw(number_width) << std::setfill(seperator) << totalConBranchCount;
    *out << std::right << std::setw(number_width) << std::setfill(seperator) << forwardConBranchCount;
    *out << std::right << std::setw(number_width) << std::setfill(seperator) << backwardConBranchCount << endl;
    for (unsigned int i = 0; i < dps.size(); i++)
    {
        if (dps[i] == NULL)
            continue;
        double total = 100.0 - (double)(dps[i]->show_correct_predictions()) * 100.0 / totalConBranchCount;
        double forward = 100.0 - (double)(dps[i]->show_fcorrect_predictions()) * 100.0 / forwardConBranchCount;
        double backward = 100.0 - (double)(dps[i]->show_bcorrect_predictions()) * 100.0 / backwardConBranchCount;
        *out << std::left << std::setw(name_width) << std::setfill(seperator) << dps[i]->dp_name;
        *out << std::right << std::setw(number_width) << std::setfill(seperator) << total << "%";
        *out << std::right << std::setw(number_width) << std::setfill(seperator) << forward << "%";
        *out << std::right << std::setw(number_width) << std::setfill(seperator) << backward << "%" << endl;
    }
    // *out << "Debugging:\n";
    // *out << "working: " << working << endl;
}

/**
 * @brief Routine run once required number of instructions have been instrumented
 *
 */
void MyExitRoutine()
{
    for (unsigned int i = 0; i < dps.size(); i++)
        if (dps[i] != NULL)
            dps[i]->show_status();
    print_results();
    PrintMilestone();
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
    for (unsigned int i = 0; i < dps.size(); i++)
        dps[i]->show_status();
    print_results();
    PrintMilestone();
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
    return (insCount >= (fastForwardCount + BILLION));
}

ADDRINT CheckMilestone(void)
{
    return (insCount % (1 << 29)) == 0; // correct at nearly every half a million
}

VOID AnalysisRoutine(ADDRINT pc, bool branch_taken, ADDRINT branch_target)
{
    totalConBranchCount++;
    bool is_forward = branch_target > pc;
    forwardConBranchCount += is_forward;
    backwardConBranchCount += !is_forward;

    // predict
    for (unsigned int i = 0; i < dps.size(); i++)
    {
        dps[i]->predict(pc, branch_target);
    }

    // update
    for (unsigned int i = 0; i < dps.size(); i++)
    {
        dps[i]->update(branch_taken, is_forward);
    }
}

/* ===================================================================== */
// Instrumentation callbacks
/* ===================================================================== */

/**
 * @brief Instrumentation routine for the program,
 * inserts predictions and updates to predictors at every conditionals
 *
 * @param ins   Instrumentation called at instruction level
 * @param v
 * @return VOID
 */
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
        // is a conditional branch (branch with fallthrough)
        INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)FastForward, IARG_END);
        INS_InsertThenPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)AnalysisRoutine, IARG_ADDRINT, INS_Address(ins), IARG_BRANCH_TAKEN, IARG_BRANCH_TARGET_ADDR, IARG_END);
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
    string logFileName = "direction_predictor.log";
    fastForwardCount = KnobFastForwardUntil.Value() * BILLION;

    out = std::make_unique<std::ofstream>(fileName.c_str());
    logOut = std::make_unique<std::ofstream>(logFileName.c_str());
    *logOut << "Fast-forwarding by: " << fastForwardCount << std::endl;

    // debugging start
    working = 0;
    // debugging end

    dps[0] = new FNBT;
    dps[1] = new Bimodal;
    dps[2] = new SAg;
    dps[3] = new GAg;
    dps[4] = new Gshare;
    dps[5] = new T_SAgGAg;
    dps[6] = new M_SAgGAgGshare;
    dps[7] = new T_SAgGAgGshare;

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
