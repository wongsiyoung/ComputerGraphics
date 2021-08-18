#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QTimer *timer;
    float Hand,Arm,Calf,Lag;    // 存储下臂，上臂，小腿，大腿 当前旋转的角度
    float hangle, langle;        // 存储手臂和腿 的摆臂幅度
    float Body,BodyAngle;        // 存储机器人的当前旋转角度和每次旋转递增的角度  
};
#endif // MYGLWIDGET_H
