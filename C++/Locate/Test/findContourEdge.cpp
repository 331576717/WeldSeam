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

#include "ProcessImg.h"
#include "SendData.h"

using namespace cv;
using namespace std;

char buffer[16];
HANDLE hTimingSemaphore;
HANDLE hProcSemaphore;
HANDLE m_hCom;
OVERLAPPED wrOverlapped;

Point TestPoint()
{
	Point i(3, 5);
	return i;
}
DWORD WINAPI TimingThread(LPVOID param)
{
	while (true)
	{
		WaitForSingleObject(hTimingSemaphore, INFINITE);
		SendData(m_hCom, wrOverlapped, buffer);
		cout << "Timing" << endl;
		ReleaseSemaphore(hProcSemaphore, 1, NULL);
		
		Sleep(2000);
		
		
	}

}
DWORD WINAPI ImgProcThread(LPVOID param)
{
	while (true)
	{
		WaitForSingleObject(hProcSemaphore, INFINITE);
		buffer[0] = 0x03;
		//X=3000
		buffer[1] = 0x00;
		buffer[2] = 0x00;
		buffer[3] = 0x00 + 1;
		buffer[4] = 0xB8;
		//Y=3000
		buffer[5] = 0x00;
		buffer[6] = 0x00;
		buffer[7] = 0x00 + 1;
		buffer[8] = 0xB8;
		buffer[9] = '\n';
		cout << "Processing" << endl;
		ReleaseSemaphore(hTimingSemaphore, 1, NULL);
		//Sleep(500);
	}
}
int TestHoughLines(Mat img)
{
	Mat dst, color_dst;
	
	Canny(img, dst, 50, 200, 3);
	cvtColor(dst, color_dst, CV_GRAY2BGR);
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 10, 10, 0);
	return 0;

}
int main()
{

	VideoCapture cap(0); // open the default camera
	cap.set(CV_CAP_PROP_FRAME_WIDTH,1024.0);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,768.0);
	Mat frame;
	for (;;)
	{
		cap >> frame; // get a new frame from camera
		//frame = imread("E:\\焊接定位\\pictures\\20140527\\18_11_46.jpg");
		Rect rec(300, 100, 300, 650);
		int p = TestHoughLines(frame(rec));
		//circle(frame, Point(p.x, p.y), 4, Scalar(255, 255, 255), 2);
		imshow("video", frame);
		waitKey();
	}
	cap >> frame; // get a new frame from camera
	bool ini = InitCom(m_hCom, wrOverlapped);
	int X = 50000, Y = 100000;
	char binary[32];
	for (int i = 0; i < 10; i++)
	{
		if (1 == i)
		{
			X = 10000;
			Y = 100000;

		}
		if (2 == i)
		{
			X = 40000;
			Y = 20000;

		}
		buffer[0] = 0x07;

		//X
		buffer[1] = X >> 24;
		buffer[2] = (X >> 16) & 255;
		buffer[3] = (X >> 8) & 255;
		buffer[4] = X & 255;
		//Y=3000
		buffer[5] = Y >> 24;
		buffer[6] = (Y >> 16) & 255;
		buffer[7] = (Y >> 8) & 255;
		buffer[8] = Y & 255;

		buffer[9] = '\n';

		bool error = SendData(m_hCom, wrOverlapped, buffer);
		cout << error << endl;
	}
	
	hTimingSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
	hProcSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	
	CreateThread(NULL, 0, TimingThread, NULL, NULL, NULL);
	CreateThread(NULL, 0, ImgProcThread, NULL, NULL, NULL);

	//cout << "begin" << endl;
	//ReleaseSemaphore(hProcSemaphore);
	Sleep(50000);
	//std::ios::sync_with_stdio(false);
	//IplImage* pFrame = NULL;

	//获取摄像头
	//CvCapture* pCapture = cvCreateCameraCapture(-1);

	
	
	//int intCount  = 0;
	//for(;;)
	//{
		//pFrame=cvQueryFrame( pCapture );
		//if(!pFrame)break;
		//cvShowImage("video",pFrame);
		//Mat frame = imread("E:\\焊接定位\\pictures\\20140609\\18_41_22.jpg");
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

