#ifndef MISC_INC
#define MISC_INC

#include <math.h>

#define constrainValue(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define rollValue(amt, low, high) ((amt) < (low) ? (high) : ((amt) > (high) ? (low) : (amt)))

#endif // MISC_INC
