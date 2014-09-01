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

bool InitCom(HANDLE& m_hCom, OVERLAPPED& wrOverlapped);

bool SendData(HANDLE& m_hCom, OVERLAPPED& wrOverlapped, char* buffer, int bufferSize);

void FormateData(Point3i pt, Speed speed, char* buffer, int flag = ControlFlag::RELATIVE_POSITION);

double MoveTime(Point3i start, Point3i end, Speed sp, double delay = 0);

#endif