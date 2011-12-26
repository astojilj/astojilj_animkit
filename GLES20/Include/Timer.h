/*
 Oolong Engine for the iPhone / iPod touch
 Copyright (c) 2007-2008 Wolfgang Engel  http://code.google.com/p/oolongengine/
 
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use of this software.
 Permission is granted to anyone to use this software for any purpose, 
 including commercial applications, and to alter it and redistribute it freely, 
 subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
*/
#include "Types.h"
#include "Mathematics.h"

#include <sys/time.h>

#include <stdio.h>

#ifdef __APPLE__

#include <mach/mach.h>
#include <mach/mach_time.h>

//int GetFps(int frame, CFTimeInterval &TimeInterval, CFTimeInterval *frameTime=NULL);
#endif

typedef struct
{
#ifdef __APPLE__
        uint64_t startTime;
        mach_timebase_info_data_t info;
#else
        timeval  startTime;
#endif
} structTimer;


#ifdef __APPLE__
void StartTimer(structTimer* timer)
{
	/* Get the timebase info */
	mach_timebase_info(&timer->info);
}

void ResetTimer(structTimer* timer)
{
	// calculate our local time
	timer->startTime = mach_absolute_time();
}

float GetAverageTimeValueInMS(structTimer* timer)
{
	uint64_t TimeInterval;
	
	// calculate our local time
	TimeInterval = mach_absolute_time();
	
	uint64_t duration = TimeInterval - timer->startTime;
	/* Return in miliseconds */
	duration = (float)(duration * timer->info.numer)/(float)(timer->info.denom*1000000.f);
	return duration;
}

float elapsedTimeInS(structTimer* timer)
{
	return GetAverageTimeValueInMS(timer) / 1000.0f;
}


#else
float elapsedTimeInS(structTimer* timer)
{
    struct  timezone  tz;
    timeval  t1;
    gettimeofday(&t1, &tz);
    return  (t1.tv_sec - timer->startTime.tv_sec) + (t1.tv_usec - timer->startTime.tv_usec) * 1e-6;
}

void ResetTimer(structTimer* timer)
{
    struct  timezone  tz;
    gettimeofday(&(timer->startTime), &tz);
}



#endif



