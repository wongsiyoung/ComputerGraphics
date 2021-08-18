#ifndef GLSKYBOX
#define GLSKYBOX
#include "base.h"
#include "texture.h"

void drawSkyBox(Texture *tex,float x,float y,float z,float width,float height,float len)
{
  tex->BindTexture(tex->m_tex);	// ��������

  //��ȡ���ĵ�
  x=x-width/2;
  y=y-height/2;
  z=z-len/2;

  glPushMatrix();
  //back face
  glBegin(GL_QUADS);
    glNormal3f(0.0,0.0,1.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x+width, y, z);
		
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x+width, y+height, z);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y+height, z);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, z);
  glEnd();
  //front face
  glBegin(GL_QUADS);
    glNormal3f(0.0,0.0,-1.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y, z+len);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x, y+height, z+len);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x+width, y+height, z+len);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x+width, y, z+len);
  glEnd();
  //bottom face
  glBegin(GL_QUADS);
    glNormal3f(0.0,1.0,0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y, z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x, y, z+len);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x+width, y, z+len);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x+width, y, z);
  glEnd();
  //top face
  glBegin(GL_QUADS);
    glNormal3f(0.0,-1.0,0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x+width, y+height, z);

		
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x+width, y+height, z+len);

		
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y+height, z+len);

		
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y+height, z);
  glEnd();
  //left face
  glBegin(GL_QUADS);
    glNormal3f(1.0,0.0,0.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y+height, z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x, y+height, z+len);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y, z+len);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, z);
  glEnd();

  //right face
  glBegin(GL_QUADS);
    glNormal3f(0.0,0.0,-1.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x+width, y, z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x+width, y, z+len);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x+width, y+height, z+len);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x+width, y+height, z);
  glEnd();
  glPopMatrix();
  tex->BindTexture(0);		// ȡ��������
}
#endif