/*
 *  piperGL20.cpp
 *  ReadBlend
 *
 *  Created by astoj@rocketmail.com on 12/28/10.
 *  Copyright 2010 astoj@rocketmail.com. All rights reserved.
 *
 */

#include <stdio.h>
#include "piperGL20.h"
#include "shaderData.h"

void PiperGL20::setMatrix(const MATRIX &matrix, Piper::Mode mode)
{  
	modeMatrix[mode].set(matrix);
}

void PiperGL20::pushMatrix(Piper::Mode mode)
{
	modeMatrix[mode].push();
}

void PiperGL20::popMatrix(Piper::Mode mode)
{
	modeMatrix[mode].pop();
}

void PiperGL20::multMatrix(const MATRIX &matrix, Mode mode)
{
	MATRIX result;
	MatrixMultiply(result, matrix, modeMatrix[mode].top());
	setMatrix(result, mode);
}

// copied from -> Oolong Engine2/Examples/Renderer/Tutorials/08 Shaders Demo (3DShaders.com)/Classes/Application.mm
// FIXME refactor
char *shaders_name[] = {
    (char *)"Toon"
};

int total_shaders = sizeof(shaders_name) / sizeof(shaders_name[0]);
int active_shader;

static GLfloat color[] = {0.f, 0.f, 0.f, 1.f};
static GLfloat previousSetColor[] = {0.f, 0.f, 0.f, 1.f};

// ^^ end of block copied from Oolong Engine2/Examples/Renderer/Tutorials/08 Shaders Demo (3DShaders.com)/Classes/Application.mm

class ShaderManager 
{
public:
	ShaderManager()
	{
		for (int i=0; i<total_shaders; i++) {
			char vertShaderName[64];
			char fragShaderName[64];
			
			sprintf( vertShaderName, "/%s.vert", shaders_name[i] );
			sprintf( fragShaderName, "/%s.frag", shaders_name[i] );
			shaders[i] = new ShaderData( vertShaderName, fragShaderName );
		}
        active_shader = 0;
	}
	
	~ShaderManager()
	{
		// Frees the OpenGL handles for the program and the 2 shaders
		for (int i=0; i<total_shaders; i++) {
			delete (shaders[i]);
		}
	}
	
	ShaderData *shaderAt(unsigned int index) const
	{
		if (index >= total_shaders) {
			return 0;
		}
		return shaders[index];
	}
	
	

private:
	// FIXME - once it becomes dynamic, need to realloc this
	ShaderData *shaders[16];
};


PiperGL20::PiperGL20() :
	shaderManager(0)
{
	init();
}

void PiperGL20::init() 
{
	shaderManager = new ShaderManager;
}

void PiperGL20::initFrame()
{
	ShaderData *shader = shaderManager->shaderAt(active_shader);
    glUseProgram(shader->uiProgramObject);
}

PiperGL20::~PiperGL20()
{
	delete shaderManager;
}

/*
 * The idea is to push all the prepared on client side to pipeline before draw():
 * i.e. this needs to be called just before drawing
 * Delivers current matrix stack snapshot to shader (through matrix uniforms @sa vert and frag shader programs),
 * updates uniform and vertex variables used by shader so that shader has all needed info
 * before draw method (following this).
 */
void PiperGL20::setupChangedVariables()
{
	MATRIX mView;
	MatrixIdentity(mView);
	const MATRIX &mViewProjection = modeMatrix[Piper::PROJECTION].top();
	const MATRIX &mModel = modeMatrix[Piper::MODEL].top();
	const MATRIX &mModelView = mModel;
	
	//  not used - view is always identity
	//	MatrixMultiply(mViewProjection, mView, modeMatrix[Piper::PROJECTION].top());
	//	MatrixMultiply(mModelView, mModel, mView);
	
	// Calculate model view projection matrix
	MATRIX mMVP;
	MatrixMultiply(mMVP, mModel, mViewProjection);
	
	MATRIX mTIM;
	MatrixInverse(mTIM, mModel);
	MatrixTranspose(mTIM, mTIM);
	
	ShaderData *shader = shaderManager->shaderAt(active_shader);
	// Then passes the matrix to that variable
	glUniformMatrix4fv( shader->PMVMatrixHandle, 1, GL_FALSE, mMVP.f);
	glUniformMatrix4fv( shader->MVMatrixHandle, 1, GL_FALSE, mModelView.f);
	glUniformMatrix4fv( shader->ModelMatrixHandle, 1, GL_FALSE, mModel.f);
	glUniformMatrix4fv( shader->ViewMatrixHandle, 1, GL_FALSE, mView.f);
	glUniformMatrix4fv( shader->TIMMatrixHandle, 1, GL_FALSE, mTIM.f);

    if (memcmp(color, previousSetColor, 4 * sizeof(GLfloat))) {
        glUniform4f(shader->ColorHandle, color[0], color[1], color[2], color[3]);
        memcpy(previousSetColor, color, 4 * sizeof(GLfloat));
    }
}

void PiperGL20::glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
	setupChangedVariables();
	::glDrawArrays (mode, first, count);
}

void PiperGL20::glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
	setupChangedVariables();
        if (count > 34000)
            return;
	::glDrawElements (mode, count, type, indices);
}

void PiperGL20::glColor4f(	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;
}
