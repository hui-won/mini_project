#pragma once

#include <GL/glut.h>
#include "MyObject.h"
#include "GL/glext.h"
GLuint g_nCubeTex;

void cubeEnvironmentTexture() {
	glGenTextures(1, &g_nCubeTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	int width, height, channels;
	uchar* img;
	for (int i = 0; i < 6; i++) {
		char buf[100];
		sprintf(buf, "img/rooom%d.bmp", i);
		buf[strlen(buf)] = 0;
		img = readImageData(buf, &width, &height, &channels);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

void draw_skybox() {//skybox±×¸®±â
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);

	float g_nSkySize = 100;
	glBegin(GL_QUADS);
	//px
	glTexCoord3f(1, -1, -1); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(1, -1, 1); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(1, 1, 1); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(1, 1, -1); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	//nx
	glTexCoord3f(-1, -1, -1); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(-1, -1, 1); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1, 1, 1); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(-1, 1, -1); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);

	//py
	glTexCoord3f(1, 1, 1); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(-1, 1, 1); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(-1, 1, -1); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3f(1, 1, -1); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	//ny
	glTexCoord3f(1, -1, 1); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1, -1, 1); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1, -1, -1); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(1, -1, -1); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);

	//pz
	glTexCoord3f(1, -1, 1); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1, -1, 1); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3f(-1, 1, 1); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3f(1, 1, 1); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);

	//nz
	glTexCoord3f(1, -1, -1); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(-1, -1, -1); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3f(-1, 1, -1); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3f(1, 1, -1); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	glEnd();
	glDisable(GL_TEXTURE_CUBE_MAP);
}
