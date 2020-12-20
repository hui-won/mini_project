#pragma once
#define PI 3.141592

double radius = 20;
double theta = 60, phi = 90;
double cam[3];
double center[3] = { 0,0,0 };
double up[3] = { 0,1,0 };

float x_axis = 0, y_axis = 0;

void calculate_axis() {

	if (theta > 180 && theta <= 360) {
		up[1] = -1;
	}
	else {
		up[1] = 1;
	}

	double rad_theta = theta * PI / 180;
	double rad_phi = phi * PI / 180;


	cam[0] = radius * sin(rad_theta) * cos(rad_phi);
	cam[1] = radius * cos(rad_theta);
	cam[2] = radius * sin(rad_theta) * sin(rad_phi);
}
void init_view_dir() {//인터페이스 1에서의 카메라 초기 시점 지정
	radius = 20;
	theta = 60, phi = 90;
	x_axis = 0, y_axis = 0;
	
}
void init_view_int2() {//인터페이스 2에서의 카메라 초기 시점
	radius = 25;
	theta = 60, phi = 90;
	x_axis = 0, y_axis = 0;
}
void aerial_view() {//항공뷰
	radius = 20;
	theta = 360, phi = 90;
}