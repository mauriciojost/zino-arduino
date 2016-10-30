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
// http://ascii-table.com/ansi-escape-sequences.php
#define KNRM  "\x1B[0m"
#define KBLU  "\x1B[34m"
#define KWHTBLU  "\x1B[37;44m"


#endif // MISC_INC
