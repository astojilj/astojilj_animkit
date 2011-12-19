#include <QCoreApplication>
#include  <iostream>
using namespace std;

#include  <cmath>
#include  <sys/time.h>

#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>

#include  <GLES2/gl2.h>
#include  <EGL/egl.h>

#include <stdlib.h>
#include <string.h>
#include "piper.h"
#include "akDemo.h"

//! support only GLES 2.0 on MeeGo
int __OPENGLES_VERSION = 2;

const char *APPLICATION_RESOURCES_INSTALL_DIR = "/usr/share/animkitdemo/";

void GetResourcePathASCII(char* cBuffer, int iLength)
{
    assert(iLength > strlen(APPLICATION_RESOURCES_INSTALL_DIR));
    strcpy(cBuffer, APPLICATION_RESOURCES_INSTALL_DIR);
}

#define LOG_ERROR(a) cerr << __FILE__ << ":" << __LINE__ << " "<< a << endl;

Display    *displayX;
Window      win;
EGLDisplay  eglDisplay;
EGLContext  eglContext;

GLfloat
norm_x    =  0.0,
norm_y    =  0.0,
offset_x  =  0.0,
offset_y  =  0.0,
p1_pos_x  =  0.0,
p1_pos_y  =  0.0;

GLint
texture_loc,
aTextCoord_loc,
offset_loc,
position_loc;


EGLSurface  eglSurface;
bool        update_pos = false;

const float vertexArray[] = {
    0.0,  0.5,  0.0,
    -0.5,  0.0,  0.0,
    0.0, -0.5,  0.0,
    0.5,  0.0,  0.0,
    0.0,  0.5,  0.0
};

const float textureArray[] = {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.1, 0.0, 1.0, 1.0
};

extern float WindowHeight;
extern float WindowWidth;

akDemo* demo;

class CShell
{
public:
    static void init()
    {
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 30000
    if( __OPENGLES_VERSION >= 2 ) {
        Piper::initInstance(false);
    } else
#elif defined(QT_BUILD)
    Piper::initInstance(false);
#else
    {
        Piper::initInstance(true);
    }
#endif
        demo = new akDemo();
        demo->start();
    }
};

int main(int argc, char ** argv)
{
    // Qt code expects the instance
    QCoreApplication theApp(argc, argv);

    displayX = XOpenDisplay(NULL);
    if (!displayX) {
        LOG_ERROR("Exiting,... XOpenDisplay(NULL) failed.")
        return 1;
    }

    const int window_width  = XDisplayWidth(displayX, 0);
    const int window_height = XDisplayHeight(displayX, 0);
    WindowHeight = window_height;
    WindowWidth = window_width;

    eglDisplay = eglGetDisplay((EGLNativeDisplayType)displayX);
    if (eglDisplay == EGL_NO_DISPLAY) {
        LOG_ERROR("Exiting,... eglGetDisplay((EGLNativeDisplayType)displayX) failed.")
        return 1;
    }

    if (!eglInitialize(eglDisplay, NULL, NULL)) {
        LOG_ERROR("Exiting,... eglInitialize failed.")
        return 1;
    }

    Window root = DefaultRootWindow(displayX);

    XSetWindowAttributes  swa;
    swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;

    win = XCreateWindow(displayX, root,
                         0, 0, window_width, window_height, 0,
                         CopyFromParent, InputOutput,
                         CopyFromParent, CWEventMask,
                         &swa);

    XSetWindowAttributes xattr;
    Atom atom;

    xattr.override_redirect = false;
    XChangeWindowAttributes(displayX, win, CWOverrideRedirect, &xattr);

    atom = XInternAtom (displayX, "_NET_WM_STATE_FULLSCREEN", true);
    XChangeProperty (displayX, win,
                     XInternAtom (displayX, "_NET_WM_STATE", false),
                     XA_ATOM,  32,  PropModeReplace,
                     (unsigned char*) &atom, 1);

    XWMHints hints;
    hints.input = true;
    hints.flags = InputHint;
    XSetWMHints(displayX, win, &hints);

    XMapWindow(displayX , win);             // make the window visible on the screen
    XStoreName(displayX , win , "ReadBlend"); // give the window a name

    //// get identifiers for the provided atom name strings
    Atom wm_state   = XInternAtom(displayX, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(displayX, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));

    xev.type                 = ClientMessage;
    xev.xclient.window       = win;
    xev.xclient.message_type = wm_state;
    xev.xclient.format       = 32;
    xev.xclient.data.l[0]    = 1;
    xev.xclient.data.l[1]    = fullscreen;
    XSendEvent(               // send an event mask to the X-server
                                displayX,
                                DefaultRootWindow(displayX),
                                False,
                                SubstructureNotifyMask,
                                &xev);


    ///////  the egl part  //////////////////////////////////////////////////////////////////
    //  egl provides an interface to connect the graphics related functionality of openGL ES
    //  with the windowing interface and functionality of the native operation system (X11
    //  in our case.

    EGLint attr[] = {       // some attributes to set up our egl-interface
                            EGL_BUFFER_SIZE, 16,
                            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                            EGL_DEPTH_SIZE, 24,
                            EGL_NONE
                    };

    EGLConfig  ecfg;
    EGLint     num_config;
    if(!eglChooseConfig( eglDisplay, attr, &ecfg, 1, &num_config)) {
        cerr << "Failed to choose config (eglError: " << eglGetError() << ")" << endl;
        return 1;
    }

    if(num_config != 1) {
        cerr << "Didn't get exactly one config, but " << num_config << endl;
        return 1;
    }


    eglSurface = eglCreateWindowSurface(eglDisplay, ecfg, (EGLNativeWindowType)win, NULL);
    if(eglSurface == EGL_NO_SURFACE) {
        cerr << "Unable to create EGL surface (eglError: " << eglGetError() << ")" << endl;
        return 1;
    }


    //// egl-contexts collect all state descriptions needed required for operation
    EGLint ctxattr[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    eglContext = eglCreateContext(eglDisplay, ecfg, EGL_NO_CONTEXT, ctxattr);
    if(eglContext == EGL_NO_CONTEXT) {
        cerr << "Unable to create EGL context (eglError: " << eglGetError() << ")" << endl;
        return 1;
    }

    //// associate the egl-context with the egl-surface
    eglMakeCurrent( eglDisplay, eglSurface, eglSurface, eglContext);

    glViewport(0 , 0 , window_width, window_height);
    glScissor(0, 0, window_width, window_height);
    glClearColor(0.08 , 0.06 , 0.07 , 1.);    // background color

    glEnable(GL_DEPTH_TEST);

    CShell::init();

    struct  timezone  tz;
    timeval  t1, t2;
    gettimeofday(&t1 , &tz);
    int  num_frames = 0;

    bool quit = false;
    while(!quit) {    // the main loop

        while(XPending(displayX)) {   // check for events from the x-server

            XEvent  xev;
            XNextEvent( displayX, &xev);

            if(xev.type == MotionNotify) {  // if mouse has moved
                //            cout << "move to: << xev.xmotion.x << "," << xev.xmotion.y << endl;
                GLfloat window_y  =  (window_height - xev.xmotion.y) - window_height / 2.0;
                norm_y            =  window_y / (window_height / 2.0);
                GLfloat window_x  =  xev.xmotion.x - window_width / 2.0;
                norm_x            =  window_x / (window_width / 2.0);
                update_pos = true;
            }

            if(xev.type == KeyPress)   quit = true;
        }

        // render
        demo->idleCallback();

        eglSwapBuffers(eglDisplay, eglSurface);
                // frames per second count calculation
                if(++num_frames % 60 == 0) {
                    gettimeofday( &t2, &tz);
                    float dt  =  t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6;
                    cout << "fps: " << num_frames / dt << endl;
                    num_frames = 0;
                    t1 = t2;
                }

    }


    ////  cleaning up...
    delete demo;
    demo = 0;

    eglDestroyContext(eglDisplay, eglContext);
    eglDestroySurface(eglDisplay, eglSurface);
    eglTerminate     (eglDisplay);
    XDestroyWindow   (displayX, win);
    XCloseDisplay    (displayX);

    return 0;
}
