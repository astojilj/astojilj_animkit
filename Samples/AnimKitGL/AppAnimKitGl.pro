INCLUDEPATH += ../../Dependencies/Utils
INCLUDEPATH += ../../Dependencies/vectormath
INCLUDEPATH += ../../Source
INCLUDEPATH += ../../Dependencies/FileTools/FileFormats/Blend
INCLUDEPATH += ../../Dependencies/FileTools/File

TEMPLATE = app
TARGET = animkitdemo

DEFINES += FBT_USE_GZ_FILE

unix:!symbian:exists($$QMAKE_INCDIR_X11/../GLES2/gl2.h) : {
    message(Using OPENGLES 20)
    DEFINES += QT_BUILD
    DEFINES += OPENGL_ES_2_0

    include(../../GLES20/Renderer/renderer.pri)
    include(../../GLES20/Math/math.pri)
    SOURCES += ../../GLES20/main.cpp
    INCLUDEPATH += ../../GLES20/Include
    LIBS += -lEGL -lGLESv2
} else {
    HEADERS +=     ../akGlutDemo.h
    SOURCES +=     ../akGlutDemo.cpp
    LIBS += -lglut -lGLEW -lGLU
}

# install
target.path = /usr/bin/

desktop.path = /usr/share/applications
desktop.files += animkitdemo.desktop

graphics.path = /usr/share/animkitdemo/
graphics.files += icon.png \
    Blu.blend

shaders.path = /usr/share/animkitdemo/
shaders.files += Toon.frag \
    Toon.vert

INSTALLS += target desktop graphics shaders


OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    ../../Source/cmake_install.cmake \
    ../../Source/CMakeLists.txt \
    ../../Dependencies/ZLib/algorithm.txt \
    ../../Dependencies/ZLib/configure \
    ../../Dependencies/ZLib/zlib.3 \
    ../../Dependencies/Utils/CMakeLists.txt \
    ../../Dependencies/Utils/libGameKitUtils.a \
    ../../Dependencies/Utils/Makefile \
    ../../Dependencies/FileTools/File/fbtScanner.l \
    Blu.blend

HEADERS += \
    ../../Source/akAnimationChannel.h \
    ../../Source/akAnimationClip.h \
    ../../Source/akAnimationCurve.h \
    ../../Source/akAnimationPlayer.h \
    ../../Source/akAnimationPlayerSet.h \
    ../../Source/akBufferInfo.h \
    ../../Source/akColor.h \
    ../../Source/akCommon.h \
    ../../Source/akDualQuat.h \
    ../../Source/akEuler.h \
    ../../Source/akGeometryDeformer.h \
    ../../Source/akMathUtils.h \
    ../../Source/akMesh.h \
    ../../Source/akMorphTarget.h \
    ../../Source/akPose.h \
    ../../Source/akPoseBlender.h \
    ../../Source/akSkeleton.h \
    ../../Source/akSkeletonPose.h \
    ../../Source/akTransformState.h \
    ../../Source/akTransitionBlender.h \
    ../../Source/akVertexGroup.h \
    ../../Source/btAlignedAllocator.h \
    ../../Source/btAlignedObjectArray.h \
    ../../Dependencies/ZLib/crc32.h \
    ../../Dependencies/ZLib/deflate.h \
    ../../Dependencies/ZLib/inffast.h \
    ../../Dependencies/ZLib/inffixed.h \
    ../../Dependencies/ZLib/inflate.h \
    ../../Dependencies/ZLib/inftrees.h \
    ../../Dependencies/ZLib/trees.h \
    ../../Dependencies/ZLib/zconf.h \
    ../../Dependencies/ZLib/zconf.in.h \
    ../../Dependencies/ZLib/zlib.h \
    ../../Dependencies/ZLib/zutil.h \
    ../../Dependencies/Utils/utCommon.h \
    ../../Dependencies/Utils/utMemoryPool.h \
    ../../Dependencies/Utils/utRandom.h \
    ../../Dependencies/Utils/utScript.h \
    ../../Dependencies/Utils/utSingleton.h \
    ../../Dependencies/Utils/utStreams.h \
    ../../Dependencies/Utils/utString.h \
    ../../Dependencies/Utils/utTypes.h \
    akDemo.h \
    ../../Dependencies/FileTools/FileFormats/Blend/Blender.h \
    ../../Dependencies/FileTools/FileFormats/Blend/fbtBlend.h \
    ../../Dependencies/FileTools/File/fbtBuilder.h \
    ../../Dependencies/FileTools/File/fbtConfig.h \
    ../../Dependencies/FileTools/File/fbtFile.h \
    ../../Dependencies/FileTools/File/fbtPlatformHeaders.h \
    ../../Dependencies/FileTools/File/fbtStreams.h \
    ../../Dependencies/FileTools/File/fbtTables.h \
    ../../Dependencies/FileTools/File/fbtTypes.h \
    ../akDemoBase.h \
    ../akEntity.h \
    ../BlenderLoader/akBLoader.h \
    ../BlenderLoader/akAnimationLoader.h \
    ../BlenderLoader/akMeshLoader.h \
    ../BlenderLoader/akTextureLoader.h

SOURCES += \
    ../../Source/akAnimationChannel.cpp \
    ../../Source/akAnimationClip.cpp \
    ../../Source/akAnimationCurve.cpp \
    ../../Source/akAnimationPlayer.cpp \
    ../../Source/akAnimationPlayerSet.cpp \
    ../../Source/akBufferInfo.cpp \
    ../../Source/akDualQuat.cpp \
    ../../Source/akGeometryDeformer.cpp \
    ../../Source/akMathUtils.cpp \
    ../../Source/akMesh.cpp \
    ../../Source/akMorphTarget.cpp \
    ../../Source/akPose.cpp \
    ../../Source/akPoseBlender.cpp \
    ../../Source/akSkeleton.cpp \
    ../../Source/akSkeletonPose.cpp \
    ../../Source/akTransitionBlender.cpp \
    ../../Source/akVertexGroup.cpp \
    ../../Source/btAlignedAllocator.cpp \
    ../../Dependencies/ZLib/adler32.c \
    ../../Dependencies/ZLib/compress.c \
    ../../Dependencies/ZLib/crc32.c \
    ../../Dependencies/ZLib/deflate.c \
    ../../Dependencies/ZLib/gzio.c \
    ../../Dependencies/ZLib/infback.c \
    ../../Dependencies/ZLib/inffast.c \
    ../../Dependencies/ZLib/inflate.c \
    ../../Dependencies/ZLib/inftrees.c \
    ../../Dependencies/ZLib/trees.c \
    ../../Dependencies/ZLib/uncompr.c \
    ../../Dependencies/ZLib/zutil.c \
    ../../Dependencies/Utils/utRandom.cpp \
    ../../Dependencies/Utils/utScript.cpp \
    ../../Dependencies/Utils/utStreams.cpp \
    ../../Dependencies/Utils/utString.cpp \
    akDemo.cpp \
    ../../Dependencies/FileTools/FileFormats/Blend/fbtBlend.cpp \
    ../../Dependencies/FileTools/File/fbtBuilder.cpp \
    ../../Dependencies/FileTools/File/fbtFile.cpp \
    ../../Dependencies/FileTools/File/fbtStreams.cpp \
    ../../Dependencies/FileTools/File/fbtTables.cpp \
    ../../Dependencies/FileTools/File/fbtTypes.cpp \
    ../../Dependencies/FileTools/File/Generated/fbtScanner.inl \
    ../akDemoBase.cpp \
    ../akEntity.cpp \
    ../BlenderLoader/akBLoader.cpp \
    ../BlenderLoader/akAnimationLoader.cpp \
    ../BlenderLoader/akMeshLoader.cpp \
    ../BlenderLoader/akTextureLoader.cpp \
    ../../Dependencies/FileTools/FileFormats/Blend/Generated/bfBlender.cpp
