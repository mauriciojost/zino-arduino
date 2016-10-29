#ifndef MISC_INC
#define MISC_INC

#include <math.h>

#define constrainValue(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define rollValue(amt, low, high) ((amt) < (low) ? (high) : ((amt) > (high) ? (low) : (amt)))

#ifdef CYCLE_OF_1S
#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 1.024f
#else // CYCLE_OF_8S
#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 8.192f
#endif

// Colors to be used with printf in PC tests
#define KWHT  ""
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


#endif // MISC_INC
