#ifndef _MOTION_PAHT
#define _MOTION_PATH
#include <opencv2\core\core.hpp>
#include <vector>

using namespace cv;
using namespace std;

enum MotionFlag{ ZIGZAG = 1 };

vector<Point> GenerateZigzagPath(Point& center, const double theta, const double offset, const double step);

void RotateCenter(Point &p, const double theta);

vector<Point> GenerateZigzagPath(Point& center, const double theta, const double xStep, const double yStep)
{
	vector<Point> resPoints;

	RotateCenter(center, 0 - theta);

	Point upPoint(center.x + xStep, center.y - yStep);
	Point downPoint(upPoint.x + xStep, center.y + yStep);

	RotateCenter(upPoint, theta);
	RotateCenter(downPoint, theta);

	resPoints.push_back(upPoint);
	resPoints.push_back(downPoint);
	return resPoints;

}

void RotateCenter(Point &p, const double theta)
{
	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	int tempX = p.x;
	int tempY = p.y;
	p.x = tempX*cosTheta - tempY*sinTheta + 0.5;
	p.y = tempX*sinTheta + tempY*cosTheta + 0.5;
}
#endif
