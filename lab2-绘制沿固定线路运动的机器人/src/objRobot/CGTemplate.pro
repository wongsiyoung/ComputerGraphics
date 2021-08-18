QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11

INCLUDEPATH += "D:\tools\glew-2.1.0-win32\glew-2.1.0\include"

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += \
	Glu32.lib \
	OpenGL32.lib
LIBS += glew32.lib

SOURCES += \
    main.cpp \
    myglwidget.cpp

HEADERS += \
    myglwidget.h
    robot.h