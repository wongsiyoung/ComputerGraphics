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

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    
    void createSphere1(GLfloat* sphere, GLuint longitude, GLuint latitude);                 // 球的顶点生成函数，非索引数组存储方式
    void createSphere2(GLfloat* sphere, GLuint longitude, GLuint latitude);                 // 球的顶点生成函数，索引数组存储方式
    void initShader(const char *vertexPath, const char *fragmentPath,unsigned int *ID);
    void initShaderVariables();            // 初始化着色器变量
    void initVboVao();                     // 初始化 vao、vbo 的绑定
    void DrawWithVBO();                    // 使用 vbo 方式绘制
    void DrawWithoutVBO();                 // 不使用 vbo 方式绘制
    void DrawIndex();                      // 使用 index array 方式绘制

private:
    QTimer *timer;
    unsigned int program;
    float angle;                        // 漩涡度数 
    float ballAngle;                    // 漩涡增加的角度
    float body;                         // 球的旋转度数
    // 你可以将你的shader或者vbo声明为私有变量
    // GLuint vertexShader;
    // GLuint fragmentShader;
    GLuint vertexShader;                // 顶点着色器
    GLuint fragmentShader;              // 片元着色器
    GLuint vboId;
    GLuint vaoId;

    int mode;                           // 绘制模式
};
#endif // MYGLWIDGET_H
