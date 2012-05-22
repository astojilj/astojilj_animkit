/*
 *  Timer.cpp
 *  gles20farm
 *
 *  Created by astojilj@gmail.com on 1/29/12.
 *  Copyright 2012 astojilj@gmail.com All rights reserved.
 *
 */

#include "Timer.h"

#ifdef __APPLE__
static void StartTimer(ElapsedTimer* timer)
{
	/* Get the timebase info */
	mach_timebase_info(&timer->info);
}

static void ResetTimer(ElapsedTimer* timer)
{
	// calculate our local time
	timer->startTime = mach_absolute_time();
}
#else

static void ResetTimer(ElapsedTimer* timer)
{
    struct  timezone  tz;
    gettimeofday(&(timer->startTime), &tz);
}

#endif


ElapsedTimer::ElapsedTimer()
{
    StartTimer(this);
    ResetTimer(this);
}

float ElapsedTimer::elapsed()
{
#ifdef __APPLE__
	uint64_t TimeInterval;
	TimeInterval = mach_absolute_time();
	uint64_t duration = TimeInterval - this->startTime;
	return (float)(duration * info.numer)/(float)(info.denom*1000000000.f);
#else
    struct  timezone  tz;
    timeval  t1;
    gettimeofday(&t1, &tz);
    return  (t1.tv_sec - timer->startTime.tv_sec) + (t1.tv_usec - timer->startTime.tv_usec) * 1e-6;
#endif
}


