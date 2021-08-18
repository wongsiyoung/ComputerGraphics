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
    
    void createSphere(GLfloat* sphere, GLuint longitude, GLuint latitude);
    void initShader(const char *vertexPath, const char *fragmentPath,unsigned int *ID);
    void initVboVao();
    void Draw();

private:
    QTimer *timer;
    unsigned int program;
    unsigned int lightId;
    float angle;
    float ballAngle;
    float body;
    // 你可以将你的shader或者vbo声明为私有变量
    // GLuint vertexShader;
    // GLuint fragmentShader;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint lightVertShader;
    GLuint lightFragShader;
    GLuint vboId;
    GLuint vaoId;
    GLuint lampVAOId;
    GLuint lampVBOId;
};
#endif // MYGLWIDGET_H
