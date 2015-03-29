#define _POSIX_C_SOURCE 200809L

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "dbg.h"

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif
