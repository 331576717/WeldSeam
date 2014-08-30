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
//���ܺ���������Ϣ�����ɻ�е���ƶ��켣����
int MoveMachineArm(cv::Point3d point,double weld_line_width,double weld_direction,double offset_direction);
//point:������������
//weld_line_width:������
//weld_direction�����췽��
//offset_direction:ģ�����Ƶ���˷���

//for test use
int main()
{
	//��������
	cv::Point3d p1;
	double weld_line_width = 0.1;
	double weld_direction  = 0.1;//��
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
	//���������ж��Ƿ�������
	//assert((point.x >=0 && point.y >= 0) && (weld_line_width >= 0) && (weld_direction >= 0));
	if(!((point.x >=0 && point.y >= 0) && (weld_line_width >= 0) && (weld_direction >= 0)))	
		return 1;

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
			//ģ������ĺ��������
			double up_x = point.x + (weld_line_width / 2 / cv::cos(weld_direction))/(cv::cos(weld_direction + offset_direction) / 
				cv::sin(weld_direction + offset_direction) - cv::sin(weld_direction) / cv::cos(weld_direction));
			double up_y = (up_x - point.x) * cv::cos(weld_direction + offset_direction) / cv::sin(weld_direction + offset_direction) 
				+ point.y;
			//ģ������ĺ��������
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