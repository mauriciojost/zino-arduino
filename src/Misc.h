#ifndef MISC_INC
#define MISC_INC

#include <math.h>

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#define constrainValue(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define rollValue(amt, low, high) ((amt) < (low) ? (high) : ((amt) > (high) ? (low) : (amt)))

// Colors to be used with printf in PC tests
// http://ascii-table.com/ansi-escape-sequences.php
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KMAG "\x1B[35m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KWHTBLU "\x1B[37;44m"

#define LCD_LENGTH 16

#endif // MISC_INC
