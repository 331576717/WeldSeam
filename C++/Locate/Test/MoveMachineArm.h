////////*****************author:wilson*********************************
////////*****************email:wilson_91@foxmail.com*******************
////////*****************date:2014-07-24*******************************
#ifndef _MOVE_MACHINE_ARM
#define _MOVE_MACHINE_ARM
#include <iostream>
#include <cassert>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <vector>

using namespace std;

inline void RotateCenter(cv::Point3i &p,const double theta);

//���ܺ���������Ϣ�����ɻ�е���ƶ��켣����
vector<cv::Point3i> MoveMachineArm(cv::Point3i point, const double theta, const double weld_line_width);


void RotateCenter(cv::Point3i &p, const double theta)
{
	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	int tempX = p.x;
	int tempY = p.y;
	p.x = tempX*cosTheta - tempY*sinTheta + 0.5;
	p.y = tempX*sinTheta + tempY*cosTheta + 0.5;
}

vector<cv::Point3i> MoveMachineArm(cv::Point3i point, const double theta, const double weld_line_width)
{
	//���������ж��Ƿ�������
	//assert((point.x >=0 && point.y >= 0) && (weld_line_width >= 0) && (weld_direction >= 0));
	if(!((point.x >=0 && point.y >= 0) && (weld_line_width >= 0)))	
		exit(0);

	vector<cv::Point3i> weldtrack;
	//����weld_line_width�������㣬����Ҫ��ȡ�ĺ��ӷ�ʽ
	enum weld_way{straight_line,zigzag };
	typedef unsigned weld_way_;
	weld_way_ choise = 0;
	//choise��ֵΪ���ݺ����ȼ��������ֵ

	switch(choise)
	{
		//ֱ���ͣ�ֱ�ӷ������ݣ�����Ҫ�ı�
	case straight_line:
		{
			//SendData(m_hCom, wrOverlapped, buffer);
			break;
		}
		//����Σ�
	case zigzag:
		{
			cv::Point3i up_point;
			cv::Point3i down_point;
			//Speed zigzag_speed(8000, 4000, 17000);
			//����ת
			RotateCenter(point, 0-theta);
			//ģ������ĺ��������
			up_point.x = point.x - weld_line_width / 2;
			up_point.y = point.y - weld_line_width / 2;

			//ģ������ĺ��������
			down_point.x = point.x + weld_line_width / 2;
			down_point.y = point.y + weld_line_width / 2;
			
			RotateCenter(up_point, theta);
			RotateCenter(down_point, theta);
			
			weldtrack.push_back(up_point);
			weldtrack.push_back(down_point);
			//FormateData(up_point, zigzag_speed, g_buffer);
	
			break;
		}
	default:
		break;
	}

	return weldtrack;
}
#endif