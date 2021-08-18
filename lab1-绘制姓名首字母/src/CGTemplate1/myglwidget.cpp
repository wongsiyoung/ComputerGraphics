#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent),
	scene_id(0)
{
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
	if (scene_id==0) {
		scene_1();
	}
	else {
		scene_0();
	}
}

void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	//Press 0 or 1 to switch the scene
	if (e->key() == Qt::Key_0) {
		scene_id = 1;
		update();
	}
	else if (e->key() == Qt::Key_1) {
		scene_id = 0;
		update();
	}
}

void MyGLWidget::scene_0()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 100.0f, 0.0f, 100.0f, -1000.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50.0f, 50.0f, 0.0f);
	
	//draw a diagonal "I"
	glPushMatrix();
	glColor3f(0.839f, 0.153f, 0.157f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(-2.5f, -22.5f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(5.0f, 0.0f);
	glVertex2f(0.0f, 45.0f);

	glVertex2f(5.0f, 0.0f);
	glVertex2f(0.0f, 45.0f);
	glVertex2f(5.0f, 45.0f);

	glEnd();
	glPopMatrix();	
}

// 用 GL_TRIANGLES 实现
void MyGLWidget::scene_1()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 0.5*width(), 0.0f,0.5*height(), -1000.0f, 1000.0f);
	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.2 * width(), 0.2 * height(), 0.0f);

    //your implementation here, maybe you should write several glBegin
	glPushMatrix();
	//your implementation
	
	//draw H
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLES);
	glColor3f(0.2823f, 0.2392f, 0.5450f);

	glVertex2f(0.0f, 0.0f);
	glVertex2f(6.0f, 0.0f);
	glVertex2f(0.0f, 46.0f);

	
	glVertex2f(6.0f, 0.0f);
	glVertex2f(0.0f, 46.0f);
	glVertex2f(6.0f, 46.0f);

	
	glVertex2f(6.0f, 20.0f);
	glVertex2f(6.0f, 26.0f);
	glVertex2f(24.0f, 20.0f);

	glVertex2f(24.0f, 20.0f);
	glVertex2f(6.0f, 26.0f);
	glVertex2f(24.0f, 26.0f);

	
	glVertex2f(24.0f, 0.0f);
	glVertex2f(30.0f, 0.0f);
	glVertex2f(24.0f, 46.0f);

	glVertex2f(30.0f, 0.0f);
	glVertex2f(24.0f, 46.0f);
	glVertex2f(30.0f, 46.0f);

	glEnd();

	//draw S
	glBegin(GL_TRIANGLES);
	glColor3f(0.415f, 0.4529f, 0.803f);
	//draw -
	glVertex2f(36.0f, 0.0f);
	glVertex2f(36.0f, 6.0f);
	glVertex2f(66.0f, 0.0f);

	glVertex2f(66.0f, 6.0f);
	glVertex2f(36.0f, 6.0f);
	glVertex2f(66.0f, 0.0f);

	// |
	glVertex2f(36.0f, 26.0f);
	glVertex2f(42.0f, 26.0f);
	glVertex2f(36.0f, 40.0f);

	glVertex2f(42.0f, 26.0f);
	glVertex2f(42.0f, 40.0f);
	glVertex2f(36.0f, 40.0f);

	// -
	glVertex2f(36.0f, 20.0f);
	glVertex2f(36.0f, 26.0f);
	glVertex2f(66.0f, 20.0f);

	glVertex2f(66.0f, 20.0f);
	glVertex2f(36.0f, 26.0f);
	glVertex2f(66.0f, 26.0f);

	// |
	glVertex2f(60.0f, 6.0f);
	glVertex2f(66.0f, 6.0f);
	glVertex2f(60.0f, 20.0f);

	glVertex2f(60.0f, 20.0f);
	glVertex2f(66.0f, 20.0f);
	glVertex2f(66.0f, 6.0f);

	// -
	glVertex2f(36.0f, 40.0f);
	glVertex2f(36.0f, 46.0f);
	glVertex2f(66.0f, 40.0f);

	glVertex2f(66.0f, 40.0f);
	glVertex2f(36.0f, 46.0f);
	glVertex2f(66.0f, 46.0f);

	glEnd();
	
	//draw R
	glBegin(GL_TRIANGLES);
	glColor3f(0.482f, 0.407f, 0.933f);
	glVertex2f(72.0f, 0.0f);
	glVertex2f(78.0f, 0.0f);
	glVertex2f(72.0f, 46.0f);

	glVertex2f(72.0f, 46.0f);
	glVertex2f(78.0f, 0.0f);
	glVertex2f(78.0f, 46.0f);


	glVertex2f(102.0f, 40.0f);
	glVertex2f(102.0f, 46.0f);
	glVertex2f(78.0f, 46.0f);

	glVertex2f(102.0f, 40.0f);
	glVertex2f(78.0f, 40.0f);
	glVertex2f(78.0f, 46.0f);


	glVertex2f(102.0f, 40.0f);
	glVertex2f(102.0f, 26.0f);
	glVertex2f(96.0f, 40.0f);

	glVertex2f(96.0f, 26.0f);
	glVertex2f(102.0f, 26.0f);
	glVertex2f(96.0f, 40.0f);


	glVertex2f(82.0f, 23.0f);
	glVertex2f(102.0f, 0.0f);
	glVertex2f(94.0f, 0.0f);

	glVertex2f(94.0f, 0.0f);
	glVertex2f(74.0f, 23.0f);
	glVertex2f(82.0f, 23.0f);


	glVertex2f(102.0f, 20.0f);
	glVertex2f(102.0f, 26.0f);
	glVertex2f(78.0f, 26.0f);

	glVertex2f(102.0f, 20.0f);
	glVertex2f(78.0f, 20.0f);
	glVertex2f(78.0f, 26.0f);

	glEnd();

	glPopMatrix();
}
