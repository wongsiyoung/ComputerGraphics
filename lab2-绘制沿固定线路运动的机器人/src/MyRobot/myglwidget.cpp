#include "myglwidget.h"


/*###################################################
##  函数: DrawCube
##  函数描述： 绘制单位正方体，中心在(0,0,0)
##  参数描述：
##   无
#####################################################*/
void DrawCube() {
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glEnd();
}

/*###################################################
##  函数: DrawElement
##  函数描述： 绘制以单位正方体为基础部件的元素
##  参数描述：
##   tx,ty,tz: 物体相在物体坐标原点的位置
##   sx,sy,sz: 物体的长、宽、高
##   angle,rx,ry,rz: 物体的旋转角度和旋转轴
##   cx,cy,cz: 物体的颜色
#####################################################*/
void DrawElement(float tx,float ty,float tz,float sx,float sy,float sz,float angle,float rx,float ry,float rz,float cx,float cy,float cz) {
	glPushMatrix();
	glColor3f(cx, cy, cz);   
	glTranslatef(tx, ty, tz);
	glRotatef(angle, rx, ry, rz);
	glScalef(sx, sy, sz);   
	DrawCube();
	glPopMatrix();
}


/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

MyGLWidget::MyGLWidget(QWidget* parent)
	:QOpenGLWidget(parent)
{
	timer = new QTimer(this); // 实例化一个定时器
	timer->start(16); // 时间间隔设置为16ms，可以根据需要调整
	connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图
	Arm = Lag = Calf = Hand = 0.0f;		// 初始化手臂和腿的度数
	hangle = 0.7f, langle = 0.7f;		// 初始化手臂和腿的摆动幅度
	Body = 0.0f, BodyAngle = 0.2f;		// 初始化机器热的旋转度数和旋转递增度数
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
	
	glEnable(GL_DEPTH_TEST);	// 被挡住的看不到
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0f, width() / height(), 0.1f, 100.0f);		// 放置摄像机

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -35.0f);		// 将机器人初始位置置于沿 z 轴负方向平移 35 个单位
	glRotatef(-Body, 0, 1, 0);				// 绕 y 轴顺时针旋转 Body°

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 15.0f);		// 设置机器人的旋转半径
	glRotatef(-90, 0, 1, 0);				// 将机器人始终朝向 x 轴负方向
	
	// 绘制body， 传入 位置坐标，      size 为 5*7*5       不旋转                    颜色为蓝色
	DrawElement(0.0f, 3.5f, 0.0f,    5.0f, 7.0f, 5.0f,   0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f);

	// 绘制head  传入 位置坐标        size 为 2*2*2        不选择                    颜色为红色
	DrawElement(0.0f, 8.5f, 0.0f,    2.0f, 2.0f, 2.0f,   0.0f, 0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f);

	// 绘制shoulder  传入位置坐标      size 为 2*2*3       不旋转                    颜色为蓝色
	DrawElement(-3.0f, 6.0f, 0.0f,   2.0f, 2.0f, 3.0f,   0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f);		// 左肩膀
	DrawElement(3.0f, 6.0f, 0.0f,    2.0f, 2.0f, 3.0f,   0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f);       // 右肩膀
	
	// 绘制左上臂	传入位置坐标       size 为 1*3*1       摆臂，绕 x 轴旋转 Arm°      颜色为红色
	DrawElement(-3.0f, 4.0f, 0.0f,   1.0f, 3.0f, 1.0f,   Arm,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f);

	// 绘制左下臂
	glPushMatrix();
		glTranslatef(-3.0f, 4.0f, 0.0f);		// 将左下臂平移到左上臂
		glRotatef(Hand, 1, 0, 0);				// 左下臂绕着左上臂旋转 Hand°
		glTranslatef(3.0f, -4.0f, 0.0f);		// 平移回去

		//          	传入位置坐标       size 为 1*2*1       不旋转                  颜色为红色
		DrawElement(-3.0f, 1.5f, 0.0f,   1.0f, 2.0f, 1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f);
	glPopMatrix();
		
	// 绘制右上臂   传入位置坐标       size 为 1*3*1       摆臂，绕 x 轴旋转 -Arm°      颜色为红色
	DrawElement(3.0f, 4.0f, 0.0f,    1.0f, 3.0f, 1.0f,  -Arm, 1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f);

	// 绘制右下臂
	glPushMatrix();
		glTranslatef(3.0f, 4.0f, 0.0f);			// 将右下臂平移到右上臂
		glRotatef(-Hand, 1, 0, 0);				// 右下臂绕着右上臂旋转 -Hand°
		glTranslatef(-3.0f, -4.0f, 0.0f);		// 平移回去

		//          	传入位置坐标       size 为 1*2*1       不旋转                  颜色为红色
		DrawElement(3.0f, 1.5f, 0.0f,   1.0f, 2.0f, 1.0f,  0.0f, 0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f);
	glPopMatrix();

	// 绘制左上腿
	glPushMatrix();
		glRotatef(Lag, 1.0f, 0.0f, 0.0f);		// 绕着 x 轴旋转 Lag°

		//          	传入位置坐标       size 为 1*4*1       不旋转                  颜色为红色
		DrawElement(-1.5f, -1.5f, 0.0f,   1.0f, 4.0f, 1.0,   0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);	
	glPopMatrix();

	// 绘制左下腿
	glPushMatrix();
		glRotatef(Calf, 1.0f, 0.0f, 0.0f); 		// 绕着 x 轴旋转 Calf°

		//          	传入位置坐标       size 为 1*2*1       不旋转                  颜色为红色
		DrawElement(-1.5f, -4.5f, 0.0f, 1.0f, 2.0f, 1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	glPopMatrix();

	// 绘制右上腿
	glPushMatrix();
		glRotatef(-Lag, 1.0f, 0.0f, 0.0f);		// 绕着 x 轴旋转 -Lag°

		//          	传入位置坐标       size 为 1*4*1       不旋转                  颜色为红色
		DrawElement(1.5f, -1.5f, 0.0f, 1.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	glPopMatrix();

	// 绘制右下腿
	glPushMatrix();
		glRotatef(-Calf, 1.0f, 0.0f, 0.0f);		// 绕着 x 轴旋转 -Calf°

		//          	传入位置坐标       size 为 1*2*1       不旋转                  颜色为红色
		DrawElement(1.5f, -4.5f, 0.0f, 1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	glPopMatrix();

	glPopMatrix();


	// 递增旋转角度
	Calf += langle;			
	Lag += langle * 0.75;
	Body += BodyAngle;

	// 手臂摆到 40° 
	if (Hand >= 40 || Hand <= -40) {
		hangle = -hangle;
	}
	
	// 下臂继续上摆，上臂停住
	if ( Hand >= 25 || Hand <= -25 ) {
		Hand += hangle;
	}
	else {
		Hand += hangle;
		Arm += hangle * 0.75;
	}

	// 腿摆到 35°
	if (Calf >= 35 || Calf <= -35) {
		langle = -langle;
	}
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