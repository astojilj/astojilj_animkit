/*
 *  piper_p.h
 *  ReadBlend
 *
 *  Created by astoj@rocketmail.com on 12/28/10.
 *  Copyright 2010 astoj@rocketmail.com. All rights reserved.
 *
 */

#ifndef PIPER_P_H
#define PIPER_P_H

#include "piper.h"

#define MAX_STACK_DEPTH_LOG_2 4

class MatrixStack 
{
public:
	MatrixStack();
	const MATRIX &top() const;
	void set(const MATRIX &);
	void push();
	void pop();

private:
    // MAX_STACK_DEPTH good enough - avoid malloc or realloc in render method
    MATRIX stack[(1 << MAX_STACK_DEPTH_LOG_2)];
	unsigned char topIndex;
};

#endif
