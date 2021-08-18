#ifndef GLWARP_H
#define GLWARP_H
#include "base.h"

class Texture{
public:
  	GLuint m_tex;
	Texture(){m_tex=0;}

	Texture(const char* fname,bool make_mipmaps=true);

	~Texture();
	
	void BindTexture(GLuint ID);
};


#endif