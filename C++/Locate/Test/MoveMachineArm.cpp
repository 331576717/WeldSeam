////////*****************author:wilson*********************************
////////*****************email:wilson_91@foxmail.com*******************
////////*****************date:2014-07-24*******************************
#ifndef _MOVE_MACHINE_ARM
#define _MOVE_MACHINE_ARM

#include <iostream>
#include <cassert>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include "SendData.h"


void RotatePoint(cv::Point &p, double theta);
//
//接受焊缝坐标信息，生成机械臂移动轨迹坐标
int MoveMachineArm(cv::Point point,double weld_line_width,double weld_direction,double offset_direction);
//point:焊缝中心坐标
//weld_line_width:焊缝宽度
//weld_direction：焊缝方向
//offset_direction:模拟声称点便宜方向

extern char g_buffer[32];

//for test use
//int main()
//{
//	//测试数据
//	cv::Point3d p1;
//	double weld_line_width = 0.1;
//	double weld_direction  = 0.1;//以
//	double offset_direction = 0.1;
//
//	//
//	MoveMachineArm(p1,weld_line_width,weld_direction,offset_direction);
//
//	return 0;
//}

void RotatePoint(cv::Point3i &p, const double theta)
{
	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	int tempX = p.x;
	int tempY = p.y;
	p.x = tempX*cosTheta - tempY*sinTheta + 0.5;
	p.y = tempX*sinTheta + tempY*cosTheta + 0.5;
}

/*
*
*
*
*/
//int MoveMachineArm(cv::Point3d point,double weld_line_width,double weld_direction,double offset_direction)
int MoveMachineArm(cv::Point3i point, const double theta, const double weld_line_width)
{
	//传入数据判断是否有问题
	//assert((point.x >=0 && point.y >= 0) && (weld_line_width >= 0) && (weld_direction >= 0));
	if(!((point.x >=0 && point.y >= 0) && (weld_line_width >= 0)))	
		return 1;

	//根据weld_line_width进行运算，决定要采取的焊接方式
	enum weld_way{straight_line,zigzag };
	typedef unsigned weld_way_;
	weld_way_ choise = 0;
	//choise赋值为根据焊缝宽度计算出来的值

	switch(choise)
	{
		//直线型，直接发送数据，不需要改变
	case straight_line:
		{
			//SendData(m_hCom, wrOverlapped, buffer);
			break;
		}
		//锯齿形，
	case zigzag:
		{
			cv::Point3i up_point;
			cv::Point3i down_point;
			Speed zigzag_speed(8000, 4000, 17000);
			//点旋转
			RotatePoint(point,theta);
			//模拟出来的焊缝上面点
			up_point.x = point.x - weld_line_width / 2;
			up_point.y = point.y - weld_line_width / 2;

			//模拟出来的焊缝下面点
			down_point.x = point.x + weld_line_width / 2;
			down_point.y = point.y + weld_line_width / 2;
			
			RotatePoint(up_point,-theta);
			RotatePoint(down_point,-theta);
			
			FormateData(up_point, zigzag_speed, g_buffer);
	
			break;
		}
	default:
		break;
	}

	return 0;
}

#endif