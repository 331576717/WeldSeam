#ifndef _TEST_DEMO
#define _TEST_DEMO

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#include "MovePath.h"


void TestMotionPath()
{
	Mat img(1000, 300, CV_8UC1, Scalar(255));
	putText(img, "Start", Point(90, 60), 1, 3, Scalar(0));

	for (size_t i = 0; i < 10; i++)
	{
		Point center(i * 20, i * 20);
		vector<Point> pathPoints = GenerateZigzagPath(center, 0.25*CV_PI, 10, 10);
		for (int j = 0; j < pathPoints.size(); j++)
			circle(img, Point(cvRound(pathPoints[i].x), cvRound(pathPoints[i].y)), 2, Scalar(0, 0, 255), 2);
	}
	imshow("motion", img);
	waitKey();

}

#endif