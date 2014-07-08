#include  <windows.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <iostream>
#include  <string>
#include <atlbase.h>
using namespace std;

struct Speed
{ 
	int xSpeed; int ySpeed; int zSpeed; 

	Speed(int x, int y, int z) : xSpeed(x), ySpeed(y), zSpeed(z){} 

};

bool InitCom(HANDLE& m_hCom, OVERLAPPED& wrOverlapped);
bool SendData(HANDLE& m_hCom, OVERLAPPED& wrOverlapped, char* buffer);
void FormateData(Point3i pt, Speed speed, char* buffer);

bool InitCom(HANDLE& m_hCom, OVERLAPPED& wrOverlapped)
{
	//第一步，打开串口
	//HANDLE m_hCom;
	m_hCom = CreateFile((WCHAR*)_T("com5"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		std::cout << "CreateFile fail!" << endl;
		return -1;
	}
	//cout << "CreateFile OK!" << endl;

	//第二步，设置缓冲区大小
	if (!SetupComm(m_hCom, 2048, 2048))
	{
		cout << "SetupComm fail! Close Comm!" << endl;
		CloseHandle(m_hCom);
		return -1;
	}
	//cout << "SetupComm OK!" << endl;

	//第三步，设置超时
	COMMTIMEOUTS TimeOuts;
	memset(&TimeOuts, 0, sizeof(TimeOuts));
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	TimeOuts.WriteTotalTimeoutMultiplier = 50;
	SetCommTimeouts(m_hCom, &TimeOuts);

	//第四步，设置串口参数
	DCB dcb;
	if (!GetCommState(m_hCom, &dcb))
	{
		cout << "GetCommState fail! Comm close" << endl;
		CloseHandle(m_hCom);
		return -1;
	}
	//cout << "GetCommState OK!" << endl;

	dcb.DCBlength = sizeof(dcb);
	if (!BuildCommDCB((WCHAR*)_T("9600,n,8,1"), &dcb))//填充DCB的数据传输率、奇偶校验类型、数据位、停止位
	{
		//参数修改错误，进行错误处理
		cout << "BuileCOmmDCB fail,Comm close!" << endl;
		CloseHandle(m_hCom);
		return -1;
	}
	if (SetCommState(m_hCom, &dcb))
	{
		//cout << "SetCommState OK!" << endl;
	}

	//第五步，建立并初始化重叠结构
	ZeroMemory(&wrOverlapped, sizeof(wrOverlapped));
	if (wrOverlapped.hEvent != NULL)
	{
		ResetEvent(wrOverlapped.hEvent);
		wrOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	
}
bool SendData(HANDLE& m_hCom, OVERLAPPED& wrOverlapped, char* buffer)
{
	
	//第七步，发送数据
	//y = 80000(16# 00 01 38 80),VB2004~VB2007,VD2004;
	// send data 30000/80000
	// 10#30000~16# 00 00 75 30
	/*char buffer[16];
	buffer[0] = 0x00;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x64;*/
	//cout << "subFunc:" << buffer[0] << endl;
	
	DWORD dwerrorflag;
	COMSTAT comstat;
	DWORD writenumber = 1024;
	DWORD dwhavewrite;
	bool error = WriteFile(m_hCom, buffer, writenumber, &dwhavewrite, &wrOverlapped);
	return error;
	//// 10#80000~16# 00 01 38 80
	//buffer[4] = 0x00;
	//buffer[5] = 0x01;
	//buffer[6] = 0x38;
	//buffer[7] = 0x80;

	/* 80000/30000
	buffer[8] = 0x00;
	buffer[9] = 0x01;
	buffer[10] = 0x38;
	buffer[11] = 0x80;
	buffer[12] = 0x00;
	buffer[13] = 0x00;
	buffer[14] = 0x75;
	buffer[15] = 0x30;*/

}
	
void FormateData(Point3i pt, Speed speed, char* buffer)
{
	buffer[0] = 0x07;
	//X
	buffer[1] = pt.x >> 24;
	buffer[2] = (pt.x >> 16) & 255;
	buffer[3] = (pt.x >> 8) & 255;
	buffer[4] = pt.x & 255;
	//Y
	buffer[5] = pt.y >> 24;
	buffer[6] = (pt.y >> 16) & 255;
	buffer[7] = (pt.y >> 8) & 255;
	buffer[8] = pt.y & 255;
	//Z
	buffer[9] = pt.z >> 24;
	buffer[10] = (pt.z >> 16) & 255;
	buffer[11] = (pt.z >> 8) & 255;
	buffer[12] = pt.z & 255;

	//xSPEED
	buffer[13] = speed.xSpeed >> 24;
	buffer[14] = (speed.xSpeed >> 16) & 255;
	buffer[15] = (speed.xSpeed >> 8) & 255;
	buffer[16] = speed.xSpeed & 255;

	//ySPEED
	buffer[17] = speed.ySpeed >> 24;
	buffer[18] = (speed.ySpeed >> 16) & 255;
	buffer[19] = (speed.ySpeed >> 8) & 255;
	buffer[20] = speed.ySpeed & 255;

	//zSPEED
	buffer[21] = speed.zSpeed >> 24;
	buffer[22] = (speed.zSpeed >> 16) & 255;
	buffer[23] = (speed.zSpeed >> 8) & 255;
	buffer[24] = speed.zSpeed & 255;

	buffer[25] = '\n';

}

