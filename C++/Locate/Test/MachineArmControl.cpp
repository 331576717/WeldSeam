#include "MachineArmControl.h"

void MoveMachineArm(cv::Point center, const double theta, const double weld_line_width, int MotionFlag, char* buffer)
{
	//���������ж��Ƿ�������
	//assert((point.x >=0 && point.y >= 0) && (weld_line_width >= 0) && (weld_direction >= 0));
	if (!((center.x >= 0 && center.y >= 0) && (weld_line_width >= 0)))
		exit(0);

	vector<cv::Point3i> weldtrack;
	//����weld_line_width�������㣬����Ҫ��ȡ�ĺ��ӷ�ʽ
	
	//choise��ֵΪ���ݺ����ȼ��������ֵ

	switch (MotionFlag)
	{
			//ֱ���ͣ�ֱ�ӷ������ݣ�����Ҫ�ı�
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