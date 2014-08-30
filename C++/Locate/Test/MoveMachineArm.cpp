////////*****************author:wilson*********************************
////////*****************email:wilson_91@foxmail.com*******************
////////*****************date:2014-07-24*******************************
#ifndef _MOVE_MACHINE_ARM
#define _MOVE_MACHINE_ARM

#include <iostream>
#include <cassert>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>

//
//接受焊缝坐标信息，生成机械臂移动轨迹坐标
int MoveMachineArm(cv::Point3d point,double weld_line_width,double weld_direction,double offset_direction);
//point:焊缝中心坐标
//weld_line_width:焊缝宽度
//weld_direction：焊缝方向
//offset_direction:模拟声称点便宜方向

//for test use
int main()
{
	//测试数据
	cv::Point3d p1;
	double weld_line_width = 0.1;
	double weld_direction  = 0.1;//以
	double offset_direction = 0.1;

	//
	MoveMachineArm(p1,weld_line_width,weld_direction,offset_direction);

	return 0;
}

/*
*
*
*
*/
int MoveMachineArm(cv::Point3d point,double weld_line_width,double weld_direction,double offset_direction)
{
	//传入数据判断是否有问题
	//assert((point.x >=0 && point.y >= 0) && (weld_line_width >= 0) && (weld_direction >= 0));
	if(!((point.x >=0 && point.y >= 0) && (weld_line_width >= 0) && (weld_direction >= 0)))	
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
			//模拟出来的焊缝上面点
			double up_x = point.x + (weld_line_width / 2 / cv::cos(weld_direction))/(cv::cos(weld_direction + offset_direction) / 
				cv::sin(weld_direction + offset_direction) - cv::sin(weld_direction) / cv::cos(weld_direction));
			double up_y = (up_x - point.x) * cv::cos(weld_direction + offset_direction) / cv::sin(weld_direction + offset_direction) 
				+ point.y;
			//模拟出来的焊缝下面点
			double down_x = point.x - (weld_line_width / 2 / cv::cos(weld_direction))/(cv::cos(weld_direction + offset_direction) / 
				cv::sin(weld_direction + offset_direction) - cv::sin(weld_direction) / cv::cos(weld_direction));
			double down_y = (down_x - point.x) * cv::cos(weld_direction + offset_direction) / cv::sin(weld_direction + offset_direction) 
				+ point.y;

			//formatdata()
			//senddata()
			break;
		}
	default:
		break;
	}

	return 0;
}

#endif