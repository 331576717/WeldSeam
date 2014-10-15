#include <iostream>
#include <vector>
#include <deque>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <direct.h>
#include <time.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <Windows.h>
#include <fstream>


#include "ProcessImg.h"
#include "SendData.h"
#include "MachineArmControl.h"
#include "TestDemo.h"


using namespace cv;
using namespace std;

char g_buffer[128];
HANDLE g_hTimingSemaphore;
HANDLE g_hProcSemaphore;
HANDLE g_hCom;
OVERLAPPED g_wrOverlapped;

CONST int XLOCATIONBOUND = 160001;
CONST int ZLOCATIONBOUND = 57000;
CONST double XOFFSET = -126.85;
CONST double YOFFSET = 8.59;

vector<Point3i> g_vP;
vector<Speed> g_vS;
int g_pointCount = 0;

DWORD WINAPI TimingThread(LPVOID param)
{
	while (true)
	{
		WaitForSingleObject(g_hTimingSemaphore, INFINITE);
		SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
		cout << "Timing" << endl;
		ReleaseSemaphore(g_hProcSemaphore, 1, NULL);

		Sleep(2000);
		/*
		for(int i = 0, i < pointvector.count; i++)
		{
		FormatePointData();
		SendData();
		sleep(2000 / pointvector.count);
		}

		*/

		//Sleep(2000);

	}

}

DWORD WINAPI ImgProcThread(LPVOID param)
{
	while (true)
	{
		WaitForSingleObject(g_hProcSemaphore, INFINITE);
		if (g_pointCount < 14)
		{
			FormatPointData(g_vP[g_pointCount], g_vS[g_pointCount], g_buffer);
			g_pointCount++;
			cout << "Processing" << "Y: " << g_vP[g_pointCount].y << endl;
			ReleaseSemaphore(g_hTimingSemaphore, 1, NULL);
		}
		//Sleep(500);
	}
}

void StartWeld()
{
	//原点
	Point3i originPoint(0, 0, 0);
	//焊条起焊准备点
	Point3i readyPoint(8000, 4000, 0);
	//前往焊条起焊准备点的速度
	Speed goReaPoiSpeed(3000, 3000, 3000);
	FormatPointData(readyPoint, goReaPoiSpeed, g_buffer);
	//FormatPointData(Point3i(8000, 4000, 0), Speed(3000, 3000, 3000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(MoveTime(originPoint, readyPoint, goReaPoiSpeed));
	//Sleep(10000);


	//焊条下降点（点火点）
	Point3i downPoint(8000, 4000, 17000);
	//前往点火点速度
	Speed goDownPoiSpeed(3000, 3000, 40000);
	FormatPointData(downPoint, goDownPoiSpeed, g_buffer);
	//FormatPointData(Point3i(8000, 4000, 17000), Speed(3000, 3000, 40000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(MoveTime(readyPoint, downPoint, goDownPoiSpeed));
	//Sleep(1100);


	//焊条点火以后抬起点
	Point3i upPoint(8000, 4000, 15000);
	//点火后抬起速度
	Speed goUpPoiSpeed(3000, 3000, 20000);
	FormatPointData(upPoint, goUpPoiSpeed, g_buffer);
	//FormatPointData(Point3i(8000, 4000, 15000), Speed(3000, 3000, 20000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(MoveTime(downPoint, upPoint, goUpPoiSpeed));
	//Sleep(1100);


	////焊条抬起后压低点（起焊点）
	//Point3i Point(8000, 4000, 15000);
	////前往压低点速度
	//Speed goUpPoiSpeed(3000, 3000, 20000);
	//FormatPointData(upPoint, goUpPoiSpeed, g_buffer);
	////FormatPointData(Point3i(8000, 4000, 15000), Speed(3000, 3000, 20000), g_buffer);
	//SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	//Sleep(MoveTime(downPoint, upPoint, goUpPoiSpeed));
	////Sleep(1100);
}

void StartWeldTest()
{
	//Mat img(100, 300, CV_8UC1, Scalar(255));
	//putText(img, "Start", Point(90, 60), 1, 3, Scalar(0));

	//准备
	FormatPointData(Point3i(267 * 1000 / 3, 12 * 1000 / 3, 90 * 1000 / 3), Speed(25000, 25000, 25000), g_buffer, ControlFlag::ABSOLUTE_POSITION);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(1000);
	//下压
	FormatPointData(Point3i(261 * 1000 / 3, 12 * 1000 / 3, 99 * 1000 / 3), Speed(13000, 11000, 40000), g_buffer, ControlFlag::ABSOLUTE_POSITION);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(200);
	//抬起
	FormatPointData(Point3i(263 * 1000 / 3, 12 * 1000 / 3, 94 * 1000 / 3), Speed(2500, 2500, 15000), g_buffer, ControlFlag::ABSOLUTE_POSITION);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(1000);
	//压低1
	FormatPointData(Point3i(270 * 1000 / 3, 12 * 1000 / 3, 95 * 1000 / 3), Speed(1000, 2500, 5000), g_buffer, ControlFlag::ABSOLUTE_POSITION);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(1500);
	//压低2
	FormatPointData(Point3i(270 * 1000 / 3, 12 * 1000 / 3, 95 * 1000 / 3), Speed(1000, 2500, 5000), g_buffer, ControlFlag::ABSOLUTE_POSITION);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(300);
	//前进1000
	FormatPointData(Point3i(600 * 1000 / 3, 12 * 1000 / 3, 150 * 1000 / 3), Speed(3 * 1000 / 3, 2500, 1250), g_buffer, ControlFlag::ABSOLUTE_POSITION);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(1000);
	FormatPointData(Point3i(600 * 1000 / 3, 13 * 1000 / 3, 0.0 * 1000 / 3), Speed(2500, 2500, 20000), g_buffer, ControlFlag::ABSOLUTE_POSITION);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(1000);
for (size_t i = 0; i < 15; i++)
	{
		Point tmp(10, 10);

		vector<Point> pathPoints = GenerateZigzagPath(tmp, 0, 2, 1);
		Point3i nextPoint;
		Speed sp(0, 0, 0);
		sp.xSpeed = 4 * 1000 / 3;
		sp.ySpeed = 2 * 1000 / 3;
		sp.zSpeed = 1250;
		for (size_t j = 0; j < pathPoints.size(); j++)
		{
			double z = 150.0 * 1000 / 3;
			pathPoints[j].x = pathPoints[j].x * 1000 / 3;
			pathPoints[j].y = pathPoints[j].y * 1000 / 3;
			nextPoint = Point3i(pathPoints[j].x, pathPoints[j].y, z);
			FormatPointData(nextPoint, sp, g_buffer, ControlFlag::RELATIVE_POSITION);
			bool send = SendData(g_hCom, g_wrOverlapped, g_buffer, 32);
			Sleep(2000 / pathPoints.size());
		}
	}

	////朝目标点移动
	//FormatPointData(Point3i(600 * 1000 / 3, 52 * 1000 / 3, 320 * 1000 / 3), Speed(1850, 0, 950), g_buffer);
	//SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));

	//imshow("wait", img);
	//waitKey(30000);
}

void WeldTest()
{
	FormatPointData(Point3i(1, 1000, 2000), Speed(90000, 20000, 0), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(3000);
	for (int i = 0; i < 15; i++)
	{
		FormatPointData(Point3i(10000, 0, 0), Speed(25000, 0, 0), g_buffer);
		SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
		Sleep(1000);
	}

}

double ComputeDistance()
{
	int x1, y1, x2, y2;
	cin >> x1 >> y1 >> x2 >> y2;
	return sqrt(1.0*(x1 - x2)*(x1 - x2) + 1.0*(y1 - y2)*(y1 - y2));

}
double ConvertPixelToMillimeter(int pixel)
{
	const double pixel2millimeter = 39.01;

	double millimeter = pixel / pixel2millimeter;

	return millimeter;
}

void testMove()
{
	FormatPointData(Point3i(10000, 10000, 10000), Speed(50000, 50000, 10000), g_buffer, ControlFlag::ABSOLUTE_POSITION);
	SendData(g_hCom, g_wrOverlapped, g_buffer, 32);
	Sleep(2000);
	for (size_t i = 1; i < 15; i++)
	{
		FormatPointData(Point3i(10000+3000*i,10000,10000), Speed(2000,10000,10000), g_buffer, ControlFlag::ABSOLUTE_POSITION);
		SendData(g_hCom, g_wrOverlapped, g_buffer, 32);
		Sleep(1000);
	}
}

int main()
{
	
	int a = 4;
	FormateInt32(a, g_buffer);
	unsigned char c[10];
	c[0] = 177;
	int ui = c[0];
	cout << ui;
	testImg();
	//FormatPointData(Point3i(10000,10000,10000), Speed(10000,10000,10000), g_buffer, ControlFlag::ABSOLUTE_POSITION);
	//SendData(g_hCom, g_wrOverlapped, g_buffer, 32);
	
	//bool ini = InitCom(g_hCom, g_wrOverlapped, "COM6");
	//StartWeldTest();

	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())
	{
		std::cout << "Camera is not opened!" << endl;
		return 1;
	}
	//cap.set(CV_CAP_PROP_FRAME_WIDTH,1024.0);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT,768.0);
	Mat frame(768, 1024, CV_32FC3, Scalar::all(255.0));

	vector<Mat> frames;
	int count = 0;
	for (;;){
		bool readSucceed = cap.read(frame); // get a new frame from camera
		if (readSucceed){
			frames.push_back(frame);
		}
		else{
			std::cout << "Camera read failed !" << endl;
			continue;
		}

		int frameNumber = 1;
		if (frameNumber == frames.size())
		{
			Mat resFrame = frames[0] / frameNumber;
			for (int i = 1; i < frameNumber; i++)
			{
				resFrame = (resFrame + frames[i] / frameNumber);
			}
			imshow("resFrame", resFrame);
			waitKey();
			Point contourCenter(0, 0);
			double contourWidth = 0.0;
			double contourLength = 0.0;
			double contourTheta = 0.0;
			bool proImg = ProcessImg(resFrame, contourCenter, contourTheta, contourWidth, contourLength);
			circle(resFrame, contourCenter, contourWidth/2, Scalar(255, 0, 0), 1);
			imshow("res", resFrame);
			waitKey();
			Sleep(1000);

			char c = waitKey(1000);
			if ('s' == c){
				SaveImg(resFrame, count);
				count++;
			}
			if ('a' == c){
				FormatPointData(Point3i(0, 0, 0), Speed(4000, 0, 0), g_buffer, ControlFlag::ABSOLUTE_POSITION);
				SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
				break;
			}
			frames.clear();
		}
	}

	//bool ini = InitCom(g_hCom, g_wrOverlapped);

	g_hTimingSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	g_hProcSemaphore = CreateSemaphore(NULL, 0, 1, NULL);

	CreateThread(NULL, 0, TimingThread, NULL, NULL, NULL);
	CreateThread(NULL, 0, ImgProcThread, NULL, NULL, NULL);

	

	return 0;
}