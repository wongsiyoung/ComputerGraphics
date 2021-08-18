#include "myglwidget.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QtCore/qdir.h>
#include <qopenglextrafunctions.h>
using namespace std;

#define PI acos(-1) 	// PI 常量
#define lats 512		// 纬度细分
#define lons 512		// 经度细分

double TIME;			// 计时
LARGE_INTEGER t1,t2,tc; 

// 顶点结构
struct Float3{
	float x,y,z;
};

GLfloat vertices[6 * 3 * lats * lons * 2];  		// 顶点属性数组, 存放顶点值和法向量,顶点重复存储
GLfloat verticesIndex[4 * 3 * lats * lons * 2];  	// 顶点属性数组, 存放顶点值和法向量,顶点不重复存储
GLuint index[6 * lats * lons];						// 索引顶点数组

/*###################################################
##  函数: getPoint
##  函数描述： 根据 u，v 角度得到顶点
##  参数描述：
##  u,v 角度，值在[0,1]
#####################################################*/
Float3 getPoint(float u,float v){
	float r = 0.9f;
	Float3 point;
	point.x = r * sin(PI * u) * cos(2 * PI * v);	// u,v 的范围在[0,1],一个映射到[0,180],一个映射到[0,360],所以 v 要乘 2
	point.y = r * sin(PI * u) * sin(2 * PI * v);
	point.z = r * cos(PI * u);
	return point;
}

/*###################################################
##  函数: createSphere1
##  函数描述： 创建球的顶点属性，顶点重复存储
##  参数描述：
##  	sphere: 球的顶点属性存放地址
##  	longitude: 经度细分程度
##		latitude: 纬度细分程度
#####################################################*/
void MyGLWidget::createSphere1(GLfloat *sphere,GLuint longitude, GLuint latitude){
	GLfloat lon_step = 1.0f / longitude;		// 经度细分
	GLfloat lat_step = 1.0f / latitude;			// 纬度细分
	GLuint offset = 0;							// 顶点数组偏移量
	Float3 point1,point2,point3,point4;	
	Float3 nvector;
	float vec1[3],vec2[3],vec3[3];
	float D;

	for(int lat = 0; lat < latitude; lat++){
		for(int lon = 0; lon < longitude; lon++){
			// 一次得到四个点,生成两个三角形
			point1 = getPoint(lat * lat_step, lon * lon_step);
			point2 = getPoint((lat + 1) * lat_step, lon * lon_step);
			point3 = getPoint((lat + 1) * lat_step, (lon + 1) * lon_step);
			point4 = getPoint(lat * lat_step, (lon + 1) * lon_step);
			
			// 计算第一个三角形的顶点的法向量
			vec1[0] = point1.x - point4.x;
			vec1[1] = point1.y - point4.y;
			vec1[2] = point1.z - point4.z;

			vec2[0] = point1.x - point3.x;
			vec2[1] = point1.y - point3.y;
			vec2[2] = point1.z - point3.z;

			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));
			
			nvector.x = vec3[0] / D;
			nvector.y = vec3[1] / D;
			nvector.z = vec3[2] / D;

			// 存储第一个顶点的位置信息和法向量
			memcpy(sphere + offset, &point1, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &nvector, sizeof(Float3));
			offset += 3;	
			memcpy(sphere + offset, &point4, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &nvector, sizeof(Float3));
			offset += 3;			
			memcpy(sphere + offset, &point3, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &nvector, sizeof(Float3));
			offset += 3;
			
			// 计算第二个三角形的顶点的法向量
			vec1[0] = point1.x - point3.x;
			vec1[1] = point1.y - point3.y;
			vec1[2] = point1.z - point3.z;

			vec2[0] = point1.x - point2.x;
			vec2[1] = point1.y - point2.y;
			vec2[2] = point1.z - point2.z;

			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));
			
			nvector.x = vec3[0] / D;
			nvector.y = vec3[1] / D;
			nvector.z = vec3[2] / D;

			// 存储第一个顶点的位置信息和法向量
			memcpy(sphere + offset, &point1, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &nvector, sizeof(Float3));
			offset += 3;	
			memcpy(sphere + offset, &point3, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &nvector, sizeof(Float3));
			offset += 3;			
			memcpy(sphere + offset, &point2, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &nvector, sizeof(Float3));
			offset += 3;			
		}
	}
}

/*###################################################
##  函数: createSphere2
##  函数描述： 创建球的顶点属性，顶点不重复存储
##  参数描述：
##  	sphere: 球的顶点属性存放地址
##  	longitude: 经度细分程度
##		latitude: 纬度细分程度
#####################################################*/
void MyGLWidget::createSphere2(GLfloat *sphere,GLuint longitude, GLuint latitude){
	GLfloat lon_step = 1.0f / longitude;		// 经度细分
	GLfloat lat_step = 1.0f / latitude;			// 纬度细分
	GLuint offset = 0;							// 顶点数组偏移量
	GLuint indexOffset = 0;
	Float3 point1,point2,point3,point4;	
	Float3 nvector;
	float vec1[3],vec2[3],vec3[3];
	float D;

	for(int lat = 0; lat < latitude; lat++){
		for(int lon = 0; lon < longitude; lon++){
			// 一次得到四个点,生成两个三角形
			point1 = getPoint(lat * lat_step, lon * lon_step);
			point2 = getPoint((lat + 1) * lat_step, lon * lon_step);
			point3 = getPoint((lat + 1) * lat_step, (lon + 1) * lon_step);
			point4 = getPoint(lat * lat_step, (lon + 1) * lon_step);
			
			// 计算第三角形的顶点的法向量
			vec1[0] = point1.x - point4.x;
			vec1[1] = point1.y - point4.y;
			vec1[2] = point1.z - point4.z;

			vec2[0] = point1.x - point3.x;
			vec2[1] = point1.y - point3.y;
			vec2[2] = point1.z - point3.z;

			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));
			
			nvector.x = vec3[0] / D;
			nvector.y = vec3[1] / D;
			nvector.z = vec3[2] / D;

			// 1 4 3、 1 3 2
			index[indexOffset] = offset / 6 + 1 - 1;
			indexOffset++;
			index[indexOffset] = offset / 6 + 4 - 1;
			indexOffset++;
			index[indexOffset] = offset / 6 + 3 - 1;
			indexOffset++;

			index[indexOffset] = offset / 6 + 1 - 1;
			indexOffset++;
			index[indexOffset] = offset / 6 + 3 - 1;
			indexOffset++;
			index[indexOffset] = offset / 6 + 2 - 1;
			indexOffset++;


			// 存储 4 个顶点的位置和法向量
			memcpy(sphere + offset, &point1, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &nvector, sizeof(Float3));
			offset += 3;	
			memcpy(sphere + offset, &point2, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &nvector, sizeof(Float3));
			offset += 3;			
			memcpy(sphere + offset, &point3, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &nvector, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &point4, sizeof(Float3));
			offset += 3;
			memcpy(sphere + offset, &nvector, sizeof(Float3));
			offset += 3;

		}
	}
}

/*###################################################
##  函数: initShader
##  函数描述： 初始化Shader,加载到内存，编译链接
##  参数描述：
##  	vertexPath:	顶点着色器相对于该文件的路径
##		fragmentPath: 片元着色器相对于该文件的路径
## 		ID: program ID 
#####################################################*/
void MyGLWidget::initShader(const char *vertexPath, const char *fragmentPath,unsigned int *ID){
     string vertexCode;
     string fragmentCode;
     ifstream vertexShaderFile;
     ifstream fragmentShaderFile;
     stringstream vertexStream;
     stringstream fragmentStream;
     const char *vertexShaderSource;
     const char *fragmentShaderSource;

     // 将相对路径转化成绝对路径
     string vertexDir = QDir::currentPath().toStdString() + "/" + vertexPath;
     string fragmentDir = QDir::currentPath().toStdString() + "/" + fragmentPath;

     //加载顶点着色器代码
     vertexShaderFile.open(vertexDir);
     vertexStream << vertexShaderFile.rdbuf();
     vertexShaderFile.close();

     //加载片元着色器代码
     fragmentShaderFile.open(fragmentDir);
     fragmentStream << fragmentShaderFile.rdbuf();
     fragmentShaderFile.close();

     //字符转换
     vertexCode = vertexStream.str();
     fragmentCode = fragmentStream.str();
     vertexShaderSource = vertexCode.c_str();
     fragmentShaderSource = fragmentCode.c_str();

     //编译顶点着色器，在控制台输出LOG
     int success;
     char infoLog[512];
     vertexShader = glCreateShader(GL_VERTEX_SHADER);
     glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
     glCompileShader(vertexShader);
     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
     if(!success)
     {
         glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
         cout << "error in vertexshader: compilation failed\n" << infoLog << endl;
     }
     else
         cout << "vertshader compiled successfully" << endl;

     //编译片元着色器，在控制台输出LOG
     fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
     glCompileShader(fragmentShader);
     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
     if (!success)
     {
         glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
         cout << "error in fragmentshader: compilation failed\n" << infoLog << endl;
     }
     else
         cout << "fragmentshader compiled successfully" << endl;

     //绑定并链接着色器
     *ID = glCreateProgram();
     glAttachShader(*ID, vertexShader);
     glAttachShader(*ID, fragmentShader);
     glLinkProgram(*ID);
     glGetProgramiv(*ID, GL_LINK_STATUS, &success);
     if(!success)
     {
         glGetProgramInfoLog(*ID, 512, NULL, infoLog);
         cout << "error: link failed\n" << infoLog << endl;
     }
     else
         cout << "link successfully" << endl;

     glDeleteShader(vertexShader);
     glDeleteShader(fragmentShader);

	switch(mode){
		case 0:
			initVboVao();												// 初始化 vao,vbo
	 		initShaderVariables();
			break;
		case 1:
		case 2:
			initShaderVariables();
			break;
		default:
			initVboVao();												// 初始化 vao,vbo
	 		initShaderVariables();		
	}
}


/*###################################################
##  函数: initVboVao
##  函数描述： 初始化顶点缓冲区与顶点缓冲数组配置
##  参数描述：无
#####################################################*/
void MyGLWidget::initVboVao(){
	// 创建物体的 VAO

	//创建并绑定相应功能指针
	glGenVertexArrays(1, &vaoId);
	glGenBuffers(1, &vboId);

	//绑定数组指针
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//写入顶点数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// 设置法向量属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);	

	//解绑VAO
	glBindVertexArray(0);
}

/*###################################################
##  函数: initShaderVariables
##  函数描述： 初始化着色器参数
##  参数描述：无
#####################################################*/
void MyGLWidget::initShaderVariables(){
	glUseProgram(program);
	float mat[16];

	//设置shader参数
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);			// 将球初始位置置于沿 z 轴负方向平移 10 个单位
	// glRotatef(body, 0, 1, 0);				// 绕 y 轴顺时针旋转 body°
	// glTranslatef(0.0f, 0.0f, 1.0f);			// 设置球的旋转半径
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, mat);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, mat);
	// 投影变换
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f, width() / height(), 0.1f, 1000.0f);		// 放置摄像机
	glGetFloatv(GL_PROJECTION_MATRIX, mat);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, mat);	
	glUniform1i(glGetUniformLocation(program, "mode"), mode);
}

/*###################################################
##  函数: DrawWithoutVBO
##  函数描述： 不使用 vbo 方式绘制
##  参数描述：无
#####################################################*/
void MyGLWidget::DrawWithoutVBO(){
	glUniform1f(glGetUniformLocation(program, "twisting"), angle);

	QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

	glBegin(GL_TRIANGLES);
	for(int i = 0; i < 6 * 3 * lats * lons * 2; i = i+6){
		glVertex3f(vertices[i],vertices[i+1],vertices[i+2]);
		glNormal3f(vertices[i+3],vertices[i+4],vertices[i+5]);
	}
	glEnd();

	QueryPerformanceCounter(&t2);
	TIME = (double)(t2.QuadPart - t1.QuadPart)*1.0 / (double)tc.QuadPart;

	if (body == 0)
		printf("TimeConsume : %.4fs\n",TIME); 


	body += 1.0f;
	angle += ballAngle;
	if(angle >= 10.0f || angle <= -10.0f)
		ballAngle = -ballAngle;
	
}

/*###################################################
##  函数: DrawWithVBO
##  函数描述： 使用 shader,vao 对点进行绘制
##  参数描述：无
#####################################################*/
void MyGLWidget::DrawWithVBO(){

	glUniform1f(glGetUniformLocation(program, "twisting"), angle);
	
	QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

	// 开始绘制物体
	glPushMatrix();
    glBindVertexArray(vaoId);
    glDrawArrays(GL_TRIANGLES, 0, 6 * lats * lons);
    glBindVertexArray(0);
    glPopMatrix();

	QueryPerformanceCounter(&t2);
	TIME = (double)(t2.QuadPart - t1.QuadPart)*1.0 / (double)tc.QuadPart;

	if (body == 0)
		printf("TimeConsume : %.4fs\n",TIME); 

	body += 1.0f;
	angle += ballAngle;
	if(angle >= 10.0f || angle <= -10.0f)
		ballAngle = -ballAngle;
	
}

/*###################################################
##  函数: DrawIndex
##  函数描述： 使用 index array 方式进行绘制
##  参数描述：无
#####################################################*/
void MyGLWidget::DrawIndex(){
	glUniform1f(glGetUniformLocation(program, "twisting"), angle);

    glEnableClientState(GL_NORMAL_ARRAY);		// 启用法线数组
    glEnableClientState(GL_VERTEX_ARRAY);		// 启用顶点数组
    glNormalPointer(GL_FLOAT, 6 * sizeof(GLfloat), verticesIndex + 3);	// 指定法线数组
    glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), verticesIndex);	// 指定顶点数组

	QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

    glPushMatrix();
    glDrawElements(GL_TRIANGLES, 6 * lats * lons, GL_UNSIGNED_INT, index);	// 绘制图形
    glPopMatrix();

	QueryPerformanceCounter(&t2);
	TIME = (double)(t2.QuadPart - t1.QuadPart)*1.0 / (double)tc.QuadPart;

	if (body == 0)
		printf("TimeConsume : %.4fs\n",TIME); 

    glDisableClientState(GL_VERTEX_ARRAY);  // 关闭数组
    glDisableClientState(GL_NORMAL_ARRAY);	

	body += 1.0f;
	angle += ballAngle;
	if(angle >= 10.0f || angle <= -10.0f)
		ballAngle = -ballAngle;
}


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
	angle = 0.0f;
	ballAngle = 0.1f;
	body = 0.0f;
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
	delete this->timer;

    //释放shader
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteProgram(program);

}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    // vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Your Implementation

    glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 选择绘制方式
	printf("please input a character.\n\\
			'0' means draw with VBO\n\\
			'1' means draw without VBO\n\\
			'2' means draw with index array\n\\
			else draw with VBO by default\n\\
			And the program will calculate time when painting for the first time\n");

	scanf("%d",&mode);
	
	switch(mode){
		case 0:
			createSphere1(vertices, lons, lats); 						// 创建小球顶点属性
			break;
		case 1: 
		case 2:
			createSphere2(verticesIndex, lons, lats);
			break;
		default:
			createSphere1(vertices, lons, lats); 						// 创建小球顶点属性
	}
						

	// 选择着色方式，加载编译链接物体 shader 文件
	printf("please input a number.\n\\
			1 means using the flat model\n\\
			2 means using the ground model\n\\
			3 means using the phong model\n\\
			4 menas using the cartoon model\n\\
			else use phong model by default\n");
	int number;
	scanf("%d",&number);
	switch(number){
		case 1:
			initShader("FlatVertexShader.vert", "FlatFragmentShader.frag",&program);		
			break;
		case 2:
			initShader("GroundVertexShader.vert", "GroundFragmentShader.frag",&program);
			break;
		case 3:
			initShader("PhongVertexShader.vert", "PhongFragmentShader.frag",&program);
			break;
		case 4:
			initShader("CartoonVertexShader.vert", "CartoonFragmentShader.frag",&program);
			break;
		default:
			initShader("PhongVertexShader.vert", "PhongFragmentShader.frag",&program);
	}
			
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
	// Your Implementation
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,0.0);			// 黑屏

	switch(mode){
		case 0:
			DrawWithVBO();			// 开始绘制
		case 1:
			DrawWithoutVBO();
		case 2:
			DrawIndex();
		default:
			DrawWithVBO();			// 开始绘制
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

