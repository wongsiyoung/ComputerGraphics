#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include "utils/planet.h"
#include "utils/camera.h"
#include "utils/vec3.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    MyGLWidget(QWidget* parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void keyPressEvent(QKeyEvent* e);           // 键盘操作
    void mouseMoveEvent(QMouseEvent* e);        // 鼠标操作
    void drawPlanet(Planet* planet);            // 绘制环绕太阳星球
    void drawPlanet2(Planet * p);               // 绘制带光圈星球  
    void drawParticle(Planet* p);               // 绘制流行粒子
    void DrawEarthAndMoon(Planet *p1, Planet *p2);                              // 绘制类地月星球
    void drawSphere(double radius , int slices, int stack ,bool texture);       // 绘制球
    void drawDisk(GLdouble innerRadius, GLdouble outerRadius, GLint slices);    // 绘制环
    void SetLight(bool lighting);               // 设置光照
    void SetMaterial(Planet* p);                // 设置物体材质
    void SetMaterialDefault();                  // 恢复场景设置
    void initVboVao();                          // 初始化 vbo vao
    void createStoneArr();                      // 初始化行星带粒子属性

private:
    QTimer *timer;
    Camera camera;          // 场景摄像机
    float lastX, lastY;     // 鼠标位置
    bool firstMouse;        // 鼠标第一次进入场景
    bool lighting;          // 是否设置光照
    Texture *boxtex;        // 球行天空盒纹理贴图

    GLuint vboId[2];        
    GLuint vaoId;

    // 星球
    Planet* sun;         // 太阳
    Planet* mercury;     // 水星
    Planet* venus;       // 金星
    Planet* earth;       // 地球
    Planet* moon;        // 月球
    Planet* mars;        // 火星
    Planet* jupiter;     // 木星
    Planet* saturn;      // 土星
    Planet* comet;       // 彗星
    Planet* uranus;      // 天王星
    Planet* neptune;     // 海王星
    Planet* pluto;       // 冥王星
    Planet* particle;    // 流行粒子

    GLUquadricObj *quadObj = NULL;      // 二次曲面对象
    
};
#endif // MYGLWIDGET_H
