/*
 *  piper_p.cpp
 *  ReadBlend
 *
 *  Created by astojilj@gmail.com on 12/28/10.
 *  Copyright 2010-2011 astojilj@gmail.com. All rights reserved.
 *
 */

#include "piper_p.h"
#include "string.h"
#include "piperGL11.h"
#include "piperGL20.h"


const unsigned char stackSize(1 << MAX_STACK_DEPTH_LOG_2);

MatrixStack::MatrixStack() :
	topIndex(0)
{
}

/* Allowed to push, but all pushes above 1 << MAX_STACK_DEPTH_LOG_2 ends to same, pop comes back */
static unsigned char normalizedTop(unsigned char topIndex)
{
	// topIndex % (1 << MAX_STACK_DEPTH_LOG_2)
	return topIndex & ((1 << MAX_STACK_DEPTH_LOG_2) - 1);
}


void MatrixStack::set(const MATRIX &matrix) {
	memcpy(&(stack[normalizedTop(topIndex)]), &matrix, sizeof(MATRIX));
}

const MATRIX &MatrixStack::top() const
{
	return stack[normalizedTop(topIndex)];
}

void MatrixStack::push()
{
	topIndex++;
	if (topIndex != stackSize) {
		memcpy(&(stack[topIndex]), &(stack[topIndex-1]), sizeof(MATRIX));
	} 
}

void MatrixStack::pop()
{
	if (topIndex) {
		topIndex--;
	}
}

Piper *Piper::_instance = 0;

Piper *Piper::instance() 
{
	return _instance;
}

void Piper::initInstance(bool fixedPipeline) 
{
	delete _instance;
	_instance = 0;
#ifndef QT_BUILD //! Support only for OpenGL ES 2.0 in MeeGo for start
	if (fixedPipeline) {
		_instance = new PiperGL11;
	} else
#endif
	{
		_instance = new PiperGL20;
	}
}

void Piper::multMatrix(const GLfloat * m, Mode mode)
{
    MATRIX mat;
    memcpy(&mat, m, 16 * sizeof(GLfloat));
    multMatrix(mat, mode);
}
