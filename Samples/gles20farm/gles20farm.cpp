/*
 *  gles20farm.cpp
 *  
 *  Created by astojilj@gmail.com on 01/29/12.
 *  Copyright 2012 astojilj@gmail.com. All rights reserved.
 *
 */

#include "gles20farm.h"

#include "../BlenderLoader/akBLoader.h"
#include "../akEntity.h"

#include "akMesh.h"
#include "akSkeletonPose.h"
#include "akGeometryDeformer.h"
#include "akAnimationPlayerSet.h"
#include "akAnimationEngine.h"
#include "akAnimatedObject.h"
#include "Timer.h"
#include "stdio.h"
#include "piper.h"
float DisplayHeight = 640;
float DisplayWidth = 480;
#include "Pathes.h"
#include <sstream>


Gles20Farm::Gles20Farm() 
: akDemoBase()
{

}

Gles20Farm::~Gles20Farm()
{
	
}

void Gles20Farm::init(void)
{
	akBLoader loader(this, m_animengine);
#ifdef QT_BUILD
    loader.loadFile("/usr/share/gles20farm/columbine.blend", false, true);
#else
    #if defined __APPLE__ && defined OPENGL_ES_2_0
		char absolutePath[512], bundleDir[512];
		GetResourcePathASCII(bundleDir,512);
		sprintf(absolutePath,"%s/%s",bundleDir,"columbine.blend");
		loader.loadFile(absolutePath, false, true);	
    #else
		loader.loadFile("columbine.blend", false, true);
    #endif
#endif
    // Set some animation data the loader cannot detect
	akEntity* square = getEntity("Plane.001");
	if(square)
		square->setPositionAnimated(true);
	square = getEntity("Plane.002");
	if(square)
		square->setPositionAnimated(true);
	square = getEntity("Plane.003");
	if(square)
		square->setPositionAnimated(true);
}

static unsigned int elapsedMS()
{
    // elapsed time since last simulation step. used when calling physics.stepSimulation
    static ElapsedTimer elapsedTimer;
    return 1000 * elapsedTimer.elapsed();
}


void Gles20Farm::renderFrame()
{
	//avoid steping time for the 1rst frame
	if(m_stepLastTime == 0.f)
	{
		m_stepLastTime = elapsedMS();
		m_fpsLastTime = m_stepLastTime;
	}
	
	m_time = elapsedMS();
	// fps
	m_frame++;
	if (m_time - m_fpsLastTime > 1000) {
		m_lastfps = m_frame*1000.0f/(m_time-m_fpsLastTime);
		m_fpsLastTime = m_time;
		m_frame = 0;
	}
	
	// step time
	akScalar dt;
	dt = (m_time - m_stepLastTime)/1000.f;
	m_stepLastTime = m_time;
	
	//avoid too big steps while debugging
	if(dt >= 0.2)
		dt = 0.016666;
	
    // move the columbine
    akEntity *columbine = getEntity("columbine");
    if (columbine) {
        static float linearspeed = 0.1f;
        akTransformState state = columbine->getTransformState();
        akVector3 forwardVector = rotate(normalize(state.rot), akVector3(0, -1.0f, 0));
        forwardVector = linearspeed * dt * normalize(forwardVector);
        state.loc = state.loc + forwardVector;
        state.rot = state.rot * akQuat::rotationZ(-dt*0.1f);
        columbine->setTransformState(state);
    }
    
    step(dt);
	draw();
}


void Gles20Farm::draw()
{
    Piper::instance()->initFrame();
    glClearColor(0.56f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set the OpenGL projection matrix
    MATRIX	MyPerspMatrix;
	
    MatrixPerspectiveFovRH(MyPerspMatrix, m_camera->m_fov, f2vt((DisplayWidth / DisplayHeight)), m_camera->m_clipStart, m_camera->m_clipEnd, (DisplayHeight > DisplayWidth));
    Piper::instance()->setMatrix(MyPerspMatrix, Piper::PROJECTION);
    
    
    MATRIX model1;
    akMatrix4 cam_m = m_camera->m_transform.toMatrix();
    MATRIX camM((GLfloat*)&cam_m);
    
    MatrixInverse(model1, camM);
    Piper::instance()->setMatrix(model1, Piper::MODEL);
    
	// objects
	bool shaded = m_shaded && !m_wireframe;
	unsigned int i;
	for( i=0; i<m_objects.size(); i++)
	{
		m_objects.at(i)->draw(m_drawNormals, m_drawColor, m_textured, m_useVbo, shaded, m_drawSkeleton);
	}
    
	// Stats
	std::stringstream UIString;
	UIString << "FPS: " << getFps() << "\n\n";
	UIString << "Meshes: " << m_meshCount <<"\n";
	UIString << "Submeshes: " << m_subCount <<"\n";
	UIString << "Triangles: " << m_triCount <<"\n";
	UIString << "Vertices: " << m_vertexCount <<"\n";
	utString str = UIString.str();
    static int lastfps = getFps();
    if (getFps() != lastfps) {
        lastfps = getFps();
        printf("fps = %d", lastfps);
    }
	drawString(10, 15, str.c_str());
	
	// Info text
	str.clear();
	
	str += "P: Togle draw pose (";
	str += (m_drawSkeleton? "ON":"OFF");
	str += ")\n";
	
	str += "W: Togle wireframe (";
	str += (m_wireframe? "ON":"OFF");
	str += ")\n";
	
	str += "S: Togle shading (";
	str += (m_shaded? "ON":"OFF");
	str += ")\n";
	
	str += "T: Togle textures (";
	str += (m_textured? "ON":"OFF");
	str += ")\n";
	
	str += "C: Togle vertex color (";
	str += (m_drawColor? "ON":"OFF");
	str += ")\n";
	
	str += "N: Togle draw normals (";
	str += (m_drawNormals? "ON":"OFF");
	str += ")\n";
	
	str += "D: Cycle dual quaternion skinning (";
	switch(m_dualQuatUse)
	{
        case 0:
            str += "File setting";
            break;
        case 1:
            str += "Matrix";
            break;
        case 2:
            str += "Dual Quat";
            break;
        case 3:
            str += "Dual Quat Antiposality";
            break;
	}
	str += ")\n";
	
	str += "M: Cycle normal skinning (";
	switch(m_normalMethod)
	{
        case 0:
            str += "No normal skinning";
            break;
        case 1:
            str += "No scaling";
            break;
        case 2:
            str += "Uniform scaling";
            break;
        case 3:
            str += "Full";
            break;
	}
	str += ")\n";
	
	if(m_canUseVbo)
	{
		str += "V: Togle VBO (";
		str += (m_useVbo? "ON":"OFF");
		str += ")\n";
	}
	drawString(10, m_windowy-100, str.c_str());
}
