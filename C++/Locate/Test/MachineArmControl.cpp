#include "MachineArmControl.h"

void MoveMachineArm(cv::Point center, const double theta, const double weld_line_width, int MotionFlag, char* buffer)
{
	//传入数据判断是否有问题
	//assert((point.x >=0 && point.y >= 0) && (weld_line_width >= 0) && (weld_direction >= 0));
	if (!((center.x >= 0 && center.y >= 0) && (weld_line_width >= 0)))
		exit(0);

	vector<cv::Point3i> weldtrack;
	//根据weld_line_width进行运算，决定要采取的焊接方式
	
	//choise赋值为根据焊缝宽度计算出来的值

	switch (MotionFlag)
	{
			//直线型，直接发送数据，不需要改变
	case MotionFlag::ZIGZAG:
		{
			vector<Point> pathPoints = GenerateZigzagPath(center, 0, 5, 5);
			Point3i nextPoint;
			Speed sp(0,0,0);
			sp.xSpeed = 5 * 1000 / 3;
			sp.ySpeed = 30 * 1000 / 3;
			sp.zSpeed = 0;
			for (size_t i = 0; i < pathPoints.size(); i++)
			{
				double z = 0.0;
				pathPoints[i].x = pathPoints[i].x * 1000 / 3;
				pathPoints[i].x = pathPoints[i].y * 1000 / 3;
				nextPoint = Point3i(pathPoints[i].x, pathPoints[i].y, z);
				FormateData(nextPoint, sp, buffer, ControlFlag::RELATIVE_POSITION);
				Sleep(2000 / pathPoints.size());
			}
			break;
		}
		default:
			break;
		}

}