#ifndef _SEND_DATA
#define _SEND_DATA

#include  <windows.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <iostream>
#include  <string>
#include <atlbase.h>
#include <opencv2\core\core.hpp>

using namespace std;
using namespace cv;

enum ControlFlag{ ABSOLUTE_POSITION = 1, RELATIVE_POSITION = 3 };

struct Speed
{
	int xSpeed; int ySpeed; int zSpeed;

	Speed(int x, int y, int z) : xSpeed(x), ySpeed(y), zSpeed(z){}

};

struct WeldPara
{
	int h0, h1, h2, h3, h4, h5;
	int v0, v1, v2, v3, v4, v5;
	int i1, i2, i3;
};

bool InitCom(HANDLE& m_hCom, OVERLAPPED& wrOverlapped, char* comNum);

bool SendData(HANDLE& m_hCom, OVERLAPPED& wrOverlapped, char* buffer, int bufferSize);

int FormatePointData(Point3i pt, Speed speed, char* buffer, int flag = ControlFlag::RELATIVE_POSITION);

int FormateIniWeldPara(WeldPara wp, char* buffer, int flag);

double MoveTime(Point3i start, Point3i end, Speed sp, double delay = 0);

#endif