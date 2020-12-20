#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "MyObject.h"
#include "Camera.h"
#include "skybox.h"
#include "DrawString.h"
#include <mmsystem.h>
#include "GL/glext.h"

#pragma comment(lib,"winmm.lib")//sound file�� ����ϱ� ����

int window_width = 900;
int window_height=700;

enum {//pick�ϴ� ��ü�� ���� �˾ƺ��� ����
	base_cap1 = 1, lever1, controller1, trivet1, gas_cap1, gas1,
	meat1 = 11, frypan1, dish1
};

bool default_setting = 0;//interface2������ default setting ������ ���� ����
int cur_state = 0;//�޴� �ɼ�
extern int cur_interface;//���� ��� or ������

int obj_x, obj_y;
int sel = 0;//pick�ϴ� ��ü�� name

extern bool active_manual;//manual on/off
bool fire_on = false;//fire on/off
bool sound_on = true;//sound on/off

/*texture mapping*/
GLuint texDisk;//base�� disk

/*obj var*/
GLUquadricObj* cylinder = gluNewQuadric();
GLUquadricObj* disk = gluNewQuadric();

MyObject gas(3);
MyObject base(1);
MyObject base_cap(1);
MyObject gas_controller(1);
MyObject burner(1);
MyObject lever(1);
MyObject controller(1);
MyObject trivet(1);
MyObject fire(3);
MyObject gas_cap(1);
MyObject newspaper(1);
MyObject frypan(1);
MyObject meat(1);
MyObject dish(1);


/*�Լ� ����*/

void init();
void add_menu();
void main_menu_function(int option);
void sub_menu_function0(int option);
void sub_menu_function1(int option);
void sub_menu_function2(int option);

/*callback*/
void resize(int width, int height);
void draw();
void idle();
void dir_key(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

void light_default();
void print_instruction();//�ܼ�â�� �Ŵ��� ���

/*objParser*/
void draw_cylinder();
void draw_disk();
void setDiskTexture();
void draw_sound();

void get_obj_resource();//obj, texture �ҷ�����
void set_obj_texture();//obj& texture ����

/*draw obj*/
void draw_interface1();//1�� �������̽������� default
void draw_interface2();//obj�� 2�� �������̽� default
void obj_state_change();//interaction�� ���� object���� ���� ��ȭ
void obj_state_init();//�ʱ� ���� ����
void obj_state_setting();//��� ���� �� ���� ����

/*���*/
void picking(int ,int);

void draw_endmsg();//��� �޽��� ���
void draw_multiview();//��Ƽ �� ����
void play_sound();


int main(int argc, char** argv) {

	/*Window �ʱ�ȭ*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);//�ִϸ��̼� ����, flikering ���ֱ�
	glutInitWindowSize(window_width, window_height);//â�� ũ�⸦ 500x500
	glutInitWindowPosition(250, 100);//â�� ��ġ�� 250,250
	glutCreateWindow("12181769_����� mini project");
	/*����� �ʱ�ȭ �Լ�*/
	init();
	/*pop-up �޴� ���*/
	add_menu();

	/*Callback�Լ� ����*/
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutSpecialFunc(dir_key);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);

	/*Looping ����*/
	glutMainLoop();
	
	return 0;
}

void init() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//ȭ�� �⺻�� white
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0f, 1.0f, 1.0f);//object �⺻�� white

	light_default();//light, material �Ӽ� ����

	print_instruction();//�ֿܼ� �Ŵ��� ���
	get_obj_resource();//object, texture ��������
	
	/*texture mapping set*/
	gluQuadricTexture(cylinder, GL_TRUE);
	gluQuadricTexture(disk, GL_TRUE);
	
	//setTextureMapping();
	set_obj_texture();//obj�� texture ���� ��Ű��
	cubeEnvironmentTexture();//sky box texture ����

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}

void resize(int width, int height) {
	window_width = width;
	window_height = height;
	glViewport(0, 0, width, height);//viewport transform
	glMatrixMode(GL_PROJECTION);//projection transform projection matrix�� Ȱ��ȭ ��Ų �� ����
	glLoadIdentity();//�ʱ�ȭ
	gluPerspective(45, (float)width / (float)height, 1, 500);//projection transform

	glMatrixMode(GL_MODELVIEW);

	printf("resize\n");
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//���� ���۸� ����

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(x_axis, y_axis, 0);//���� ��ǥ�� �̵��� �����ϱ� ����
	/*pop up menu in detail*/
	if (cur_interface == 1) {
		draw_manual1();//1�� �������̽��� ���� �Ŵ��� �����쿡 ���

		calculate_axis();//ī�޶� ���� ���
		gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);

		switch (cur_state) {
		case 0:
			draw_interface1();//1�� �������̽� �׸���
			break;
		case 1://base case
			//��ü+disk+�Ѳ� 
			glColor3f(1.0f, 1.0f, 1.0f);
			base.draw_obj_with_texture(base.obj);
			draw_disk();//���� ���� Ȯ���� ���� ��ƼĿ ������ ����
			glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);
			glTranslatef(5.4, 0.15, 0);
			glRotatef(-45, 0, 0, 1);
			base_cap.draw_obj_with_texture(base_cap.obj);
			glPopMatrix();
			break;
		case 2://burner
			glPushMatrix();
			glColor3f(0.5f, 0.5f, 0.5f);
			glRotatef(90, 0, 1, 0);
			burner.draw_obj_with_texture(burner.obj);
			glPopMatrix();
			break;
		case 3://gas controller
			glPushMatrix();
			glColor3f(0.5f, 0.5f, 0.5f);
			gas_controller.draw_obj_with_texture(gas_controller.obj);
			glPopMatrix();
			break;
		case 4://trivet
			glPushMatrix();
			glColor3f(1, 1, 1);
			trivet.draw_obj_with_texture(trivet.obj);
			glPopMatrix();
			break;
		case 5://lever & controller
			glPushMatrix();
			glColor3f(0.7f, 0.7f, 0.7f);
			glScalef(4, 4, 4);
			glTranslatef(-1, 0, 0);
			lever.draw_obj_with_texture(lever.obj);
			glColor3f(1, 1, 1);
			glTranslatef(2, 0, 0);
			controller.draw_obj_with_texture(controller.obj);
			glPopMatrix();
			break;
		case 6://gas
			//��ü+cylinder+�Ѳ�
			glPushMatrix();
			glTranslatef(0, -1.9, 0);
			glPushMatrix();
			glColor3f(1, 1, 1);
			glTranslatef(2, 0, 0);
			glRotatef(180, 0, 1, 0);
			gas_cap.draw_obj_with_texture(gas_cap.obj);
			glPopMatrix();
			glPushMatrix();
			draw_cylinder();//������ �ؽ��� ������ ���� �Ǹ��� 
			glColor3f(0.5f, 0.5f, 0.5f);
			gas.draw_obj(gas.obj);
			glPopMatrix();
			glPopMatrix();
			break;
		}
	}
	else if (cur_interface == 2) {

		draw_manual2();//2�� �������̽��� ���� �Ŵ��� ���

		draw_multiview();//multi viewport ����

		glViewport(0, 0, window_width, window_height);
		glLoadIdentity();

		calculate_axis();
		gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);

		glTranslatef(x_axis, y_axis, 0);//������ǥ�� �̵�
		
		draw_interface2();
		draw_endmsg();//������ �Ϸ��Ͽ��� ���� �޽��� ���
		draw_sound();


		/*skybox �׸���*/
		glPushMatrix();
		glPushAttrib(GL_LIGHTING);//skybox�� ���� ���� ȿ���� �����ϱ� ����
		glTranslatef(0, 39.8, 0);
		glScalef(0.5, 0.4, 0.5);
		draw_skybox();
		glPopAttrib();
		glPopMatrix();
	}
	glFlush();
	glutSwapBuffers();
}

void picking(int x, int y) {
	GLuint selectBuf[100] = { 0 };
	glSelectBuffer(100, selectBuf);

	glRenderMode(GL_SELECT);
	glMatrixMode(GL_PROJECTION);

	glInitNames();

	glPushName(-1);

	glPushMatrix();
	glLoadIdentity();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluPickMatrix(x, y, 0.1, 0.1, viewport);
	gluPerspective(45, (float)window_width / (float)window_height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	
	if (cur_interface == 1) draw_interface1();
	else if (cur_interface == 2) {
		draw_interface2();//�� �������̽��� �ش��ϴ� obj �׸���
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits = glRenderMode(GL_RENDER);

	if (hits <= 0) return;

	int stack = selectBuf[0];
	unsigned int zMin = selectBuf[1];
	sel = selectBuf[3];

	int index = 3 + stack;
	int i;
	for (i = 1; i < hits; i++) {
		stack = selectBuf[index];
		if (zMin > selectBuf[index + 1]) {
			zMin = selectBuf[index + 1];
			sel = selectBuf[index + 3];
		}
		index += 3 + stack;
	}//�տ� �ִ� obj�� ���� �����ϱ� ���� �ۼ�
	printf("Select: %d\n", sel);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'n') {//�ֿܼ� �Ŵ��� ���
		print_instruction();
	}

	if (key == 'm') {//�����쿡 �Ŵ��� ���--> �������̽����� �Ŵ����� �޶���
		active_manual = !(active_manual);
	}

	if (key == 'o') {
		sound_on = !sound_on;
		cout << "sound " << sound_on << endl;
	}

	if (key == 'c') {//gas change
		printf("gas.choice:%d\n", gas.choice);
		if (gas.choice == 2) {
			gas.choice = 0;
		}
		else{
			gas.choice++;
		}
		gas.setTextureMapping();
	}

	if (key == 'f') {//��� ������
		if (meat.state == 1) {
			meat.substate = !(meat.substate);
			printf("meat.substate: %d\n", meat.substate);
		}
	}
	
	/*power of fire*/
	if (key == '0') {
		printf("fire off\n");
		controller.state = 0;
		fire_on = false;
	}
	else if (key == '1') {
		if (lever.state == 1 && gas.state == 2) {//������ �������ְ�, ������ ��ü�� ����Ǿ��־�߸� ���� ����
			fire_on = true;
			fire.choice = 0;
			fire.setTextureMapping();
			printf("fire power:1\n");
		}
		controller.state = 1;
	}
	else if (key == '2') {
		if (lever.state == 1 && gas.state == 2) {
			fire_on = true;
			fire.choice = 1;
			printf("fire power:2\n");
			fire.setTextureMapping();
		}
		controller.state = 2;
	}
	else if (key == '3') {
		if (lever.state == 1 && gas.state == 2) {
			fire_on = true;
			fire.choice = 2;
			printf("fire power:3\n");
			fire.setTextureMapping();
		}
		controller.state = 3;
	}
	
	if (key == 'r') {//�ʱ� ���·� �ǵ�����(reset or retry)
		if (cur_interface == 1) init_view_dir();
		else if (cur_interface == 2) init_view_int2();
		if (default_setting) obj_state_setting();
		else obj_state_init();
	}

	/*zoom in zoom out*/
	if (key == 'z') {
		if (radius > 4) {
			radius -= 1;
		}
	}
	if (key == 'x') {
		radius += 1;
	}
	/*������ǥ�̵�*/
	if (key == 'a') {
		x_axis -= 0.5;
	}
	if (key == 'd') {
		x_axis += 0.5;
	}
	if (key == 'w') {
		y_axis += 0.5;
	}
	if (key == 's') {
		y_axis -= 0.5;
	}

	if (key == 13) {//enter�� Ŭ���ϸ� ���� ��
		if (meat.state == 0) {
			meat.state = 2;
			dish.state = 1;
			init_view_int2();
		}
	}

	play_sound();
	glFlush();
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y) {

	if (button == 3) {//��
		if (radius > 4) {
			radius -= 1;
		}
	}
	if (button == 4) {
		radius += 1;
	}

	y = window_height - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		picking(x, y);
		obj_x = x;
		obj_y = y;
		obj_state_change();
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		sel = 0;
	}

	play_sound();
	
}
void dir_key(int key, int x, int y) {

	if (key == GLUT_KEY_LEFT) {
		phi -= 5;
	}
	if (key == GLUT_KEY_RIGHT) {
		phi += 5;
	}
	if (key == GLUT_KEY_UP) {
		theta += 5;
	}
	if (key == GLUT_KEY_DOWN) {
		theta -= 5;
	}

	if (phi > 360) {
		phi -= 360;
	}
	if (phi < 0) {
		phi += 360;
	}
	if (theta > 360) {
		theta -= 360;
	}
	if (theta <= 0) {
		theta += 360;
	}

	if (key == GLUT_KEY_F1) {//�ʱ� �������� �̵�
		if (cur_interface == 1) init_view_dir();
		else if (cur_interface == 2) init_view_int2();
		printf("default view\n");
	}
	else if (key == GLUT_KEY_F2) {//�װ���
		aerial_view();
		printf("aerial view\n");
	}
	
	glFlush();
	glutPostRedisplay();
}

void play_sound() {
	if (fire_on == true && meat.state == 1 &&sound_on==true ) {
		PlaySound(TEXT("sound/Frying_Food.wav"), NULL, SND_ASYNC | SND_LOOP|SND_NOSTOP);
		//PlaySound(TEXT("sound/Frying_Food.wav"), NULL, SND_ASYNC | SND_LOOP);
	}
	else
		PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);
}
void add_menu() {
	int submenu1 = glutCreateMenu(sub_menu_function1);
	glutAddMenuEntry("base case", 1);
	glutAddMenuEntry("burner", 2);
	glutAddMenuEntry("gas controller", 3);
	glutAddMenuEntry("trivet", 4);
	glutAddMenuEntry("lever & controller", 5);
	glutAddMenuEntry("gas", 6);

	int submenu0 = glutCreateMenu(sub_menu_function0);
	glutAddMenuEntry("entire configuration", 0);
	glutAddSubMenu("in detail", submenu1);

	int submenu2= glutCreateMenu(sub_menu_function2);
	glutAddMenuEntry("Default setting X", 20);
	glutAddMenuEntry("Default setting O", 21);
	
	glutCreateMenu(main_menu_function);
	glutAddSubMenu("Catalog", submenu0);
	glutAddSubMenu("Grilling meat", submenu2);
	glutAddMenuEntry("QUIT", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);//��Ŭ�� �� �� �޴� ���̱�
}

void main_menu_function(int option) {

	if (option == 999) {
		printf("You selected QUIT\n");
		exit(0);
	}
	glFlush();
}
void sub_menu_function0(int option) {
	if (option == 0) {
		cur_state = 0;//interface 1���� ���¸� Ȯ���ϱ� ���� ����
		cur_interface = 1;//1�� �������̽����� ��Ÿ��
		default_setting = 0;//default setting ���� ����
		init_view_dir();//ī�޶� �ʱ� ��������
		obj_state_init();//onj �ʱ� ���·�
		PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);
	}
	glFlush();
}
void sub_menu_function1(int option) {
	for (int i = 1; i <= 6; i++) {//6���� submenu Ŭ��
		if (option == i) {
			init_view_dir();//ī�޶� �ʱ� �������� ������
			cur_state = i;
			printf("current state:%d\n", i);
		}
	}
	glFlush();
}
void sub_menu_function2(int option) {
	if (option == 20) {
		cur_interface = 2;//2�� �������̽�
		default_setting = 0;//���� ���� ����
		init_view_int2();//2�� �������̽��� ī�޶� �ʱ� ����
		obj_state_init();//obj �ʱ�ȭ
	}
	if (option == 21) {
		cur_interface = 2;
		default_setting = 1;//���� ������
		init_view_int2();//
		obj_state_setting();//���õ� ������ obj
	}
	glFlush();
}

void light_default() {
	GLfloat light_ambient[] = { 0.3f,0.3f,0.3f,1.0f };
	GLfloat light_diffuse[] = { 0.7f,0.7f,0.7f,1.0f };
	GLfloat light_specular[] = { 1.0f,1.0f,1.0f,1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	GLfloat ambient[] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat specular[] = { 1.0f,1.0f,1.0f,1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void idle() {
	/*������ ������ �ִϸ��̼�*/
	if (lever.state == 0) {
		if (lever.pos_y <= -0.1) {
			lever.pos_y += 0.1;
		}
	}
	else if (lever.state == 1) {
		if (lever.pos_y > - 0.6) {
			lever.pos_y -= 0.1;
		}
	}
	/*���� �Ѳ��� ������ �ִϸ��̼�*/
	if (gas_cap.state == 1) {
		if (gas_cap.pos_y < 0.6) {
			gas_cap.pos_y += 0.1;
		}
	}
	/*�������� ������ �ִϸ��̼�*/
	if (controller.state == 0) {
		if (controller.angle > 0) {
			controller.angle -= 9;
		}
	}
	else if (controller.state == 1) {
		if (controller.angle > 45) {
			controller.angle -= 9;
		}
		else if (controller.angle < 45) {
			controller.angle += 9;
		}
	}
	else if (controller.state == 2) {
		if (controller.angle > 90) {
			controller.angle -= 9;
		}
		else if (controller.angle < 90) {
			controller.angle += 9;
		}
	}
	else if (controller.state == 3) {

		if (controller.angle <= 135) {
			controller.angle += 9;
		}
	}
	/*��ü �Ѳ��� ������ �ִϸ��̼�*/
	if (base_cap.state == 0) {
		if (base_cap.angle < 0) {
			base_cap.angle += 15;
		}
	}
	else if (base_cap.state == 1) {
		if (base_cap.angle >= -75) {
			base_cap.angle -= 15;
		}
	}
	/*���� ��ü ���� ���� ȸ�� ����*/
	if (gas.state == 0 || gas.state == 1) {
		if (gas.angle > 0) {
			gas.angle -= 5;
		}
	}
	else if (gas.state == 2) {
		if (gas.angle <= 90) {
			gas.angle += 5;
		}
	}
	/*��Ⱑ ���� ���⿡ ���� �ʹ� �ӵ��� �޶����� ǥ��*/
	if (fire_on == true &&meat.state==1) {
		
		if (fire.choice == 0) {
			meat.pos_x -= 0.0005;
			meat.pos_y -= 0.0007;
			meat.pos_z -= 0.001;
		}
		else if (fire.choice == 1) {
			meat.pos_x -= 0.001;
			meat.pos_y -= 0.0014;
			meat.pos_z -= 0.002;
		}
		else if (fire.choice == 2) {
			meat.pos_x -= 0.0015;
			meat.pos_y -= 0.0021;
			meat.pos_z -= 0.003;
		}
	}

	glutPostRedisplay();
}

void obj_state_init() {//��� �ʱ�ȭ
	lever.state = 0;
	trivet.state = 0;
	gas_cap.state = 0;
	controller.state = 0;
	base_cap.state = 0;
	gas.state = 0;
	frypan.state = 0;
	meat.state = 0;
	dish.state = 0;
	meat.pos_x = meat.pos_y = meat.pos_z = 1;
	active_manual = false;
	fire_on = false;
}

void obj_state_setting() {//setting �� ���·� ����--> ���� ���ǵ鿡 ���� �����ϴ� ���� �߿�!
	trivet.state = 1;
	gas_cap.state = 2;
	base_cap.state = 1;
	gas.state = 2;
	base_cap.state = 0;
	lever.state = 1;
	frypan.state = 1;
	controller.state = 0;
	meat.state = 0;
	dish.state = 0;
	meat.pos_x = meat.pos_y = meat.pos_z = 1;
	active_manual = false;
	fire_on = false;
}

void obj_state_change() {//obj ���콺 Ŭ�� ���� ���� ��ȭ
	if (sel == trivet1) {
		if (frypan.state == 0) {
			if (trivet.state == 1) {
				trivet.state = 0;
			}
			else if (trivet.state == 0) {
				trivet.state = 1;
			}
		}
		printf("trivet.state:%d\n", trivet.state);
	}
	if (sel == lever1) {
		if (gas.state!=1&&controller.state==0) {//������ ������ ������������ Ȩ�� ��ġ�ϰų�, �����Ⱑ ������ ���¿����� ������ �� ����
			if (lever.state == 0) lever.state = 1;
			else if (lever.state == 1)lever.state = 0;
			
			printf("lever.state:%d\n", lever.state);
		}
	}
	if (sel == gas_cap1) {
		if (gas_cap.state == 2) {
			if(gas.state==0) gas_cap.state = 0;//���� �Ѳ��� ������ ��ü �ٱ��� �������� �ٽ� ���� �� ����
		}
		else {
			gas_cap.state++;
		}
		printf("gas_cap.state:%d\n",gas_cap.state);
	}
	if (sel == controller1) {
		if (controller.state == 3) controller.state = 0;
		else controller.state++;
		printf("controller.state:%d\n", controller.state);
	}
	/*Ű���� 1230���� �Է¹޾��� ���� ���� ���� ��ȭ*/
	if (controller.state == 0) {
		fire_on = false;
	}
	else if (controller.state==1) {
		if (lever.state == 1&&gas.state==2) {//������ ��������, ������ ����� ������ �� ���¿����� ���� ����
			fire_on = true;
			fire.choice = 0;
			printf("fire power: 1\n");
			fire.setTextureMapping();
		}
	}
	else if (controller.state==2)
	{
		if (lever.state == 1 && gas.state == 2) {
			fire_on = true;
			fire.choice = 1;
			printf("fire power: 2\n");
			fire.setTextureMapping();
		}
	}
	else if (controller.state==3) {
		if (lever.state == 1 && gas.state == 2) {
			fire_on = true;
			fire.choice = 2;
			printf("fire power: 3\n");
			fire.setTextureMapping();
		}
	}

	if (sel == base_cap1) {
		if (base_cap.state == 0) base_cap.state = 1;
		else if (base_cap.state == 1) base_cap.state = 0;
	}

	if (sel == gas1) {
		if (gas_cap.state != 0&&base_cap.state!=0&&lever.state==0) {//������ ���� �Ѳ��� �����ְ�, ��ü�� �Ѳ��� �������� ���� �̵� ����
			if (gas.state == 2) {
				if(lever.state==0) gas.state = 0;//������ �������־�߸� �̵� ����
			}
			else gas.state++;
			cout << gas.state << endl;
		}
	}

	if (sel == frypan1) {
		if (frypan.state == 0&&trivet.state==1) frypan.state = 1;//���������� ����̰� ����� �� ���¿����� �÷��� �� ����
		else if (frypan.state == 1&&meat.state==0) frypan.state = 0;//��Ⱑ �÷��� ���¿��� ���������� ���� �� ����
		printf("frypan.state:%d\n", frypan.state);
	}

	if (sel == meat1) {//���� �Ķ������� ���� ���ʿ� �ö� ���¿��� �÷��� �� ����
		if (frypan.state == 1) {
			if (meat.state == 0) meat.state = 1;
			else if (meat.state == 1) meat.state = 0;
			printf("meat.state:%d\n", meat.state);
		}
	}
}
void draw_interface1() {

	/*base*/
	glPushMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	base.draw_obj_with_texture(base.obj);
	draw_disk();
	glPopMatrix();

	/*burner*/
	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	burner.draw_obj_with_texture(burner.obj);
	glPopMatrix();

	/*gas controller*/
	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(4.4, 0.15, 2.97);
	glRotatef(90, 0, 1, 0);
	gas_controller.draw_obj_with_texture(gas_controller.obj);
	glPopMatrix();

	/*base cap*/
	
	glPushMatrix();
	glLoadName(base_cap1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(5.4, 0.15, 0);

	glRotatef(base_cap.angle, 0, 0, 1);

	//glRotatef(-45, 0, 0, 1);
	base_cap.draw_obj_with_texture(base_cap.obj);
	glPopMatrix();

	/*lever*/
	
	glPushMatrix();
	glLoadName(lever1);
	glColor3f(0.7f, 0.7f, 0.7f);
	glTranslatef(3.3, 0, 3.9);

	glTranslatef(0, lever.pos_y, 0);
	glTranslatef(0, 1.2, 0);

	lever.draw_obj_with_texture(lever.obj);
	glPopMatrix();

	/*controller*/
	
	glPushMatrix();
	glLoadName(controller1);
	glColor3f(1, 1, 1);
	glTranslatef(4.37, 0.88, 3.9);
	glRotatef(controller.angle, 0, 0, 1);
	controller.draw_obj_with_texture(controller.obj);
	glPopMatrix();

	/*trivet*/
	
	glPushMatrix();
	glLoadName(trivet1);
	glColor3f(1, 1, 1);
	if (trivet.state == 1) {
		glTranslatef(0, 1.5, 0);//�÷��� ����
	}else if (trivet.state == 0) {
		glTranslatef(0, 1.8, 0);
		glRotatef(180, 1, 0, 0);
	}
	trivet.draw_obj_with_texture(trivet.obj);
	glPopMatrix();

	/*fire*/
	if (fire_on == true) {
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(0, -0.2, 0);
		glScalef(1.2, 1.2, 1.2);
		fire.draw_obj_with_texture(fire.obj);
		glPopMatrix();
	}

	glPushMatrix();//������ ���� �Ѳ� �Բ� �����̱� ���ؼ�
	glTranslatef(-6, 0, 0);
	/*gas cap*/
	glPushMatrix();
	glLoadName(gas_cap1);
	glColor3f(1, 1, 1);
	
	if (gas_cap.state == 1) {
		glTranslatef(0, gas_cap.pos_y, 0);
	}
	else if (gas_cap.state == 2) {
		glTranslatef(1, -3.8, 1);
	}
	else if(gas_cap.state==0){
		glTranslatef(0, 0, 0);
		gas_cap.pos_y = 0;
	}
	glTranslatef(0,3.8, 0);
	glRotatef(180, 0, 1, 0);
	gas_cap.draw_obj_with_texture(gas_cap.obj);
	glPopMatrix();

	/*gas*/
	glPushMatrix();
	glLoadName(gas1);
	
	if (gas.state == 0) {
		
	}
	else if (gas.state == 1) {
		glTranslatef(10.4, 0.88, -1.9);
		glRotatef(90, 1, 0, 0);
	}
	else if (gas.state == 2) {
		glTranslatef(10.4, 0.88, -1.9);
		glRotatef(90, 1, 0, 0);
		glRotatef(gas.angle, 0, 1, 0);
	}
	draw_cylinder();
	glColor3f(0.5f, 0.5f, 0.5f);
	gas.draw_obj(gas.obj);
	glPopMatrix();

	glPopMatrix();
}

void draw_interface2() {
	/*newspaper*/
	glPushMatrix();
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(-4, -0.05, 0.5);
	glRotatef(87, 0, 1, 0);
	newspaper.draw_obj_with_texture(newspaper.obj);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(7, -0.03, 0);
	glRotatef(-89, 0, 1, 0);
	newspaper.draw_obj_with_texture(newspaper.obj);
	glPopMatrix();
	glPopMatrix();
	
	draw_interface1();//1�� �������̽��� ���� �״�� �׷��ֱ�
	
	/*frypan*/
	glPushMatrix(); 
	glLoadName(frypan1);
	glColor3f(1, 1, 1);
	if (frypan.state == 0) {
		glTranslatef(-6.5, 0, 6);
		//glTranslatef(9, 0, 2);
		glRotatef(180, 0, 1, 0);
	}
	else if (frypan.state == 1) {
		glTranslatef(0, 2.3, 0);
	}
	frypan.draw_obj_with_texture(frypan.obj);
	glPopMatrix();

	/*���� ���� �Բ� �����̱�*/
	glPushMatrix();
	if (meat.state == 2 && dish.state == 1) {//������ ���� �� (enter�� �Է��Ͽ��� ��)
		glTranslatef(-8, 8, 10);
	}
	/*meat*/
	glPushMatrix();
	glLoadName(meat1);
	//meat������ pos_x,pos_y,pos_z�� ������� �����Ƿ� ������ r,g,b�� ����Ѵ�!!
	glColor3f(meat.pos_x, meat.pos_y, meat.pos_z);
	
	if (meat.state == 0) {
		glTranslatef(0, 0, 0);
	}
	else if (meat.state == 1) {
		
		glTranslatef(-8, 2.7, -5);
	}
	//0.75,0.65,0.5--> �߰����� ����� ���� ����
	//0.5,0.3,0--> ������ ����� ���� ����
	glTranslatef(8, 0.08, 5);
	glScalef(1.3, 1.3, 1.3);
	if (meat.state == 1) {
		if (meat.substate) glRotatef(180, 0, 0, 1);
		else glRotatef(180, 1, 0, 0);
	}
	meat.draw_obj_with_texture(meat.obj);
	glPopMatrix();

	/*dish*/
	glPushMatrix();
	glLoadName(dish1);
	glColor3f(1, 1, 1);
	glTranslatef(8, 0, 5);
	dish.draw_obj_with_texture(dish.obj);
	glPopMatrix();
	
	glPopMatrix();
}

void draw_endmsg() {//��� ���� ��� �޽��� ���
	char endmsg[30];
	if (meat.state == 2 && dish.state == 1) {
		if (meat.pos_x > 0.75 && meat.pos_y > 0.65 && meat.pos_z > 0.5) {
			strcpy(endmsg, "Too Raw!!!");//�ʹ� ������
		}
		else if (meat.pos_x <= 0.75 && meat.pos_x > 0.5 && meat.pos_y <= 0.65 && meat.pos_y > 0.3 && meat.pos_z <= 0.5) {
			strcpy(endmsg, "Good~~");//������
		}
		else if (meat.pos_x <= 0.5 && meat.pos_y <= 0.3) {
			strcpy(endmsg, "It's burnt,,,");//�¿�,,,
		}

		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, endmsg, -0.5, 2.5, 1, 1, 1);
	}
}

void draw_sound() {

	char msg[10];
	if (sound_on == true) {
		strcpy(msg, "Sound On");
	}
	else {
		strcpy(msg, "Sound Off");
	}

	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, msg, 3.5, -4.2, 1, 1, 1);
}


void setDiskTexture() {
	int imgWidth, imgHeight, channels;
	uchar* img;
	char buf[30];

	img = readImageData("texture/base2.bmp", &imgWidth, &imgHeight, &channels);

	glGenTextures(1, &texDisk);
	glBindTexture(GL_TEXTURE_2D, texDisk);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT ���� �ϳ� ����
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);
}

void draw_cylinder() {
	glColor3f(1, 1, 1);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, gas.texture);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, 0.05);
	gluCylinder(cylinder, 0.71, 0.71, 3.2, 16, 4);
	
	glPopMatrix();
}
void draw_disk() {
	glColor3f(1, 1, 1);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texDisk);
	glPushMatrix();
	glTranslatef(4.37, 0.87, 3.78);
	gluDisk(disk, 0.5, 0.8, 16, 4);
	glPopMatrix();
}


void draw_multiview() {//��Ƽ ����Ʈ ����
	glLoadIdentity();
	gluLookAt(0, 3, 6, 0, 3, 0, 0, 1, 0);//ī�޶��� ������ ����

	glViewport(window_width*2 / 3, window_height*3 /4, window_width / 3, window_height /4 );
	draw_interface2();

	glFlush();
}

void get_obj_resource() {
	newspaper.getResource("obj/newspaper.obj");
	newspaper.getTexture1("texture/newspaper.bmp");

	base.getResource("obj/base.obj");
	base.getTexture1("texture/base1.bmp");

	burner.getResource("obj/burner.obj");
	burner.getTexture1("texture/gas_controller.bmp");

	gas_controller.getResource("obj/gas_controller.obj");
	gas_controller.getTexture1("texture/gas_controller.bmp");

	base_cap.getResource("obj/base_cap.obj");
	base_cap.getTexture1("texture/base_cap.bmp");

	lever.getResource("obj/lever.obj");
	lever.getTexture1("texture/lever1.bmp");

	controller.getResource("obj/controller.obj");
	controller.getTexture1("texture/controller.bmp");

	trivet.getResource("obj/trivet.obj");
	trivet.getTexture1("texture/trivet.bmp");

	gas.getResource("obj/gas.obj");
	gas.getTexture1("texture/gas1.bmp");
	gas.getTexture2("texture/gas2.bmp");
	gas.getTexture3("texture/gas3.bmp");

	gas_cap.getResource("obj/gas_cap.obj");
	gas_cap.getTexture1("texture/gas_cap.bmp");

	fire.getResource("obj/fire.obj");
	fire.getTexture1("texture/fire.bmp");
	fire.getTexture2("texture/fire2.bmp");
	fire.getTexture3("texture/fire3.bmp");

	frypan.getResource("obj/frypan.obj");
	frypan.getTexture1("texture/frypan.bmp");

	meat.getResource("obj/meat.obj");
	meat.getTexture1("texture/meat1.bmp");

	dish.getResource("obj/dish.obj");
	dish.getTexture1("texture/dish.bmp");

}

void set_obj_texture() {
	gas.setTextureMapping();
	setDiskTexture();
	base.setTextureMapping();
	base_cap.setTextureMapping();
	gas_controller.setTextureMapping();
	burner.setTextureMapping();
	lever.setTextureMapping();
	controller.setTextureMapping();
	trivet.setTextureMapping();
	fire.setTextureMapping();
	gas_cap.setTextureMapping();
	newspaper.setTextureMapping();
	frypan.setTextureMapping();
	meat.setTextureMapping();
	dish.setTextureMapping();
}
