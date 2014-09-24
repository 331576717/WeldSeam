#include "MotionPath.h"



vector<Point> GenerateZigzagPath(Point& center, const double theta, const double xStep, const double yStep)
{
	vector<Point> resPoints;

	RotateCenter(center, 0 - theta);

	Point point1(xStep, 0-yStep);
	Point point2(xStep, yStep);
	Point point3(xStep, yStep);
	Point point4(xStep, 0 - yStep);

	RotateCenter(point1, theta);
	RotateCenter(point2, theta);
	RotateCenter(point3, theta);
	RotateCenter(point4, theta);

	resPoints.push_back(point1);
	resPoints.push_back(point2);
	resPoints.push_back(point3);
	resPoints.push_back(point4);
	return resPoints;

}

void RotateCenter(Point &p, const double theta)
{
	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	int tempX = p.x;
	int tempY = p.y;
	p.x = round(tempX*cosTheta - tempY*sinTheta);
	p.y = round(tempX*sinTheta + tempY*cosTheta);
}
