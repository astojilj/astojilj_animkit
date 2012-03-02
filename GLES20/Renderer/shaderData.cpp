/*
 *  ShaderData.cpp
 *  Shaders
 *
 *  Created by Jim on 8/1/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "shaderData.h"
#include "Pathes.h"
#include "ResourceFile.h"
#include "Shader.h"
#include <stdio.h>

#ifdef QT_BUILD
#include <GLES2/gl2.h>
#endif

// This ties in with the shader attribute to link to openGL, see pszVertShader.
const char* pszAttribs[] = { "a_Vertex", "a_MultiTexCoord0", "a_Normal", "a_Color", "a_BoneIndices", "a_BoneWeights" };

ShaderData::ShaderData( const char *vertShaderFilename, const char *fragShaderFilename )
{
	char buffer[2048];
	GetResourcePathASCII(buffer, 2048);
	
	CPVRTResourceFile::SetReadPath(buffer);
	
	/* Gets the Data Path */	
	if(ShaderLoadFromFile("blank",fragShaderFilename, GL_FRAGMENT_SHADER, 0, &uiFragShader) == 0)
		printf("Loading the fragment shader fails:%s", fragShaderFilename);
	if(ShaderLoadFromFile("blank", vertShaderFilename, GL_VERTEX_SHADER, 0, &uiVertShader) == 0)
		printf("Loading the vertex shader fails:%s", vertShaderFilename);
	
	CreateProgram(&uiProgramObject, uiVertShader, uiFragShader, pszAttribs, sizeof(pszAttribs)/sizeof(pszAttribs[0]) );	
	
	// First gets the location of that variable in the shader using its name
	PMVMatrixHandle = glGetUniformLocation(uiProgramObject, "u_ModelViewProjectionMatrix");
	TIMMatrixHandle = glGetUniformLocation(uiProgramObject, "u_TrasposeInverseModelMatrix");
	ViewMatrixHandle = glGetUniformLocation(uiProgramObject, "u_ViewMatrix");
	
	EyePositionHandle = glGetUniformLocation(uiProgramObject, "myEye");
	LightDirectionHandle = glGetUniformLocation(uiProgramObject, "light_direction");
	LightPositionHandle = glGetUniformLocation(uiProgramObject, "light_position");
	
	AmbientICHandle = glGetUniformLocation(uiProgramObject, "ambient_ic");
	DiffuseICHandle = glGetUniformLocation(uiProgramObject, "diffuse_ic");
	SpecularICHandle = glGetUniformLocation(uiProgramObject, "specular_ic");
	
	MaterialShininessHandle = glGetUniformLocation(uiProgramObject, "material_shininess");
	
	ColorHandle = glGetUniformLocation(uiProgramObject, "u_Color");		
	TextureHandle = glGetUniformLocation(uiProgramObject, "s_texture");			
	NormalMapHandle = glGetUniformLocation(uiProgramObject, "s_normalmap");	
    
    BoneCountHandle = glGetUniformLocation(uiProgramObject, "u_BoneCount");
    BoneMatricesHandle = glGetUniformLocation(uiProgramObject, "u_BoneMatrices[0]");
}

ShaderData::~ShaderData()
{
	glDeleteProgram(uiProgramObject);
	glDeleteShader(uiFragShader);
	glDeleteShader(uiVertShader);
}

