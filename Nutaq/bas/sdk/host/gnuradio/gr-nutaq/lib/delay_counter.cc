#include "delay_counter.h"
#include <memory.h>
#define NSEC_PER_SEC    1000000000L
#define MSEC_PER_SEC    1000L

Result delay_counter_reset(delay_counter * counter)
{
    memset(counter, 0, sizeof(*counter));
    return CLOCKOK;
}

#ifdef _MSC_VER

Result delay_counter_begin(delay_counter * counter)
{
    counter->begin = timeGetTime();
    return CLOCKOK;
}
Result delay_counter_end(delay_counter * counter)
{
    counter->end = timeGetTime();
    return CLOCKOK;
}
Result delay_counter_get(delay_counter * counter, double * delay)
{
    DWORD delta;
    if(!counter->begin || !counter->end)
        return CLOCKNOTSET;
    delta = counter->end - counter->begin;
    *delay = ((double)delta)/((double)MSEC_PER_SEC);
    return CLOCKOK;
}
Result delay_counter_resolution(delay_counter * counter, double * resolution)
{
    *resolution = ((double)1)/((double)MSEC_PER_SEC);
    return CLOCKOK;
}
#else
#include <time.h>
#include <errno.h>
#include <memory.h>

static struct timespec timespec_sub(struct timespec lhs, struct timespec rhs)
{
  struct timespec ts_delta;
  ts_delta.tv_sec = lhs.tv_sec - rhs.tv_sec;
  ts_delta.tv_nsec = lhs.tv_nsec - rhs.tv_nsec;
  while (ts_delta.tv_nsec < 0){
    ts_delta.tv_sec--;
    ts_delta.tv_nsec += NSEC_PER_SEC;
  }
  return ts_delta;
}

static double timespec2double(struct timespec delta)
{
  double fdelta;
  fdelta = (double)delta.tv_sec;
  fdelta += ((double)delta.tv_nsec) / ((double)NSEC_PER_SEC);
  return fdelta;
}

static Result get_clock(struct timespec * time)
{
    int ret;
    Result result = CLOCKOK;
    ret = clock_gettime(CLOCK_MONOTONIC, time);
    if (ret<0)
    {
        switch(errno){
            case EINVAL:
                result = CLOCKNOTSUPPORTED;
                break;
            case EFAULT:
                result = CLOCKINVALIDCOUNTER;
                break;
            default:
                result = CLOCKFAILURE;
                break;
        }
    }
    return result;
}
Result delay_counter_begin(delay_counter * counter)
{
    return get_clock(&counter->begin);
}
Result delay_counter_end(delay_counter * counter)
{
    return get_clock(&counter->end);
}
Result delay_counter_get(delay_counter * counter, double * delay)
{
    struct timespec delta;
    if((!counter->begin.tv_nsec && !counter->begin.tv_sec) ||
        (!counter->end.tv_nsec && !counter->end.tv_sec))
        return CLOCKNOTSET;
    delta = timespec_sub(counter->end, counter->begin);
    *delay = timespec2double(delta);
    return CLOCKOK;
}
Result delay_counter_resolution(delay_counter * counter, double * resolution)
{
    struct timespec res;
    int ret;
    ret = clock_getres(CLOCK_MONOTONIC, &res);
    *resolution = timespec2double(res);
    if (ret<0)
        return CLOCKFAILURE;
    else
        return CLOCKOK;
}

#endif

/* return bytes/second */
double calculate_speed(double delay, unsigned size)
{
  double fsize = (double)size;//bytes
  return fsize/delay;
}
