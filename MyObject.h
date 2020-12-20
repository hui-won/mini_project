#pragma once
#include "bmpfuncs.h"
#include "ObjParser.h"

class MyObject {
private:
	char buf[3][50] = { 0 };//�ؽ��� �̸�
	int NumOfTex;//������Ʈ�� ���� �ؽ����� ����

public:
	int choice;//���õ� �ؽ���
	GLuint texture;
	ObjParser* obj;
	
	int state;//����
	bool substate;//�ΰ�����--> meat���� �����. 
	float pos_x, pos_y, pos_z;//���¸� ��ȭ��ų �� �ʿ��� ����(meat������ ���� rgb�� ���)
	float angle;//���� ��ȭ�� ���

	MyObject(int count);
	void getResource(const char*);
	void getTexture1(const char*);
	void getTexture2(const char*);
	void getTexture3(const char*);//�� obj�� �ִ� �ؽ��Ĵ� 3��
	void setTextureMapping();//obj�� �ؽ��� ����
	void draw_obj(ObjParser* objParser);
	void draw_obj_with_texture(ObjParser* objParser);
};

MyObject::MyObject(int count) {
	choice = 0;
	NumOfTex = count;//��ü ������ �� �ʿ��� �ؽ����� ���� �Բ� �ޱ�
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT ���� �ϳ� ����
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
	glEnable(GL_TEXTURE_2D);	// texture �� ������ ���� enable

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
