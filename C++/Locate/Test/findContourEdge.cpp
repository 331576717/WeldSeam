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

HANDLE hTimingMutex;
HANDLE hProcMutex;
char buffer[16];
DWORD WINAPI TimingThread(LPVOID param)
{
	while (true)
	{
		WaitForSingleObject(hTimingMutex, INFINITE);
		//SendData(buffer);
		cout << "Timing" << endl;
		ReleaseSemaphore(hProcMutex, 1, NULL);
		
		Sleep(500);
		
		
	}

}
DWORD WINAPI ImgProcThread(LPVOID param)
{
	while (true)
	{
		WaitForSingleObject(hProcMutex, INFINITE);
		cout << "Processing" << endl;
		ReleaseSemaphore(hTimingMutex, 1, NULL);
		Sleep(500);
	}
}
int main()
{
	HANDLE m_hCom;
	hTimingMutex = CreateSemaphore(NULL, 1, 1, NULL);
	hProcMutex = CreateSemaphore(NULL, 0, 1, NULL);
	
	CreateThread(NULL, 0, TimingThread, NULL, NULL, NULL);
	CreateThread(NULL, 0, ImgProcThread, NULL, NULL, NULL);

	//cout << "begin" << endl;
	//ReleaseMutex(hProcMutex);
	Sleep(50000);
	//std::ios::sync_with_stdio(false);
	//IplImage* pFrame = NULL;

	//获取摄像头
	//CvCapture* pCapture = cvCreateCameraCapture(-1);

	//VideoCapture cap(0); // open the default camera
	//cap.set(CV_CAP_PROP_FRAME_WIDTH,1024.0);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT,768.0);
	
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

