MATH_SRC_PATH = ../../GLES20/Math
#MATH_SRC_PATH = $$PWD

INCLUDEPATH += $$MATH_SRC_PATH
INCLUDEPATH += $$MATH_SRC_PATH/../../Include

HEADERS += \
    $$MATH_SRC_PATH/Vector.h \
    $$MATH_SRC_PATH/Transform.h \
    $$MATH_SRC_PATH/Random.h \
    $$MATH_SRC_PATH/Quaternion.h \
    $$MATH_SRC_PATH/Matrix.h \
    $$MATH_SRC_PATH/Endian.h \
    $$MATH_SRC_PATH/neonmath/neon_matrix_impl.h \
    $$MATH_SRC_PATH/vfpmath/vfp_clobbers.h \
    $$MATH_SRC_PATH/vfpmath/utility_impl.h \
    $$MATH_SRC_PATH/vfpmath/matrix_impl.h \
    $$MATH_SRC_PATH/vfpmath/common_macros.h

SOURCES += \
    $$MATH_SRC_PATH/Vector.cpp \
    $$MATH_SRC_PATH/Transform.cpp \
    $$MATH_SRC_PATH/Random.cpp \
    $$MATH_SRC_PATH/Matrix.cpp \
    $$MATH_SRC_PATH/neonmath/neon_matrix_impl.cpp \
    $$MATH_SRC_PATH/vfpmath/utility_impl.cpp \
    $$MATH_SRC_PATH/vfpmath/matrix_impl.cpp



