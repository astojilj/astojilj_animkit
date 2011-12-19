INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/Resource
INCLUDEPATH += $$PWD/Core/GraphicsDevice
INCLUDEPATH += $$PWD/Core/UI

RENDERER_ROOT = ../../GLES20/Renderer
#RENDERER_ROOT = $$PWD

SOURCES += \
    $$RENDERER_ROOT/shaderData.cpp \
    $$RENDERER_ROOT/piper_p.cpp \
    $$RENDERER_ROOT/piperGL20.cpp \
    $$RENDERER_ROOT/piperGL11.cpp\
    $$RENDERER_ROOT/Core/GraphicsDevice\Shader.cpp \
    $$RENDERER_ROOT/Resource\ResourceFile.cpp \
    $$RENDERER_ROOT/Core/UI/DisplayText.cpp \
    $$RENDERER_ROOT/Core/UI/DisplayTextAPI.cpp \

HEADERS +=  \
    $$RENDERER_ROOT/shaderData.h \
    $$RENDERER_ROOT/piper_p.h \
    $$RENDERER_ROOT/piperGL20.h \
    $$RENDERER_ROOT/piperGL11.h \
    $$RENDERER_ROOT/piper.h \
    $$RENDERER_ROOT/Core/GraphicsDevice\Shader.h \
    $$RENDERER_ROOT/Resource\ResourceFile.h \
    $$RENDERER_ROOT/Core/UI/DisplayText.h \
    $$RENDERER_ROOT/Core/UI/DisplayTextdat.h



