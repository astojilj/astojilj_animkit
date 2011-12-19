/*
 *  piperGL20.h
 *  ReadBlend
 *
 *  Created by astoj@rocketmail.com on 12/28/10.
 *  Copyright 2010 astoj@rocketmail.com. All rights reserved.
 *
 */

#ifndef PIPERGL20_H
#define PIPERGL20_H

#include "piper.h"
#include "piper_p.h"

class ShaderManager;

/* Collecting and constructing input for graphics pipeline */
class PiperGL20 : public Piper
{
public:
	PiperGL20();
	virtual ~PiperGL20();
	
	//@reimp
	void setMatrix(const MATRIX &matrix, Piper::Mode mode);
	void pushMatrix(Piper::Mode mode);
	void popMatrix(Piper::Mode mode);
	void multMatrix(const MATRIX &matrix, Mode mode);
	void glDrawArrays (GLenum mode, GLint first, GLsizei count);
	void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
	void initFrame();
    void glColor4f(	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	//@end_reimp

private:
	void init();
	void setupChangedVariables();
	
private:	
	MatrixStack modeMatrix[(unsigned char)Piper::TEXTURE + 1];
	ShaderManager *shaderManager;
};


#endif //PIPERGL20_H
