/*
 *  piper.h
 *  ReadBlend
 *
 *  Created by astoj@rocketmail.com on 12/28/10.
 *  Copyright 2010 astoj@rocketmail.com. All rights reserved.
 *
 */

#ifndef PIPER_PIPER_H
#define PIPER_PIPER_H

#include "Mathematics.h"
#include "../Math/Matrix.h"

#ifdef QT_BUILD // first version for Qt only Opengl ES 2.0
#include <GLES2/gl2.h>
#else
#if OPENGL_ES_2_0
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
extern int __OPENGLES_VERSION;
#else
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#endif
#endif


#ifdef OPENGL_ES_2_0
#ifdef GL_VERTEX_ARRAY
#undef GL_VERTEX_ARRAY
#undef GL_TEXTURE_COORD_ARRAY
#undef GL_NORMAL_ARRAY
#undef GL_COLOR_ARRAY
#endif
#define GL_VERTEX_ARRAY 0
#define GL_TEXTURE_COORD_ARRAY	1
#define GL_NORMAL_ARRAY	2
#define GL_COLOR_ARRAY 3
#define glEnableClientState(a) glEnableVertexAttribArray(a)
#define glDisableClientState(a) glDisableVertexAttribArray(a)
#define glVertexPointer(size,type,stride,pointer) glVertexAttribPointer(GL_VERTEX_ARRAY,size,type,GL_FALSE,stride,pointer)
#define glNormalPointer(type,stride,pointer) glVertexAttribPointer(GL_NORMAL_ARRAY,3,type,GL_FALSE,stride,pointer)
#define glTexCoordPointer(size,type,stride,pointer) glVertexAttribPointer(GL_TEXTURE_COORD_ARRAY,size,type,GL_FALSE,stride,pointer)
#define glColorPointer(size,type,stride,pointer) glVertexAttribPointer(GL_COLOR_ARRAY,size,type,GL_FALSE,stride,pointer)
#endif

/* Collecting and constructing input for graphics pipeline */
class Piper
{
public:
	enum Mode {
		PROJECTION = 0,
		MODEL,	
		TEXTURE
	};

	virtual ~Piper() {};
	
	virtual void setMatrix(const MATRIX &matrix, Mode mode) = 0;	
	virtual void pushMatrix(Mode mode) = 0;
	virtual void popMatrix(Mode mode) = 0;
	virtual void multMatrix(const MATRIX &matrix, Mode mode) = 0;
    virtual void multMatrix(const GLfloat * m, Mode mode);

	virtual void glDrawArrays (GLenum mode, GLint first, GLsizei count)
	{
		::glDrawArrays (mode, first, count);
	}
	
	virtual void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
	{
		::glDrawElements (mode, count, type, indices);
	}
	
	virtual void initFrame()
	{
	}

    virtual void glColor4f(	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = 0;
	
	/* instance needs to be created with initInstance() first. This could return 0 */
	static Piper *instance();

	void setViewMatrix(const MATRIX &matrix);
	
private:
	static Piper *_instance;
	/* fixedPipeline: 
	 *	true - OpenGL ES 1.1
	 *  false - OpenGL ES 2.0 and GLSL support
	 */
	static void initInstance(bool fixedPipeline = false);
	friend class CShell; // only has access to initInstance()
};

#endif //PIPER_PIPER_H
