#include "pin.H"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <ctime>

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

UINT64 instruction_count;
UINT64 fast_forward_count;
std::ostream *out = &(std::cerr);
std::ostream *logOut = &(std::cerr);

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
    return (instruction_count % (1 << 29)) == 0;
}

VOID InsCount(void)
{
    instruction_count++;
}

VOID PrintMilestone(void)
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    *logOut << instruction_count << " done at " << std::ctime(&end_time) << std::endl;
}
