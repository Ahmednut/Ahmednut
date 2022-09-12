#ifndef __DELAY_COUNTER__
#define __DELAY_COUNTER__
#include <bas_error.h>
enum clock_error {
  CLOCKOK = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_TIME, 0),
  CLOCKFAILURE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_TIME, 1),
  CLOCKNOTSUPPORTED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_TIME, 2),
  CLOCKINVALIDCOUNTER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_TIME, 3),
  CLOCKNOTSET = MAKE_RESULT(LYR_SEVERITY_WARNING, MODULE_TIME, 4),
};
#ifdef _MSC_VER
#include <Windows.h>
typedef struct {
    DWORD begin;
    DWORD end;
} delay_counter;
#else
#include <time.h>
typedef struct {
    struct timespec begin;
    struct timespec end;
} delay_counter;
#endif
Result delay_counter_reset(delay_counter * counter);
Result delay_counter_begin(delay_counter * counter);
Result delay_counter_end(delay_counter * counter);
Result delay_counter_get(delay_counter * counter, double * delay);
Result delay_counter_resolution(delay_counter * counter, double * resolution);
double calculate_speed(double delay, unsigned size);
#endif




