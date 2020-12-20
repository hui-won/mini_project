#pragma once
#include <iostream>
using namespace std;

int cur_interface = 1;//구조 모드 or 굽기모드
bool active_manual = false;

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glPushAttrib(GL_LIGHTING_BIT);//조명의 속성을 push
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-5, 5, -5, 5);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void print_instruction() {//콘솔 출력 매뉴얼
	cout << "+------------mouse/keyboard manual------------+\n";
	cout << "| n : manual 출력                             |\n";
	cout << "| m : 사용 방법 출력                          |\n";
	cout << "| mouse wheel up: zoom in                     |\n";
	cout << "| mouse wheel down: zoom out                  |\n";
	cout << "| z : zoom in                                 |\n";
	cout << "| x : zoom out                                |\n";
	cout << "| 방향키: camera 위치                         |\n";
	cout << "| a / w / s / d : 직교 좌표계 이동            |\n";
	cout << "+---------------------------------------------+\n";
	cout << "| mouse right click: menu                     |\n";
	cout << "| r : Retry 처음부터 다시                     |\n";
	cout << "| c : 부탄가스 종류 바꾸기                    |\n";
	cout << "| F1: 기본뷰                                  |\n";
	cout << "| F2: 항공뷰                                  |\n";
	cout << "| 0 / 1 / 2 / 3 : 불 세기 조절                |\n";
	if (cur_interface == 2) {
		cout << "| o : sound on/off                            |\n";
		cout << "| f : 고기 뒤집기                             |\n";
		cout << "| Enter: 게임 종료                            |\n";
	}
	cout << "+---------------------------------------------+\n";
}

void draw_manual1() {//interface1에서 출력할 매뉴얼
	if (active_manual == true) {
		char buf[9][70] = { 0 };
		strcpy(buf[0], "-----------------------Usage(Gas Burner)------------------------");
		strcpy(buf[1], " 1.In order to install gas");
		strcpy(buf[2], "  - lid of the gas and main body must be open");
		strcpy(buf[3], "  - gas controller and the hole of the gas must fit");
		strcpy(buf[4], " 2.In order to turn on the fire ");
		strcpy(buf[5], "  - controller should be vertical in order to lower the lever");
		strcpy(buf[6], " 3.In order to finishing using");
		strcpy(buf[7], "  - controller should be vertical in order to higher the lever");
		strcpy(buf[8], "  - lever must be up in order to remove the gas");

		for (int i = 0; i < 9; i++) {
			draw_string(GLUT_BITMAP_8_BY_13, buf[i], -4.7, -2 - 0.3 * i, 0, 0, 0);
		}

		draw_string(GLUT_BITMAP_TIMES_ROMAN_24,"If you don't keep it,",1.4, -3, 1, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "it won't work!!", 1.6, -3.5, 1, 0, 0);
	}
}
void draw_manual2() {//interface2에서 출력할 매뉴얼
	if (active_manual == true) {
		char buf[7][70] = { 0 };
		strcpy(buf[0], "---------------------how to play the game---------------------");
		strcpy(buf[1], " - If you want to put on the meat on frypan, click theh meat");
		strcpy(buf[2], "     *If the trivet is up side down, frypan won't move*");
		strcpy(buf[3], " - Control the fire to cook the meat");
		strcpy(buf[4], " - Turn the meat over to cook it deliciously");
		strcpy(buf[5], " - If you think you're done, click the meat and move it to dish");
		strcpy(buf[6], " - Press Enter key to view the result");

		for (int i = 0; i < 7; i++) {
			draw_string(GLUT_BITMAP_8_BY_13, buf[i], -4.5, 4 - 0.3 * i, 0, 0, 0);
		}
	}
}

