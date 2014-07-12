#include <iostream>
#include <vector>
#include <deque>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <direct.h>
#include <time.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <Windows.h>
#include <fstream>

Point TestPoint()
{
	Point i(3, 5);
	return i;
}

int TestHoughLines(Mat img)
{
	Mat dst, color_dst;

	Canny(img, dst, 50, 200, 3);

	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 180, 0, 0);
	return 0;

}

void TestSendData()
{

}