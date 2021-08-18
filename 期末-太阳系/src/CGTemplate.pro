QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11


INCLUDEPATH += "D:\tools\OpenGL\glew-2.1.0-win32\glew-2.1.0\include"

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += \
	Glu32.lib \
	OpenGL32.lib \

SOURCES += \
    main.cpp \
    myglwidget.cpp \
    utils/texture.cpp \
    GL/lodepng.cpp     \

HEADERS += \
    myglwidget.h \
    utils/texture.h \
    base.h \
    GL/glut.h \
    GL/lodepng.h \
    GL/glew.h \
    utils/vec3.h    \
    utils/camera.h  \
    utils/skybox.h  \
    utils/planet.h