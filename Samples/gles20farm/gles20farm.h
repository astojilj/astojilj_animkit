/*
 *  gles20farm.cpp
 *  
 *  Created by astojilj@gmail.com on 01/29/12.
 *  Copyright 2012 astojilj@gmail.com. All rights reserved.
 *
 */

#ifndef GLES20FARM_H
#define GLES20FARM_H

#include "../akDemoBase.h"

class Gles20Farm : public akDemoBase
{
public:
	Gles20Farm();
	~Gles20Farm();
	
	virtual void init();
    void renderFrame();
    void draw();
    
private:
    void drawTextureToScreen();
    void drawBackground();
    void cleanup();
    
private:
    GLuint backgroundTexture;
};

#endif // GLES20FARM_H
