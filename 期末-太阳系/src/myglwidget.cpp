#include "myglwidget.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <utils/skybox.h>
#include <utils/objLoader.h>
#include <QtCore/qdir.h>
#include <qopenglextrafunctions.h>

#define N 1000              // 行星带粒子数
#define M 100               // 随机粒子数
using namespace std;
ObjLoader stone;	        // 实例化 ObjLoader
float vertices[2592*3*6];   // 一个粒子的顶点属性, 存储内容:一个顶点位置(x,y,z)，一个法向量(nx,ny,nz)
float stoneArr[N][3];       // 行星带粒子的属性,存储内容：粒子位置(stoneArr[i][0] = x,stoneArr[i][1] = y,0)，粒子半径缩放 stoneArr[i][2] = s 
float randomParticle[M][6]; // 随机粒子属性，存储内容: 粒子位置(randomParticle[i][0] = x,randomParticle[i][1] = y,0)，粒子半径缩放 randomParticle[i][2] = s,粒子倾斜角 randomParticle[i][3] = slope,粒子旋转角度 randomParticle[i][4]，粒子旋转自增角度 randomParticle[i][5]

/*###################################################
##  函数: createStoneArr
##  函数描述： 随机生成行星带粒子属性和随机粒子属性
##  参数描述： 无
#####################################################*/
void MyGLWidget::createStoneArr(){
    // 行星带粒子属性
    for(int i = 0; i < N;i++){
        float r = 2.0 *(double)rand() / (double)RAND_MAX - 1.0;         // 范围为 [-1,1] 的随机数
        r += 5.2f;
        float x = r * cos(i);           // 圆坐标
        float y = r * sin(i);
        float s = rand()%10+1;
        stoneArr[i][0] = x;
        stoneArr[i][1] = y;
        stoneArr[i][2] = s;
    }

    // 随机粒子属性
    for(int i = 0; i < M; i++){
        float r = rand()%20 + 1;
        float x = r*cos(i);
        float y = r*sin(i);
        float s = rand()%10+1;
        float slope = rand()%180;
        float addRevolution = rand()%5+1;
        randomParticle[i][0] = x;
        randomParticle[i][1] = y;
        randomParticle[i][2] = s;
        randomParticle[i][3] = slope;
        randomParticle[i][4] = 0.0f;
        randomParticle[i][5] = addRevolution;
    }

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
    quadObj = gluNewQuadric();//创建一个二次曲面物体
    setMouseTracking(true);     // 接收鼠标移动事件
    firstMouse = true;
    camera = Camera(vec3(0.0f, 0.0f, 3.0f));
    lighting = false;
    stone = ObjLoader("./obj/stone.obj",vertices);
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
	delete this->timer;
    delete sun;         // 太阳
    delete mercury;     // 水星
    delete venus;       // 金星
    delete earth;       // 地球
    delete moon;        // 月球
    delete mars;        // 火星
    delete jupiter;     // 木星
    delete saturn;      // 土星
    // delete comet;       // 彗星
    delete uranus;      // 天王星
    delete neptune;     // 海王星
    delete pluto;       // 冥王星
    gluDeleteQuadric(quadObj);
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等，加载纹理贴图，初始化vbo vao，生成行星带粒子属性和随机粒子属性
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Your Implementation

    glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glShadeModel(GL_SMOOTH);                //允许平滑着色

    // 加载天空盒纹理
    boxtex = new Texture("./tex/final.png", true);
    // 加载星球纹理
	sun     = new Planet("./tex/太阳.png",  0.0f,   0.1f,   0.0f,   1.0f,       vec3(0.0f, 0.0f, 0.0f));	// 公转每天增加的角度，自转每天增加的角度，倾斜角，半径，离公转星球的距离位置
    mercury = new Planet("./tex/水星.png",  2.09f,  0.614f,  0.034f, 0.07f,    vec3(1.3f, -0.1f, 0.0f));
	venus   = new Planet("./tex/金星.png",  1.20f,  0.233f,  177.3f, 0.142f,    vec3(1.885f, -0.4f, 0.0f));
	earth   = new Planet("./tex/地球.png",  0.5f,   3.0f,   23.4f,  0.15f,     vec3(2.6f, 0.0f, 0.0f));
	moon    = new Planet("./tex/月亮.png",  3.1f,   0.33f,  6.68f,  0.055f,    vec3(0.35f, 0.0f, 0.0f));
	mars    = new Planet("./tex/火星.png",  1.524f, 20.1f, 25.2f,  0.123f,    vec3(3.952f, 0.0f, 0.0f));
	jupiter = new Planet("./tex/木星.png",  0.389f, 24.0f, 3.1f,   0.2227f,    vec3(6.52f, 0.0f, 0.0f));
	saturn  = new Planet("./tex/土星.png",  0.334f, 27.4f, 26.7f,  0.266f,    vec3(9.804f, 1.0f, 0.0f));
	uranus  = new Planet("./tex/天王星.png",0.311f, 10.8f, 97.8f,  0.23f,    vec3(11.97f, 0.0f, 0.0f));
	neptune = new Planet("./tex/海王星.png",0.186f, 15.0f, 28.3f,  0.22f,    vec3(16.0f, 0.8f, 0.0f));
    pluto = new Planet("./tex/冥王星.png",0.286f, 6.0f, 15.3f,  0.32f,    vec3(20.0f, 0.5f, 0.0f));
    particle = new Planet("./tex/flash.png",3.06f, 0.0f, 28.3f,  0.01f,    vec3(9.0f, 0.8f, 0.0f));

    // 设置星球的材质
	sun->ambient[0] = 1.0f,sun->ambient[1] = 1.0f,sun->ambient[2] = 1.0f,sun->ambient[3] = 1.0f;            // 材质属性中的环境光
    sun->diffuse[0] = 1.0f,sun->diffuse[1] = 1.0f,sun->diffuse[2] = 1.0f,sun->diffuse[3] = 1.0f;            // 材质属性中的散射光
    sun->specular[0] = 1.0f,sun->specular[1] = 1.0f,sun->specular[2] = 1.0f,sun->specular[3] = 1.0f;        // 材质属性中的镜面反射光
    sun->emission[0] = 1.0f,sun->emission[1] = 1.0f,sun->emission[2] = 1.0f,sun->emission[3] = 1.0f;        // 材质属性的镜面反射指数
    sun->shininess = 1.0f;

 	mercury->ambient[0] = 0.2f,mercury->ambient[1] = 0.2f,mercury->ambient[2] = 0.2f,mercury->ambient[3] = 1.0f;
    mercury->diffuse[0] = 0.8f,mercury->diffuse[1] = 0.8f,mercury->diffuse[2] = 0.8f,mercury->diffuse[3] = 1.0f;
    mercury->specular[0] = 0.0f,mercury->specular[1] = 0.0f,mercury->specular[2] = 0.0f,mercury->specular[3] = 1.0f;
    mercury->emission[0] = 0.0f,mercury->emission[1] = 0.0f,mercury->emission[2] = 1.0f,mercury->emission[3] = 1.0f;
    mercury->shininess = 0.0f;

 	venus->ambient[0] = 0.1f,venus->ambient[1] = 0.1f,venus->ambient[2] = 0.3f,venus->ambient[3] = 1.0f;
    venus->diffuse[0] = 0.0f,venus->diffuse[1] = 0.0f,venus->diffuse[2] = 0.0f,venus->diffuse[3] = 1.0f;
    venus->specular[0] = 0.0f,venus->specular[1] = 0.0f,venus->specular[2] = 1.0f,venus->specular[3] = 1.0f;
    venus->emission[0] = 0.5f,venus->emission[1] = 0.0f,venus->emission[2] = 0.0f,venus->emission[3] = 1.0f;
    venus->shininess = 30.0f;
       
	earth->ambient[0] = 0.0f,earth->ambient[1] = 0.0f,earth->ambient[2] = 0.0f,earth->ambient[3] = 1.0f;
    earth->diffuse[0] = 0.0f,earth->diffuse[1] = 0.0f,earth->diffuse[2] = 0.0f,earth->diffuse[3] = 1.0f;
    earth->specular[0] = 0.0f,earth->specular[1] = 0.0f,earth->specular[2] = 1.0f,earth->specular[3] = 1.0f;
    earth->emission[0] = 0.5f,earth->emission[1] = 0.0f,earth->emission[2] = 0.0f,earth->emission[3] = 1.0f;
    earth->shininess = 10.0f;

	moon->ambient[0] = 0.0f,moon->ambient[1] = 0.0f,moon->ambient[2] = 0.0f,moon->ambient[3] = 1.0f;
    moon->diffuse[0] = 0.0f,moon->diffuse[1] = 0.0f,moon->diffuse[2] = 0.0f,moon->diffuse[3] = 1.0f;
    moon->specular[0] = 0.0f,moon->specular[1] = 0.0f,moon->specular[2] = 1.0f,moon->specular[3] = 1.0f;
    moon->emission[0] = 0.5f,moon->emission[1] = 0.0f,moon->emission[2] = 0.0f,moon->emission[3] = 1.0f;
    moon->shininess = 10.0f;

	mars->ambient[0] = 0.0f,mars->ambient[1] = 0.0f,mars->ambient[2] = 0.0f,mars->ambient[3] = 1.0f;
    mars->diffuse[0] = 0.0f,mars->diffuse[1] = 0.0f,mars->diffuse[2] = 0.0f,mars->diffuse[3] = 1.0f;
    mars->specular[0] = 0.0f,mars->specular[1] = 0.0f,mars->specular[2] = 1.0f,mars->specular[3] = 1.0f;
    mars->emission[0] = 0.5f,mars->emission[1] = 0.0f,mars->emission[2] = 0.0f,mars->emission[3] = 1.0f;
    mars->shininess = 10.0f;

	jupiter->ambient[0] = 0.0f,jupiter->ambient[1] = 0.0f,jupiter->ambient[2] = 0.0f,jupiter->ambient[3] = 1.0f;
    jupiter->diffuse[0] = 0.0f,jupiter->diffuse[1] = 0.0f,jupiter->diffuse[2] = 0.0f,jupiter->diffuse[3] = 1.0f;
    jupiter->specular[0] = 0.0f,jupiter->specular[1] = 0.0f,jupiter->specular[2] = 1.0f,jupiter->specular[3] = 1.0f;
    jupiter->emission[0] = 0.5f,jupiter->emission[1] = 0.0f,jupiter->emission[2] = 0.0f,jupiter->emission[3] = 1.0f;
    jupiter->shininess = 10.0f;

	saturn->ambient[0] = 0.0f,saturn->ambient[1] = 0.0f,saturn->ambient[2] = 0.0f,saturn->ambient[3] = 1.0f;
    saturn->diffuse[0] = 0.0f,saturn->diffuse[1] = 0.0f,saturn->diffuse[2] = 0.0f,saturn->diffuse[3] = 1.0f;
    saturn->specular[0] = 0.0f,saturn->specular[1] = 0.0f,saturn->specular[2] = 1.0f,saturn->specular[3] = 1.0f;
    saturn->emission[0] = 0.5f,saturn->emission[1] = 0.0f,saturn->emission[2] = 0.0f,saturn->emission[3] = 1.0f;
    saturn->shininess = 10.0f;

	uranus->ambient[0] = 0.0f,uranus->ambient[1] = 0.0f,uranus->ambient[2] = 0.0f,uranus->ambient[3] = 1.0f;
    uranus->diffuse[0] = 0.0f,uranus->diffuse[1] = 0.0f,uranus->diffuse[2] = 0.0f,uranus->diffuse[3] = 1.0f;
    uranus->specular[0] = 0.0f,uranus->specular[1] = 0.0f,uranus->specular[2] = 1.0f,uranus->specular[3] = 1.0f;
    uranus->emission[0] = 0.5f,uranus->emission[1] = 0.0f,uranus->emission[2] = 0.0f,uranus->emission[3] = 1.0f;
    uranus->shininess = 10.0f;

	neptune->ambient[0] = 0.0f,neptune->ambient[1] = 0.0f,neptune->ambient[2] = 0.0f,neptune->ambient[3] = 1.0f;
    neptune->diffuse[0] = 0.0f,neptune->diffuse[1] = 0.0f,neptune->diffuse[2] = 0.0f,neptune->diffuse[3] = 1.0f;
    neptune->specular[0] = 0.0f,neptune->specular[1] = 0.0f,neptune->specular[2] = 1.0f,neptune->specular[3] = 1.0f;
    neptune->emission[0] = 0.5f,neptune->emission[1] = 0.0f,neptune->emission[2] = 0.0f,neptune->emission[3] = 1.0f;
    neptune->shininess = 10.0f;

	particle->ambient[0] = 1.0f,particle->ambient[1] = 1.0f,particle->ambient[2] = 1.0f,particle->ambient[3] = 1.0f;
    particle->diffuse[0] = 1.0f,particle->diffuse[1] = 1.0f,particle->diffuse[2] = 1.0f,particle->diffuse[3] = 1.0f;
    particle->specular[0] = 1.0f,particle->specular[1] = 1.0f,particle->specular[2] = 1.0f,particle->specular[3] = 1.0f;
    particle->emission[0] = 1.0f,particle->emission[1] = 1.0f,particle->emission[2] = 1.0f,particle->emission[3] = 1.0f;
    particle->shininess = 1.0f;

    // 初始化vbo vao
    initVboVao();
    
    // 随机生成行星带和随机粒子属性 
    createStoneArr();
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
	glGenBuffers(2, vboId);

	//绑定数组指针
	glBindVertexArray(vaoId);

    // 写入和设置顶点位置数据
	glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableClientState(GL_VERTEX_ARRAY);
    
    // 写入和设置顶点法向量数据
    glBindBuffer(GL_ARRAY_BUFFER, vboId[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glNormalPointer(GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GL_FLOAT)));
    glEnableClientState(GL_NORMAL_ARRAY);
    
    //设置顶点属性指针
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	// glEnableVertexAttribArray(0);

	// 设置法向量属性
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	// glEnableVertexAttribArray(1);	

	//解绑VAO
	glBindVertexArray(0);

    // for(int i = 0; i < 10;i++){
    //     printf("%d ",vertices[i]);
    // }
}

/*###################################################
##  函数: SetLight
##  函数描述： 设置光源
##  参数描述：
##          lighting：是否开启设置光源
#####################################################*/
void MyGLWidget::SetLight(bool lighting)
{
	float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };      // 环境光
	float dif[4] = { 0.6f, 0.6f, 0.9f, 1.0f };      // 漫反射
    float spe[4] = { 0.7f, 0.7f, 0.8f, 1.0f };      // 镜面反射
	float pos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };      // 光源位置
    float lightConstAttenuation[] = {1.0f};         // 常数衰减因子
    float lightLinearAttenuation[] = {0.1f};        // 线性衰减因子
    float lightQuadAttenuation[] = {0.0f};          // 平方衰减因子
    float LightModelAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0 };      // 全局环境光的 RGBA 强度

	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spe);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lightConstAttenuation);
    glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, lightLinearAttenuation);
    glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lightQuadAttenuation);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);

    // 开启光照
	if (lighting)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
	}
	else    // 关闭光照
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
        glDisable(GL_COLOR_MATERIAL);
	}

	glShadeModel(GL_SMOOTH);
}

/*###################################################
##  函数: SetMaterial
##  函数描述： 设置物体材质
##  参数描述：
##          p：星球类
#####################################################*/
void MyGLWidget::SetMaterial(Planet* p){
    glMaterialfv(GL_FRONT, GL_AMBIENT, p->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, p->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, p->specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, p->emission);
    glMaterialf (GL_FRONT, GL_SHININESS,p->shininess);  
}


/*###################################################
##  函数: SetMaterialDefault
##  函数描述： 设置场景材质默认值
##  参数描述：无
#####################################################*/
void MyGLWidget::SetMaterialDefault(){
    float ambient[]   = {0.1f, 0.1f, 0.1f, 1.0f};
    float diffuse[]   = {0.3f, 0.3f, 0.3f, 1.0f};
    float specular[]  = {0.3f, 0.3f, 0.3f, 1.0f};
    float emission[]  = {0.3f, 0.3f, 0.3f, 1.0f};
    float shininess   = 0.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);        // 材质属性中的环境光
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);        // 材质属性中的散射光
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);      // 材质属性中的镜面反射光
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);      // 材质属性中的发射光
    glMaterialf (GL_FRONT, GL_SHININESS,shininess);     // 材质属性的镜面反射指数
}

/*###################################################
##  函数: drawSphere
##  函数描述： 二次曲面球绘制
##  参数描述：
##          radius: 球的半径
##          slices: 球的经度细分
##          stack: 球的纬度细分
##          texture: 是否绑定纹理
#####################################################*/
void MyGLWidget::drawSphere(double radius , int slices, int stack ,bool texture)
{  
  gluQuadricOrientation(quadObj, GLU_OUTSIDE);      // 设置球的法向量方向向外
  gluQuadricTexture(quadObj, true);                 // 建立纹理坐标
  gluSphere(quadObj,radius,slices,stack);           // 画球
}


/*###################################################
##  函数: drawDisk
##  函数描述： 二次曲面圆盘绘制
##  参数描述：
##          innerRadius: 内环的半径
##          outerRadius: 外环的半径
##          slices: 细分程度
#####################################################*/
void MyGLWidget::drawDisk(GLdouble innerRadius, GLdouble outerRadius, GLint slices ){
  gluDisk(quadObj,innerRadius, outerRadius, slices, 1);     // 画圆盘
}


/*###################################################
##  函数: drawPlanet
##  函数描述： 绘制环绕太阳转的星球
##  参数描述：
##          planet: 星球类
#####################################################*/
void MyGLWidget::drawPlanet(Planet* planet){

    // SetMaterial(planet);                 // 设置星球材质属性
    // 绘制行星轨迹
	glPushMatrix();
    glTranslatef(0.0f,planet->pos.y,0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
	drawDisk(planet->pos.x, planet->pos.x+0.01f, 100);
    glPopMatrix();

    // 绘制星球
    glPushMatrix();
	glRotatef(planet->revolution, 0.0f, 1.0f, 0.0f);	            // 公转
	glTranslatef(planet->pos.x, planet->pos.y, planet->pos.z);      // 公转半径or物体位置
	glRotatef(planet->slope, 0.0f, 0.0f, 1.0f);                     // 绕 z 轴倾斜，星球轴倾斜角
    glRotatef(planet->rotation, 0.0f, 1.0f, 0.0f);		            // 绕 x 轴自转
    planet->texture->BindTexture(planet->texture->m_tex);           // 绑定纹理
	drawSphere(planet->radius, 20, 20, true);                       // 绘制星球
    
    // SetMaterialDefault();                                        // 恢复场景材质属性
    planet->texture->BindTexture(0);                                // 取消绑定纹理
	glPopMatrix();    

    planet->revolution += planet->addRevolution;                    // 公转角度增加
    planet->rotation += planet->addRotation;                        // 自转角度增加
}


/*###################################################
##  函数: DrawEarthAndMoon
##  函数描述： 绘制类地月的行星系统
##  参数描述：
##          p1: 中心天体
##          p2: 卫星
#####################################################*/
void MyGLWidget::DrawEarthAndMoon(Planet *p1, Planet *p2)
{    
    // SetMaterial(p1);                                  // 设置中心天体材质属性

    // 绘制中心天体轨迹
    glPushMatrix();
    glTranslatef(0.0f,p1->pos.y,0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
	drawDisk(p1->pos.x, p1->pos.x+0.01f, 100);
    glPopMatrix();

    // 绘制中心天体
	glPushMatrix();     
	p1->texture->BindTexture(p1->texture->m_tex);           // 绑定中心天体纹理
	glRotatef(p1->revolution, 0.0f, 1.0f, 0.0f);            // 公转
	glTranslatef(p1->pos.x, p1->pos.y, p1->pos.z);          // 公转半径/物体位置
    glRotatef(p1->slope, 0.0f, 0.0f, 1.0f);                 // 绕 z 轴倾斜
	glRotatef(p1->rotation, 0.0f, 1.0f, 0.0f);              // 绕 x 轴自转 
	drawSphere(p1->radius, 20, 20, true);                   // 绘制星球
    
    // SetMaterialDefault();                                // 回复默认场景材质属性
    p1->texture->BindTexture(0);                            // 取消绑定纹理

    // SetMaterial(p2);                                     // 绑定卫星天体材质    

    // 绘制卫星轨迹       
    glPushMatrix();
    glTranslatef(0.0f,p2->pos.y,0.0f);              
    glRotatef(90, 1.0f, 0.0f, 0.0f);
	drawDisk(p2->pos.x, p2->pos.x+0.005f, 100);
    glPopMatrix();

    // 绘制卫星,在中心天体的矩阵上做变换
	glPushMatrix();
	p2->texture->BindTexture(p2->texture->m_tex);               // 绑定卫星纹理
	glRotatef(p2->revolution, 0.0f, 1.0f, 0.0f);                // 公转
	glTranslatef(p2->pos.x, p2->pos.y, p2->pos.z);              // 公转半径/物体位置
    glRotatef(p2->slope, 0.0f, 0.0f, 1.0f);                     // 绕 z 轴倾斜
	glRotatef(p2->rotation, 0.0f, 1.0f, 0.0f);                  // 绕 x 轴自转
	drawSphere(p2->radius, 20, 20, true);                       // 绘制球

    // SetMaterialDefault();                                    // 恢复默认场景属性
    p2->texture->BindTexture(0);                                // 取消绑定纹理
	glPopMatrix();
	glPopMatrix();

    p1->revolution += p1->addRevolution;                        // 中心天体公转角度增加
    p1->rotation += p1->addRotation;                            // 中心天体自转角度增加
    p2->revolution += p2->addRevolution;                        // 卫星公转角度增加
    p2->rotation += p2->addRotation;                            // 卫星自转角度增加
}

/*###################################################
##  函数: drawParticle
##  函数描述： 绘制流行粒子
##  参数描述：
##          p: 星球类
#####################################################*/
void MyGLWidget::drawParticle(Planet* p) {
    SetMaterial(p);             // 设置流行粒子材质
    // 绘制多个星球实现重影效果
    for(int i = 0; i < 50;i++){
        float r = 2.0 * (double)rand() / (double)RAND_MAX - 1.0;
        r = r/10;
        glPushMatrix();
        glRotatef(90,0.0f,0.0f,1.0f);                           // 绕太阳 x 轴旋转
        glRotatef(p->revolution, 0.0f, 1.0f, 0.0f);	            // 公转
        glTranslatef(p->pos.x+r, p->pos.y+r, p->pos.z+r);       // 公转半径/物体位置
        glRotatef(p->slope, 0.0f, 0.0f, 1.0f);                  // 绕 z 轴倾斜
        // glRotatef(p->rotation, 0.0f, 1.0f, 0.0f);		    // 绕 x 轴自转
        p->texture->BindTexture(p->texture->m_tex);             // 绑定纹理
        drawSphere(p->radius+r, 20, 20, true);                  // 绘制球
        p->texture->BindTexture(0);                             // 取消绑定纹理
        glPopMatrix(); 
    }
   
    SetMaterialDefault();                                       // 恢复场景材质
    
    p->revolution += p->addRevolution;                          // 公转角度增加
}


/*###################################################
##  函数: drawPlanet2
##  函数描述： 绘制带光圈的行星，如土星
##  参数描述：
##          p: 星球类
#####################################################*/
void MyGLWidget::drawPlanet2(Planet * p)
{
    // SetMaterial(p);                                 // 设置星球材质

    // 绘制星球轨迹
    glPushMatrix();
    glTranslatef(0.0f,p->pos.y,0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
	drawDisk(p->pos.x, p->pos.x+0.01f, 100);
    glPopMatrix();

    // 绘制星球
	glPushMatrix();
	p->texture->BindTexture(p->texture->m_tex);         // 绑定纹理
    glRotatef(p->revolution, 0.0f, 1.0f, 0.0f);         // 公转
	glTranslatef(p->pos.x, p->pos.y, p->pos.z);         // 公转半径or星球位置
    glRotatef(p->slope, 0.0f, 0.0f, 1.0f);              // 绕 z 轴倾斜
    // 光圈相对当前矩阵变换，push当前矩阵，绘制星球    
        glPushMatrix();
	    glRotatef(p->rotation, 0.0f, 1.0f, 0.0f);       // 绕 x 轴自转
	    drawSphere(p->radius, 20, 20, true);
        glPopMatrix();

    // 绘制光圈
	glPushMatrix();  
    glRotatef(90, 1.0f, 0.0f, 0.0f);
	drawDisk(0.5,0.7, 50);

    // SetMaterialDefault();                            // 恢复场景默认材质
    p->texture->BindTexture(0);                         // 取消绑定纹理
	glPopMatrix();
	glPopMatrix();

    p->revolution += p->addRevolution;                  // 公转角度增加
    p->rotation += p->addRotation;                      // 自转角度增加
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);	// 被挡住的看不到
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, width() / height(), 0.1f, 60000.0f);		// 放置摄像机

    camera.update();                            // 摄像机设置
	// glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
    // 立方体天空盒
    // glPushMatrix();
    // glRotatef(sun->rotation/8,1.0f,0.0f,0.0f);       // 天空盒旋转
    // drawSkyBox(boxtex, 0.0f, 0.0f, 0.0f, 1000.0f, 1000.0f, 1000.0f);
    // glPopMatrix();

    // 球型天空盒
    glPushMatrix();
    boxtex->BindTexture(boxtex->m_tex);	
    glRotatef(sun->rotation,1.0f,1.0f,1.0f);        // 天空盒旋转
    gluSphere(quadObj,100.0f,20,20);
    boxtex->BindTexture(0);
    glPopMatrix();

    // 设置场景光照
    SetLight(lighting);     

    // 设置太阳材质
    SetMaterial(sun);
    drawPlanet(sun);
    SetMaterialDefault();

    // 绘制太阳系
    drawPlanet(mercury);
    drawPlanet(venus);
    drawPlanet(earth);
    DrawEarthAndMoon(earth,moon);
    drawPlanet(mars);
    drawPlanet(jupiter);
    // drawPlanet(saturn);
    drawPlanet2(saturn);
    drawPlanet(uranus);
    drawPlanet(neptune);
    drawPlanet(pluto);

    drawParticle(particle);     // 绘制流星
    
    
    // 绘制行星带
    glColor3f(0.345f,0.345f,0.345f);
    for(int i = 0; i < N;i++){
        glPushMatrix();
        glRotatef(sun->rotation, 0.0f, 1.0f, 0.0f);                 // 公转
        glTranslatef(stoneArr[i][0],0.0f,stoneArr[i][1]);           // 公转半径
        glScalef(0.00005f * stoneArr[i][2],0.00005f * stoneArr[i][2],0.00005f * stoneArr[i][2]);      // 石头大小
        // vao 绘制
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glBindVertexArray(vaoId);
        glDrawArrays(GL_TRIANGLES, 0, 2592 * 3);
        glBindVertexArray(0);
        glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
        glDisableClientState(GL_NORMAL_ARRAY);
        glPopMatrix();
    }

    // 绘制随机粒子
    for(int i = 0; i < M; i++){
        glPushMatrix();
            glRotatef(randomParticle[i][4], 1.0f, 1.0f, 0.0f);              // 公转
            glTranslatef(randomParticle[i][0],0.0f,randomParticle[i][1]);   // 公转半径/物体位置
            glRotatef(randomParticle[i][3], 0.0f, 0.0f, 1.0f);              // 绕 z 轴倾斜
            glScalef(0.00005f * randomParticle[i][2],0.00005f * randomParticle[i][2],0.00005f * randomParticle[i][2]);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);
            glBindVertexArray(vaoId);
            glDrawArrays(GL_TRIANGLES, 0, 2592 * 3);
            glBindVertexArray(0);
            glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
            glDisableClientState(GL_NORMAL_ARRAY);
        glPopMatrix();
        randomParticle[i][4] += randomParticle[i][5];
    }

    // stone.Draw(sun,5);
	glDisable(GL_TEXTURE_2D);

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

/*###################################################
##  函数: keyPressEvent
##  函数描述： 键盘控制视角
##  参数描述： 无
#####################################################*/
void MyGLWidget::keyPressEvent(QKeyEvent *e)
{
    float cameraSpeed = 0.2f;           // 摄像机移动速度
    //Key Control
    if (e->key() == Qt::Key_W){ 
        // printf("w:\nforward:%f,%f,%f\n",camera.forward.x,camera.forward.y,camera.forward.z);
        // printf("pos:%f,%f,%f\n",camera.pos.x,camera.pos.y,camera.forward.z);
        camera.pos += camera.forward*cameraSpeed;
    }
        
    if (e->key() == Qt::Key_S){
        // printf("s:\nforward:%f,%f,%f\n",camera.forward.x,camera.forward.y,camera.forward.z);
        // printf("pos:%f,%f,%f\n",camera.pos.x,camera.pos.y,camera.forward.z);
        camera.pos -= camera.forward*cameraSpeed;
    }
        
    if (e->key() == Qt::Key_A){
        // printf("a:\nright:%f,%f,%f\n",camera.right.x,camera.right.y,camera.right.z);
        // printf("pos:%f,%f,%f\n",camera.pos.x,camera.pos.y,camera.forward.z);
        camera.pos -= camera.right *cameraSpeed;
    }
        
    if (e->key() == Qt::Key_D){
        // printf("d:\nright:%f,%f,%f\n",camera.right.x,camera.right.y,camera.right.z);
        // printf("pos:%f,%f,%f\n",camera.pos.x,camera.pos.y,camera.forward.z);
        camera.pos +=camera.right *cameraSpeed;
    }    
    if (e->key() == Qt::Key_I)
        camera.processMouseMovement(0.0f, 5.0f);
    if (e->key() == Qt::Key_K)
        camera.processMouseMovement(0.0f, -5.0f);
    if (e->key() == Qt::Key_J)
        camera.processMouseMovement(-5.0f, 0.0f);
    if (e->key() == Qt::Key_L)
        camera.processMouseMovement(5.0f, 0.0f);
    if (e->key() == Qt::Key_O)
        lighting = !lighting;
    update();
}
/*###################################################
##  函数: mouseMoveEvent
##  函数描述： 鼠标控制视角移动
##  参数描述： 无
#####################################################*/
void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    float xpos = e->x();
    float ypos = e->y();
    if (firstMouse)
    {
        //捕获鼠标位置
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    //获取鼠标位移
    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    //更新鼠标位置
    lastX = xpos;
    lastY = ypos;
    //调用视角控制函数
    camera.processMouseMovement(xOffset, yOffset);
    update();
}