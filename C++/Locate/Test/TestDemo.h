#ifndef _TEST_DEMO
#define _TEST_DEMO

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "SendData.h"
#include "MotionPath.h"


void TestMotionPath()
{
	Mat img(500, 500, CV_8UC1, Scalar(255));
	putText(img, "Start", Point(90, 60), 1, 3, Scalar(0));

	for (size_t i = 2; i < 10; i++)
	{
		Point center(i * 20, i * 20);
		circle(img, Point(cvRound(center.x), cvRound(center.y)), 1, Scalar(255 - i * 15 ), 2);
		vector<Point> pathPoints = GenerateZigzagPath(center, 0.25*CV_PI, 10, 10);
		for (int j = 0; j < pathPoints.size(); j++)
			circle(img, Point(cvRound(pathPoints[j].x), cvRound(pathPoints[j].y)), 2, Scalar(255 - i * 15 - j * 25), 2);
	}
	imshow("motion", img);
	waitKey();

}

#endif