#include "myglwidget.h"
#include "robot.h"

ObjLoader robot = ObjLoader("file.obj");	// 实例化 ObjLoader

/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
	timer = new QTimer(this); // 实例化一个定时器
	timer->start(16); // 时间间隔设置为16ms，可以根据需要调整
	connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图
	Body = 0.0f, BodyAngle = 0.2f; 		// 初始化 Body 角度和每次递增的度数
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
	delete this->timer;
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());  
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
	glDisable(GL_DEPTH_TEST);             
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
	
	glEnable(GL_DEPTH_TEST);	// 使得被挡住的部分看不到

	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	gluPerspective(120.0f, width() / height(), 0.1f, 100.0f);	// 放置摄像机

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0f, -30.0f, -60.0f);		// 将机器人初始位置置于沿 z 轴负方向平移 60 个单位，沿 y 轴负方向平移 30 个单位
	glRotatef(-Body, 0, 1, 0);				// 绕 y 轴顺时针旋转 Body° 

	glColor3f(1.0f, 0.0f, 0.0f);			// 颜色全红
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 30.0f);	// 设置 robot 旋转半径 
		glRotatef(-90, 0, 1, 0);			// 将 robot 朝向 x 轴负方向
		robot.Draw();						// 绘制 robot
	glPopMatrix();

	Body += BodyAngle;						// 递增robot的旋转角度
}


/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

