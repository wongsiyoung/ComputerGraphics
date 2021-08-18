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

    void keyPressEvent(QKeyEvent* e);           // ���̲���
    void mouseMoveEvent(QMouseEvent* e);        // ������
    void drawPlanet(Planet* planet);            // ���ƻ���̫������
    void drawPlanet2(Planet * p);               // ���ƴ���Ȧ����  
    void drawParticle(Planet* p);               // ������������
    void DrawEarthAndMoon(Planet *p1, Planet *p2);                              // �������������
    void drawSphere(double radius , int slices, int stack ,bool texture);       // ������
    void drawDisk(GLdouble innerRadius, GLdouble outerRadius, GLint slices);    // ���ƻ�
    void SetLight(bool lighting);               // ���ù���
    void SetMaterial(Planet* p);                // �����������
    void SetMaterialDefault();                  // �ָ���������
    void initVboVao();                          // ��ʼ�� vbo vao
    void createStoneArr();                      // ��ʼ�����Ǵ���������

private:
    QTimer *timer;
    Camera camera;          // ���������
    float lastX, lastY;     // ���λ��
    bool firstMouse;        // ����һ�ν��볡��
    bool lighting;          // �Ƿ����ù���
    Texture *boxtex;        // ������պ�������ͼ

    GLuint vboId[2];        
    GLuint vaoId;

    // ����
    Planet* sun;         // ̫��
    Planet* mercury;     // ˮ��
    Planet* venus;       // ����
    Planet* earth;       // ����
    Planet* moon;        // ����
    Planet* mars;        // ����
    Planet* jupiter;     // ľ��
    Planet* saturn;      // ����
    Planet* comet;       // ����
    Planet* uranus;      // ������
    Planet* neptune;     // ������
    Planet* pluto;       // ڤ����
    Planet* particle;    // ��������

    GLUquadricObj *quadObj = NULL;      // �����������
    
};
#endif // MYGLWIDGET_H
