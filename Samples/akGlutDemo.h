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

#ifndef AKGLUTDEMO_H
#define AKGLUTDEMO_H

#ifdef WIN32
#include <Windows.h>
#endif

#ifndef OPENGL_ES_2_0
#if defined(__APPLE__)
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif
#else

    #ifdef QT_BUILD
        #include <GLES2/gl2.h>
    #else
        #if __IPHONE_OS_VERSION_MAX_ALLOWED >= 30000
            #define BUILD_OPENGLES_2_0
            #include <OpenGLES/ES2/gl.h>
            #include <OpenGLES/ES2/glext.h>
        #endif
    #endif
#endif

class akDemoBase;

int startGlutDemo(int argc, char** argv, akDemoBase* demo, const char* title);

#endif // AKGLUTDEMO_H
