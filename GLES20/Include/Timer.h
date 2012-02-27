/*
 *  Timer.cpp
 *  gles20farm
 *
 *  Created by astojilj@gmail.com on 1/29/12.
 *  Copyright 2012 astojilj@gmail.com All rights reserved.
 *
 */

#ifndef ELAPSED_TIMER_H
#define ELAPSED_TIMER_H

#include "Types.h"
#include "Mathematics.h"

#include <sys/time.h>

#include <stdio.h>

#ifdef __APPLE__

#include <mach/mach.h>
#include <mach/mach_time.h>

#endif

class ElapsedTimer
{
public:
    ElapsedTimer();
    float elapsed();    
#ifdef __APPLE__
        uint64_t startTime;
        mach_timebase_info_data_t info;
#else
        timeval  startTime;
#endif
};

#endif // ELAPSED_TIMER_H



