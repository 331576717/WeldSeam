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

using namespace cv;
using namespace std;

char g_buffer[32];
HANDLE g_hTimingSemaphore;
HANDLE g_hProcSemaphore;
HANDLE g_hCom;
OVERLAPPED g_wrOverlapped;

vector<Point3i> g_vP;
vector<Speed> g_vS;
int g_pointCount = 0;

DWORD WINAPI TimingThread(LPVOID param)
{
	while (true)
	{
		WaitForSingleObject(g_hTimingSemaphore, INFINITE);
		SendData(g_hCom, g_wrOverlapped, g_buffer);
		cout << "Timing" << endl;
		ReleaseSemaphore(g_hProcSemaphore, 1, NULL);
		
		Sleep(10000);
		
	}

}
DWORD WINAPI ImgProcThread(LPVOID param)
{
	while (true)
	{
		WaitForSingleObject(g_hProcSemaphore, INFINITE);
		FormateData(g_vP[g_pointCount], g_vS[g_pointCount], g_buffer);
		g_pointCount++;
		cout << "Processing" << "Y: " << g_vP[g_pointCount].y << endl;
		ReleaseSemaphore(g_hTimingSemaphore, 1, NULL);
		//Sleep(500);
	}
}
void StartWeld()
{
	FormateData(Point3i(8000, 4000, 0), Speed(3000, 3000, 3000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer);
	Sleep(10000);
	FormateData(Point3i(8000, 4000, 17000), Speed(3000, 3000, 40000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer);
	Sleep(1100);
	FormateData(Point3i(8000, 4000, 15000), Speed(3000, 3000, 20000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer);
	Sleep(1100);
	FormateData(g_vP[g_pointCount], g_vS[g_pointCount], g_buffer);
	g_pointCount++;
	Sleep(2000);
}
void StartWeldTest()
{
	FormateData(Point3i(8000, 400, 0), Speed(3000, 3000, 3000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer);
	Sleep(10000);
	FormateData(Point3i(8000, 400, 30000), Speed(3000, 3000, 40000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer);
 	Sleep(1100);
	FormateData(Point3i(8000, 400, 28000), Speed(3000, 3000, 20000), g_buffer);
	SendData(g_hCom, g_wrOverlapped, g_buffer);
	Sleep(1100);
	FormateData(g_vP[g_pointCount], g_vS[g_pointCount], g_buffer);
	g_pointCount++;
	Sleep(1100);
}

int main()
{
	ifstream fin;
	fin.open("C:\\Users\\James\\Desktop\\res2.txt");
	for (size_t i = 0; i < 15; i++)
	{
		int x, y, z, xSpped, ySpeed, zSpeed;
		fin >> x >> y >> z;
		x = 8000 + 10000*i;
		y = 400;
		z = 29000 + (i+1) * 10000;
		g_vP.push_back(Point3i(x, y, z));

		fin >> xSpped >> ySpeed >> zSpeed;
		zSpeed = 1000;
		g_vS.push_back(Speed(xSpped, ySpeed, zSpeed));
	}
	//VideoCapture cap(0); // open the default camera
	//cap.set(CV_CAP_PROP_FRAME_WIDTH,1024.0);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT,768.0);
	//Mat frame;
	//for (;;)
	//{
	//	cap >> frame; // get a new frame from camera
	//	//frame = imread("E:\\���Ӷ�λ\\pictures\\20140527\\18_11_46.jpg");
	//	Rect rec(300, 100, 300, 650);
	//	//int p = TestHoughLines(frame(rec));
	//	//circle(frame, Point(p.x, p.y), 4, Scalar(255, 255, 255), 2);
	//	imshow("video", frame);
	//	waitKey(50);
	//}
	//cap >> frame; // get a new frame from camera
	bool ini = InitCom(g_hCom, g_wrOverlapped);

	g_hTimingSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	g_hProcSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	
	CreateThread(NULL, 0, TimingThread, NULL, NULL, NULL);
	CreateThread(NULL, 0, ImgProcThread, NULL, NULL, NULL);

	StartWeldTest();

	/*FormateData(Point3i(10000, 20000, 3000), Speed(1000, 1000, 1000), buffer);
	SendData(m_hCom, wrOverlapped, buffer);
	int X = vP[0].x, Y = vP[0].y, Z = 14999;
	buffer[1] = X >> 24;
	buffer[2] = (X >> 16) & 255;
	buffer[3] = (X >> 8) & 255;
	buffer[4] = X & 255;
	//Y
	buffer[5] = Y >> 24;
	buffer[6] = (Y >> 16) & 255;
	buffer[7] = (Y >> 8) & 255;
	buffer[8] = Y & 255;
	//Z
	buffer[9] = Z >> 24;
	buffer[10] = (Z >> 16) & 255;
	buffer[11] = (Z >> 8) & 255;
	buffer[12] = Z & 255;
	buffer[13] = '\n';
	bool error = SendData(m_hCom, wrOverlapped, buffer);
	Sleep(2000);
	for (size_t i = 1; i < vP.size(); i++)
	{
		 X = vP[i].x, Y = vP[i].y, Z = 14999;
		//X
		buffer[1] = X >> 24;
		buffer[2] = (X >> 16) & 255;
		buffer[3] = (X >> 8) & 255;
		buffer[4] = X & 255;
		//Y
		buffer[5] = Y >> 24;
		buffer[6] = (Y >> 16) & 255;
		buffer[7] = (Y >> 8) & 255;
		buffer[8] = Y & 255;
		//Z
		buffer[9] = Z >> 24;
		buffer[10] = (Z >> 16) & 255;
		buffer[11] = (Z >> 8) & 255;
		buffer[12] = Z & 255;

		buffer[13] = '\n';

		cout << "X Y Z: " << X << " " << Y << " " << Z << endl;
		bool error = SendData(m_hCom, wrOverlapped, buffer);
		Sleep(9500);
		

	}
	char binary[32];
	//long long int X = 0, Y = 0, Z = 14999;
	for (int i = 12; i < 15; i++)
	{
		X = 10000 + i * 10000;
		for (int j = 0; j < 8; j++)
		{
			Y = 2200 - j * 200;

			X = X % 160000;
			Y = Y % 100000;
			Z = Z % 15000;
			buffer[0] = 0x07;

			//X
			buffer[1] = X >> 24;
			buffer[2] = (X >> 16) & 255;
			buffer[3] = (X >> 8) & 255;
			buffer[4] = X & 255;
			//Y
			buffer[5] = Y >> 24;
			buffer[6] = (Y >> 16) & 255;
			buffer[7] = (Y >> 8) & 255;
			buffer[8] = Y & 255;
			//Z
			buffer[9] = Z >> 24;
			buffer[10] = (Z >> 16) & 255;
			buffer[11] = (Z >> 8) & 255;
			buffer[12] = Z & 255;
			buffer[13] = '\n';

			bool error = SendData(m_hCom, wrOverlapped, buffer);
			cout << "Now we are at " << X << " " << Y << " " << Z << "   Save it ?" << endl;
			char c;
			cin >> c;
			if ('Y' == c || 'y' == c)
			{
				vP.push_back(Point(X, Y));
				break;
			}
		}
		//cout << error << endl;
	}
	ofstream fout;
	fout.open("C:\\Users\\James\\Desktop\\res.txt");
	for (size_t i = 0; i < vP.size(); i++)
	{
		fout << vP[i].x << " " << vP[i].y << endl;
	}
	fout.close();*/
	
	cout << "begin" << endl;
	ReleaseSemaphore(g_hProcSemaphore, 1, NULL);
	Sleep(500000);
	//std::ios::sync_with_stdio(false);
	//IplImage* pFrame = NULL;

	//��ȡ����ͷ
	//CvCapture* pCapture = cvCreateCameraCapture(-1);

	
	
	//int intCount  = 0;
	//for(;;)
	//{
		//pFrame=cvQueryFrame( pCapture );
		//if(!pFrame)break;
		//cvShowImage("video",pFrame);
		//Mat frame = imread("E:\\���Ӷ�λ\\pictures\\20140609\\18_41_22.jpg");
		//cap >> frame; // get a new frame from camera
		//cvtColor(frame, frame, CV_BGR2GRAY);
		//GaussianBlur(frame, frame, Size(7,7), 1.5, 1.5);
		//Canny(frame, frame, 0, 30, 3);
		
		//ProcessImg(frame);
		//imshow("img",frame);
		//char c=cvWaitKey(15);
		
		/*if(c=='s')
		{
			SaveImg(frame,intCount);
			intCount++;
		}
	}*/
	
	return 0;
}

