#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

/*###################################################
##  类: ObjLoader
##  类描述： 加载 obj 文件和绘制 obj 模型
##  方法描述：
##  Draw: 绘制 obj 函数
## 		传入参数： 无
##		返回参数： 无
#####################################################*/
class ObjLoader{
public:
 	struct vertex{			// 顶点(x,y,z) 结构体
  		float x;
  		float y;
  		float z;
 	};

 	// 面的三个顶点
	struct face3{
    	int vIndex[3]; 		// 三个顶点坐标索引
		int vnIndex[3]; 	// 三个法向量索引
	};
 	
	 // 面的四个顶点
 	struct face4{
  		int vIndex[4]; 		// 四个顶点坐标索引
  		int vnIndex[4];  	// 四个法向量索引
 	};

 	ObjLoader(string filename); 	// 读取 obj 文件
	void Draw();

private:
 	vector<vertex> v;  		// 存储顶点坐标
    vector<vertex> vn;   	// 存储法向量坐标
 	vector<face3> f3;  		// 存储面的三个顶点
 	vector<face4> f4;  		// 存储面的四个顶点
};

/*###################################################
##  函数: ObjLoader
##  函数描述： ObjLoader类的构造函数，读入 obj 文件
##  参数描述：
##  filename: 要读入的 obj 文件路径
#####################################################*/
ObjLoader::ObjLoader(string filename){
 	
	int count = 0; 
	ifstream file(filename.c_str());
	string line;
	// 存储 顶点坐标、法向量、面的顶点索引信息
	while (getline(file, line)){
		if(line.substr(0, 2) == "vn" ){ 			// 法向量坐标
			vertex Point;
			float x,y,z;
			istringstream s(line.substr(2));
			s >> Point.x; s >> Point.y; s >> Point.z;
			vn.push_back(Point);   
		}
		else if (line.substr(0, 1) == "v"){ 		// 顶点坐标
			vertex Point;
			float x, y, z;
			istringstream s(line.substr(2));
			s >> Point.x; s >> Point.y; s >> Point.z;
			v.push_back(Point);
		}
		else if (line.substr(0, 1) == "f"){			// 面的顶点索引
			string s = line.substr(2);
			int num = 0, x = 0, y = 0;
			vector<int> v;
			while (s.find('/',x) != string::npos){					
				num++;									// 记录这个面是 3 个顶点还是 4 个顶点
				y = s.find('/',x);						// 找顶点的 v 索引
				v.push_back(stoi(s.substr(x,y-x))-1);	// -1 因为数组索引从 0 开始
				x = s.find('/',y+1)+1;
				y = s.find(' ',x);						// 找顶点的法向量 vn 索引
				v.push_back(stoi(s.substr(x,y-x))-1);
				x = y+1;
			}
		
			if(num == 3){ 								// 该面是 3 个顶点
				face3 face;
				for(int i = 0; i < 3; i++){
					face.vIndex[i] = v[i*2];			// 面的其中一个顶点坐标
					face.vnIndex[i] = v[i*2+1];			// 该顶点的法向量坐标
				}
				f3.push_back(face);						// 记录到存储面的三个顶点的 vector
			}else if(num == 4){ 						// 该面是 4 个顶点
				face4 face;
				for(int i = 0; i < 4; i++){
					face.vIndex[i] = v[i*2];			// 面的其中一个顶点坐标
					face.vnIndex[i] =  v[i*2+1];		// 该顶点的法向量坐标
				}
				f4.push_back(face);						// 记录到存储面的四个顶点的 vector
			} 
		}
	}
	file.close();
}

void ObjLoader::Draw()
{
	// 绘制每个面的三个顶点
	for (int i = 0; i < f3.size(); i++) {
		glBegin(GL_POLYGON);  // 开始绘制

		glNormal3f( vn[f3[i].vnIndex[0]].x, vn[f3[i].vnIndex[0]].y, vn[f3[i].vnIndex[0]].z );	// 第一个点的法向量信息
		glVertex3f( v[f3[i].vIndex[0]].x, v[f3[i].vIndex[0]].y, v[f3[i].vIndex[0]].z );			// 第一个点的坐标

		glNormal3f( vn[f3[i].vnIndex[1]].x, vn[f3[i].vnIndex[1]].y, vn[f3[1].vnIndex[1]].z );	// 第二个点的法向量信息
		glVertex3f( v[f3[i].vIndex[1]].x, v[f3[i].vIndex[1]].y, v[f3[i].vIndex[1]].z );  		// 第二个点的坐标

		glNormal3f( vn[f3[i].vnIndex[2]].x, vn[f3[i].vnIndex[2]].y, vn[f3[i].vnIndex[2]].z );	// 第三个点的法向量信息
		glVertex3f( v[f3[i].vIndex[2]].x, v[f3[i].vIndex[2]].y, v[f3[i].vIndex[2]].z );			// 第三个点的坐标

		glEnd();
	}

		// 绘制每个面的四个顶点
	for (int i = 0; i < f4.size(); i++) {
		glBegin(GL_POLYGON);  // 开始绘制

		glNormal3f( vn[f4[i].vnIndex[0]].x, vn[f4[i].vnIndex[0]].y, vn[f4[i].vnIndex[0]].z );	// 第一个点的法向量信息
		glVertex3f( v[f4[i].vIndex[0]].x, v[f4[i].vIndex[0]].y, v[f4[i].vIndex[0]].z );			// 第一个点的坐标

		glNormal3f( vn[f4[i].vnIndex[1]].x, vn[f4[i].vnIndex[1]].y, vn[f4[1].vnIndex[1]].z );	// 第二个点的法向量信息
		glVertex3f( v[f4[i].vIndex[1]].x, v[f4[i].vIndex[1]].y, v[f4[i].vIndex[1]].z );			// 第二个点的坐标

		glNormal3f( vn[f4[i].vnIndex[2]].x, vn[f4[i].vnIndex[2]].y, vn[f4[i].vnIndex[2]].z );	// 第三个点的法向量信息
		glVertex3f( v[f4[i].vIndex[2]].x, v[f4[i].vIndex[2]].y, v[f4[i].vIndex[2]].z );			// 第三个点的坐标

		glNormal3f( vn[f4[i].vnIndex[3]].x, vn[f4[i].vnIndex[3]].y, vn[f4[i].vnIndex[3]].z );	// 第四个点的法向量信息
		glVertex3f( v[f4[i].vIndex[3]].x, v[f4[i].vIndex[3]].y, v[f4[i].vIndex[3]].z );			// 第四个点的坐标

		glEnd();
	}
	
}

