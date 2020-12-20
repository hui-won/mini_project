#pragma once
#include "bmpfuncs.h"
#include "ObjParser.h"

class MyObject {
private:
	char buf[3][50] = { 0 };//텍스쳐 이름
	int NumOfTex;//오브젝트가 가진 텍스쳐의 개수

public:
	int choice;//선택된 텍스쳐
	GLuint texture;
	ObjParser* obj;
	
	int state;//상태
	bool substate;//부가상태--> meat에서 사용함. 
	float pos_x, pos_y, pos_z;//상태를 변화시킬 때 필요한 변수(meat에서는 각각 rgb로 사용)
	float angle;//상태 변화시 사용

	MyObject(int count);
	void getResource(const char*);
	void getTexture1(const char*);
	void getTexture2(const char*);
	void getTexture3(const char*);//한 obj당 최대 텍스쳐는 3개
	void setTextureMapping();//obj의 텍스쳐 맵핑
	void draw_obj(ObjParser* objParser);
	void draw_obj_with_texture(ObjParser* objParser);
};

MyObject::MyObject(int count) {
	choice = 0;
	NumOfTex = count;//객체 선언할 때 필요한 텍스쳐의 개수 함께 받기
	state = 0;
	substate = false;
	pos_x = 0, pos_y = 0, pos_z = 0;

}
void MyObject::getResource(const char* objname) {
	this->obj = new ObjParser(objname);
}
void MyObject::getTexture1(const char* textureName) {
	strcpy(buf[0], textureName);
	buf[0][strlen(buf[0])] = 0;
}
void MyObject::getTexture2(const char* textureName) {
	strcpy(buf[1], textureName);
	buf[1][strlen(buf[1])] = 0;
}
void MyObject::getTexture3(const char* textureName) {
	strcpy(buf[2], textureName);
	buf[2][strlen(buf[2])] = 0;
}
void MyObject::setTextureMapping() {
	int imgWidth, imgHeight, channels;
	uchar* img;
	char buf2[50];
	for (int i = 0; i < NumOfTex; i++) {
		if (this->choice == i) {
			strcpy(buf2, buf[i]);
		}
	}
	img = readImageData(buf2, &imgWidth, &imgHeight, &channels);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//printf("%s texture setting complete\n", buf2);
}
void MyObject::draw_obj(ObjParser* objParser) {
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
}
void MyObject:: draw_obj_with_texture(ObjParser* objParser)
{
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);	// texture 색 보존을 위한 enable

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glTexCoord2f(objParser->textures[objParser->textureIdx[n] - 1].x,
			objParser->textures[objParser->textureIdx[n] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 1] - 1].x,
			objParser->textures[objParser->textureIdx[n + 1] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 2] - 1].x,
			objParser->textures[objParser->textureIdx[n + 2] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();

	glEnable(GL_BLEND);
}
