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
#include "TestDemo.h"
#include "MachineArmControl.h"
#include "MovePath.h"

using namespace cv;
using namespace std;

char g_buffer[32];
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
		FormateData();
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
			FormateData(g_vP[g_pointCount], g_vS[g_pointCount], g_buffer);
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
	FormateData(readyPoint, goReaPoiSpeed, g_buffer);
	//FormateData(Point3i(8000, 4000, 0), Speed(3000, 3000, 3000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(MoveTime(originPoint, readyPoint, goReaPoiSpeed));
	//Sleep(10000);


	//焊条下降点（点火点）
	Point3i downPoint(8000, 4000, 17000);
	//前往点火点速度
	Speed goDownPoiSpeed(3000, 3000, 40000);
	FormateData(downPoint, goDownPoiSpeed, g_buffer);
	//FormateData(Point3i(8000, 4000, 17000), Speed(3000, 3000, 40000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(MoveTime(readyPoint, downPoint, goDownPoiSpeed));
	//Sleep(1100);


	//焊条点火以后抬起点
	Point3i upPoint(8000, 4000, 15000);
	//点火后抬起速度
	Speed goUpPoiSpeed(3000, 3000, 20000);
	FormateData(upPoint, goUpPoiSpeed, g_buffer);
	//FormateData(Point3i(8000, 4000, 15000), Speed(3000, 3000, 20000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(MoveTime(downPoint, upPoint, goUpPoiSpeed));
	//Sleep(1100);


	////焊条抬起后压低点（起焊点）
	//Point3i Point(8000, 4000, 15000);
	////前往压低点速度
	//Speed goUpPoiSpeed(3000, 3000, 20000);
	//FormateData(upPoint, goUpPoiSpeed, g_buffer);
	////FormateData(Point3i(8000, 4000, 15000), Speed(3000, 3000, 20000), g_buffer);
	//SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	//Sleep(MoveTime(downPoint, upPoint, goUpPoiSpeed));
	////Sleep(1100);
}

void StartWeldTest()
{
	//Mat img(100, 300, CV_8UC1, Scalar(255));
	//putText(img, "Start", Point(90, 60), 1, 3, Scalar(0));

	//准备
	FormateData(Point3i(106 * 1000 / 3, 52 * 1000 / 3, 10 * 1000 / 3), Speed(30000, 30000, 3000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(2000);
	//下压
	FormateData(Point3i(106 * 1000 / 3, 52 * 1000 / 3, 40 * 1000 / 3), Speed(3000, 3000, 80000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(100);

	//抬起
	FormateData(Point3i(106 * 1000 / 3, 52 * 1000 / 3, 31 * 1000 / 3), Speed(2500, 2500, 20000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(600);
	//压低
	FormateData(Point3i(106 * 1000 / 3, 52 * 1000 / 3, 37 * 1000 / 3), Speed(2500, 2500, 30000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(800);
	//朝目标点移动
	FormateData(Point3i(600 * 1000 / 3, 52 * 1000 / 3, 320 * 1000 / 3), Speed(1850, 0, 950), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));

	//抬起
	FormateData(Point3i(600 * 1000 / 3, 52 * 1000 / 3, 0), Speed(1800, 0, 20000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(10000);
	Mat img(100, 300, CV_8UC1, Scalar(255));
	putText(img, "Start", Point(90, 60), 1, 3, Scalar(0));
	//准备
	FormateData(Point3i(10000, 10000, 10000), Speed(3000, 3000, 3000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(5000);
	//下压
	FormateData(Point3i(30000, 28000, 13500), Speed(3000, 3000, 30000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(800);
	//抬起
	FormateData(Point3i(100000, 28000, 11500), Speed(2500, 2500, 30000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(200);
	//压低
	FormateData(Point3i(100000, 28000, 12000), Speed(2500, 2500, 30000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(800);
	//朝目标点移动
	FormateData(Point3i(180000, 28000, 77000), Speed(2500, 2500, 950), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));


	imshow("wait", img);
	waitKey(30000);
	//抬起

}

void WeldTest()
{
	FormateData(Point3i(1, 1000, 2000), Speed(90000, 20000, 0), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
	Sleep(3000);
	for (int i = 0; i < 15; i++)
	{
		FormateData(Point3i(10000, 0, 0), Speed(25000, 0, 0), g_buffer);
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

int main()
{
	TestMotionPath();
	bool ini = InitCom(g_hCom, g_wrOverlapped);

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
			//imshow("resFrame", resFrame);
			//waitKey();
			Point contourCenter(0, 0);
			double contourWidth = 0.0;
			double contourLength = 0.0;
			double contourTheta = 0.0;
			bool proImg = ProcessImg(resFrame, contourCenter, contourTheta, contourWidth, contourLength);

			Point distancePoint(contourCenter.x, contourCenter.y);
			RotatePoint(distancePoint, 1.0 / 180 * CV_PI);
			long long distanceX = (ConvertPixelToMillimeter(contourCenter.x) - XOFFSET) * 1000 / 3;
			long long distanceY = (ConvertPixelToMillimeter(contourCenter.y) - YOFFSET) * 1000 / 3;
			distanceX = 1.0 * contourCenter.x / 280 * 1000 / 0.3;
			distanceY = 1.0 * contourCenter.y / 280 * 1000 / 0.3;
			circle(resFrame, Point(cvRound(contourCenter.x), cvRound(contourCenter.y)), contourWidth / 2, Scalar(0, 0, 255), 3);
			imshow("video", resFrame);

			int ySp = 200;
			int xSp = 2000;
			FormateData(Point3i(distanceX, distanceY, 0), Speed(xSp, ySp, 0), g_buffer, ControlFlag::RELATIVE_POSITION);
			SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));


			//Sleep(1000);

			int step = 5000;
			distanceX = step * cos(contourTheta);
			distanceY = step * sin(contourTheta);

			vector<cv::Point3i> zigzag_point;
			zigzag_point = MoveMachineArm(Point3i(distanceX, distanceY, 0), contourTheta, contourWidth);
			//FormateData(Point3i(distanceX, distanceY, 0), Speed(5000, 5000, 0), g_buffer, ControlFlag::RELATIVE_POSITION);
			for (int i = 0; i < zigzag_point.size(); i++)
			{
				FormateData(zigzag_point[i], Speed(5000, 5000, 0), g_buffer, ControlFlag::RELATIVE_POSITION);
				SendData(g_hCom, g_wrOverlapped, g_buffer, sizeof(g_buffer));
				Sleep(1000 / zigzag_point.size());
			}

			Sleep(1000);

			char c = waitKey(1000);
			if ('s' == c){
				SaveImg(resFrame, count);
				count++;
			}
			if ('a' == c){
				FormateData(Point3i(0, 0, 0), Speed(4000, 0, 0), g_buffer, ControlFlag::ABSOLUTE_POSITION);
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

	StartWeldTest();

	return 0;
}