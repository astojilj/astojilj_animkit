/*
 *  piper.cpp
 *  ReadBlend
 *
 *  Created by astoj@rocketmail.com on 12/28/10.
 *  Copyright 2010 astoj@rocketmail.com. All rights reserved.
 *
 */

#ifndef QT_BUILD // only OpenGL ES 2.0 for Qt in first version
#include "piperGL11.h"
#include <stdio.h>


PiperGL11::PiperGL11()
{
	currentMode = (Piper::Mode)-1;
}

static unsigned int toOpenGLMatrixMode(Piper::Mode mode) 
{
	switch (mode) {
		case Piper::PROJECTION:
			return GL_PROJECTION;
		case Piper::MODEL:
			return GL_MODELVIEW;
		case Piper::TEXTURE:
			return GL_TEXTURE;
		default:
			// unsupported
			return 0;
	}
}

bool PiperGL11::setMode(const Piper::Mode mode) 
{
	if (mode != currentMode) {
		if (unsigned int glMode = toOpenGLMatrixMode(mode)) {
			glMatrixMode(glMode); 
			currentMode = mode;
		} else {
			return false;
		}
	}
	return true;
}

void PiperGL11::setMatrix(const MATRIX &matrix, Piper::Mode mode)
{
	if (setMode(mode))
		glLoadMatrixf(matrix.f);
}

void PiperGL11::pushMatrix(Piper::Mode mode)
{
	if (setMode(mode))
		glPushMatrix();
}

void PiperGL11::popMatrix(Piper::Mode mode)
{
	if (setMode(mode))
		glPopMatrix();
}

void PiperGL11::multMatrix(const MATRIX &matrix, Mode mode)
{
	if (setMode(mode))
		glMultMatrixf(matrix.f);
}

void PiperGL11::glColor4f(	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    ::glColor4f( red, green, blue, alpha);
}

#endif
