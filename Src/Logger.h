#ifndef LOGGER_H_
#define LOGGER_H_

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include "Common.h"

typedef struct Logger {
  struct timespec t0;
  //todo Here we could add other fiedls such ar file descriptor, etc...
} Logger;

static Logger _logger;
static bool _forceFlush; // This controls whether to fflush the logger at each log event or not.

void Logger_initializeStaticLogger();
void Logger_getExecutionTime(time_t *s, long *ms);
void LOG(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

#endif // LOGGER_H_
