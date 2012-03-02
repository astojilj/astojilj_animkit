/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#include "akDemo.h"

#include "../BlenderLoader/akBLoader.h"
#include "../akEntity.h"

#include "akMesh.h"
#include "akSkeletonPose.h"
#include "akGeometryDeformer.h"
#include "akAnimationPlayerSet.h"
#include "akAnimationEngine.h"
#include "akAnimatedObject.h"

#if defined __APPLE__ && defined OPENGL_ES_2_0
#include "Pathes.h"
#endif

akDemo::akDemo() : akDemoBase()
{

}

akDemo::~akDemo()
{
	
}

void akDemo::init(void)
{
	akBLoader loader(this, m_animengine);
#ifdef QT_BUILD
    loader.loadFile("/usr/share/animkitdemo/Blu.blend", false, true);
#else
    #if defined __APPLE__ && defined OPENGL_ES_2_0
		char absolutePath[512], bundleDir[512];
		GetResourcePathASCII(bundleDir,512);
		sprintf(absolutePath,"%s/%s",bundleDir,"columbine.blend");
//    		sprintf(absolutePath,"%s/%s",bundleDir,"Blu.blend");
		loader.loadFile(absolutePath, false, true);	
    #else
		loader.loadFile("Blu.blend", false, true);
    #endif
#endif
    //loader.loadFile("Sintel.blend", false, true);
	
	// Set some animation data the loader cannot detect
	akEntity* square = getEntity("Plane");
	if(square)
		square->setPositionAnimated(true);
	
    // Join the morph action and the rest action together.
	akAnimationClip* bluc = m_animengine->getAnimationClip("Rest");
	akAnimationClip* morphc = m_animengine->getAnimationClip("KeyAction");
	if(bluc && morphc)
	{
		akAnimationChannel* chan = morphc->getChannel("Key 1");
		if(chan)
		{
            morphc->removeChannel(chan);
            bluc->addChannel(chan);
		}
	}
/*
    // Joint mask test
	akEntity* blu = getEntity("Blu");
	if(blu)
	{
		akAnimationPlayer* play = blu->getAnimatedObject()->getAnimationPlayers()->getAnimationPlayer(0);
		akSkeleton* skel = blu->getSkeleton();
		if(play && skel)
		{
			UTuint32 bid = skel->getIndex("Arm.L");
			if(bid>0)
			{
				play->createJointMask(skel);
				play->setJointMaskWeight(bid, 0.5f);
			}
		}
	}
*/
}

#ifndef OPENGL_ES_2_0

int main(int argc, char** argv)
{
	akDemo* demo = new akDemo();
	
	startGlutDemo(argc, argv, demo,"AnimKit OpenGL Demo");
	
	delete demo;
	return 0;
}

#endif
