#include "texture.h"
#include "GL/lodepng.h"
#include <vector>
#include <iostream>

/*###################################################
##  函数: Texture
##  函数描述： Texture 类构造函数，读入 png 图像绑定纹理
##  参数描述： 
##			fname:文件名
##			make_mipmaps：是否设置 mipmap
#####################################################*/
Texture::Texture(const char *fname,bool make_mipmaps)
{
	printf("%s\n",fname);
	std::vector<unsigned char> image;
	unsigned int width, height;
	unsigned int error = lodepng::decode(image, width, height, fname);	// 解码 png 图像

	if (error != 0)
	{
		std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
		return;
	}

	m_tex = 0;
	// 设置纹理空间并开启
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	// 设置纹理属性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// 是否设置 mipmap
	if (make_mipmaps)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	}
	else
	{
	
		double xPow2, yPow2;
		int ixPow2, iyPow2;
		int xSize2, ySize2;
		unsigned char* pData = NULL;

		GLint glMaxTexDim;
		// 获取 opengl 支持的最大纹理size
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);

		// 把图像长宽转换成最近的2的幂
		if (width <= glMaxTexDim)
			xPow2 = log((double)width) / log((double)2.0);
		else
			xPow2 = log((double)glMaxTexDim) / log((double)2.0);

		if (height <= glMaxTexDim)
			yPow2 = log((double)height) / log((double)2.0);
		else
			yPow2 = log((double)glMaxTexDim) / log((double)2.0);

		ixPow2 = (int)xPow2;
		iyPow2 = (int)yPow2;

		if (xPow2 != (double)ixPow2)
			ixPow2++;
		if (yPow2 != (double)iyPow2)
			iyPow2++;
		xSize2 = 1 << ixPow2;
		ySize2 = 1 << iyPow2;


		// 如果改变的尺寸和原图像不一样则 resize rgba 图像
		if (xSize2 != width || ySize2 != height)
		{
			pData = (unsigned char*)malloc(xSize2 * ySize2 * 4 * sizeof(unsigned char));

			if (!pData)
				return;

			gluScaleImage(GL_RGBA,
				width,
				height,
				GL_UNSIGNED_BYTE,
				&image[0],
				xSize2,
				ySize2,
				GL_UNSIGNED_BYTE,
				pData);

			width = xSize2;
			height = ySize2;
		}
		
		glTexImage2D(GL_TEXTURE_2D,
			0, 4, width,height,
			0, GL_RGB, GL_UNSIGNED_BYTE,pData);

		if (pData)
		{
			free(pData);
			pData = NULL;
		}
	}	
}

/*###################################################
##  函数: BindTexture
##  函数描述： 绑定纹理
##  参数描述： 
##			ID: 纹理 ID
#####################################################*/
void Texture::BindTexture(GLuint ID)
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

/*###################################################
##  函数: ~Texture
##  函数描述： Texture 类销毁函数
##  参数描述： 
#####################################################*/
Texture::~Texture()
{
	glDeleteTextures(1, &m_tex);
}

