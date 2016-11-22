#ifndef MISC_INC
#define MISC_INC

#include <math.h>

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#define constrainValue(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define rollValue(amt, low, high) ((amt) < (low) ? (high) : ((amt) > (high) ? (low) : (amt)))

#define LCD_LENGTH 16

#endif // MISC_INC
