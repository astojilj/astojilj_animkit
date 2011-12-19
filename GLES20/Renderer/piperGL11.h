/*
 *  piperGL11.h
 *  ReadBlend
 *
 *  Created by astoj@rocketmail.com on 12/28/10.
 *  Copyright 2010 astoj@rocketmail.com. All rights reserved.
 *
 */

#ifndef PIPERGL11_H
#define PIPERGL11_H

#include "piper.h"

/* Collecting and constructing input for graphics pipeline */
class PiperGL11 : public Piper
{
public:
	PiperGL11();
	//@reimp
	void setMatrix(const MATRIX &matrix, Piper::Mode mode);
	void pushMatrix(Piper::Mode mode);
	void popMatrix(Piper::Mode mode);
	void multMatrix(const MATRIX &matrix, Mode mode);
    virtual void glColor4f(	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	//@end_reimp
	
private:
	bool setMode(const Piper::Mode mode);
	Piper::Mode currentMode;
};


#endif //PIPERGL11_H
