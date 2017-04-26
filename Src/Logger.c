#define _GNU_SOURCE //for vasprintf

#include "Logger.h"

void Logger_initializeStaticLogger(bool forceFlush_) {
  _logger.t0.tv_sec = (time_t) 0;
  _logger.t0.tv_nsec = (long) 0;
  Logger_getExecutionTime(&(_logger.t0.tv_sec), &(_logger.t0.tv_nsec));
  _forceFlush = forceFlush_;
}

void Logger_getExecutionTime(time_t *s, long *micros) {
  struct timespec spec;
  /*
  * Below use:
  *   -CLOCK_MONOTONIC: to get elapsed exec time
  *   -CLOCK_REALTIME: to get the time of the day
  */
  clock_gettime(CLOCK_MONOTONIC, &spec);
  *s = spec.tv_sec - _logger.t0.tv_sec;
  *micros = round((spec.tv_nsec - _logger.t0.tv_nsec) / 1.0e3);
}

void LOG(const char *format, ...) {
  // timing
  time_t *s = malloc(sizeof(time_t));
  long *micros = malloc(sizeof(long));
  Logger_getExecutionTime(s, micros);
  //
  char *text;
  va_list args;

  va_start(args, format);
  if(0 > vasprintf(&text, format, args))
    text = NULL;    //this is for logging, so failed allocation is not fatal
  va_end(args);

  if (text) {
    printf("%5ld.%06ld | %s\n", (intmax_t)*s, *micros, text); //trying %5d instead of %"PRIdMAX"
    free(text);
  }
  else {
    printf("CRITICAL: Error while logging trace, memory allocation failed.\n");
  }
  _forceFlush && fflush(stdout);
  free(s);
  free(micros);
}

//eof

