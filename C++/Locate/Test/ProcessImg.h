#ifndef _PROCESS_IMG
#define _PROCESS_IMG

#include<vector>
#include <deque>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <cstdlib>

using namespace cv;
using namespace std;

int SaveImg(Mat mat, int flag);

bool ProcessImg(Mat& img, Point& center, double& theta, double& width, double& length);

Mat localOTSU(Mat img, Size block);

Vector<Point> getBound(Vector<Point> vP);

void RankLines(vector<Vec4i>& lines);

bool CompareSlop(const Vec2f &l1, const Vec2f &l2);

bool CompareTheta(const Vec2f &l1, const Vec2f &l2);

bool ComparePointX(const Point &p1, const Point &p2);

bool GetCenterAndWidthAndLength(vector<Point>& vecPoints, const double contourTheta, Point& contourCenter, double& contourWidth, double& contourLength);

double GetTheta(Mat img);

void RotateContour(vector<Point>& p, const double theta);

void RotatePoint(Point &p, double theta);

bool GetCenterAndWidthAndLength(vector<Point>& vecPoints, const double theta, Point& center, double& width, double& length )
{
	/*sort(vecPoints.begin(), vecPoints.end(), ComparePointX);
	cout << vecPoints[0].x << endl << vecPoints[vecPoints.size() - 1].x << endl;
	cout << vecPoints[0].y << endl << vecPoints[vecPoints.size() - 1].y << endl;
	cout << abs(vecPoints[0].x - vecPoints[vecPoints.size() - 1].x) << endl;*/
	
	RotateContour(vecPoints, 0-theta);

	/*Mat houghImg(2048, 2048, CV_8U, Scalar::all(0));
	cout << endl << houghImg.channels() << endl;
	for (int i = 0; i < vecPoints.size(); i++)
	{
		Point p(vecPoints[i].x + 100, vecPoints[i].y+1200);
		cout << p.x << " " << p.y << endl;
		houghImg.at<uchar>(p) = 255;
	}
	imshow("rotateContour", houghImg);
	waitKey();*/

	//对符合条件的所有轮廓点按照X进行排序
	sort(vecPoints.begin(), vecPoints.end(), ComparePointX);
	length = abs(vecPoints[0].x - vecPoints[vecPoints.size() - 1].x);

	int nextStart = 0;
	int currentStart = 0;
	int windowSize = 100;
	int pCount = 0;
	int step = 20;
	int maxCount = 0;
	int maxStart = 0;
	/*for (int i = 0; i < vecPoints.size(); i++){
		if (abs(vecPoints[i].x - vecPoints[currentStart].x) < windowSize){
			pCount++;
			if (abs(vecPoints[i].x - vecPoints[currentStart].x - step) < 3){
				nextStart = i;
			}
		}
		else{
			if (pCount > maxCount){
				maxCount = pCount;
				maxStart = currentStart;
			}
			pCount = 0;
			i = nextStart;
			currentStart = nextStart;
		}
	}*/

	/*for (int i = 0; i < maxCount; i++)
	{
		
		if (vecPoints[maxStart + i].y < minYPoint.y)
			minYPoint = vecPoints[maxStart + i];
		else if (vecPoints[maxStart + i].y > maxYPoint.y)
			maxYPoint = vecPoints[maxStart + i];
	}*/

	Point minYPoint(5000,5000), maxYPoint(0,0);

	for (int i = 0; i < vecPoints.size(); i++)
	{
		if (abs(vecPoints[vecPoints.size()-1].x/2 + vecPoints[0].x/2 - vecPoints[i].x) < 2 )
		{
			if (vecPoints[i].y < minYPoint.y)
			{
				minYPoint = vecPoints[i];
			}

			if (vecPoints[i].y > maxYPoint.y)
			{
				maxYPoint = vecPoints[i];
			}

		}
	}

	center.x = (minYPoint.x + maxYPoint.x) / 2 + 0.5;
	center.y = (minYPoint.y + maxYPoint.y) / 2 + 0.5;
	width = maxYPoint.y - minYPoint.y;

	RotatePoint(center, theta);
	return true;
}

void RotateContour(vector<Point>& vp, const double theta)
{
	//cout << cos(60.0/180.0*3.1415926);
	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	for (int i = 0; i < vp.size(); i++)
	{
		int tempX = vp[i].x;
		int tempY = vp[i].y;
		vp[i].x = tempX*cosTheta - tempY*sinTheta + 0.5;
		vp[i].y = tempX*sinTheta + tempY*cosTheta + 0.5;
	}
}

void RotatePoint(Point &p, const double theta)
{
	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	int tempX = p.x;
	int tempY = p.y;
	p.x = tempX*cosTheta - tempY*sinTheta + 0.5;
	p.y = tempX*sinTheta + tempY*cosTheta + 0.5;
}

double GetTheta(Mat img)
{
	vector<Vec2f> lines;
	int houghThreshold = 100;
	HoughLines(img, lines, 1, CV_PI / 360, houghThreshold);
	sort(lines.begin(), lines.end(), CompareTheta);

	float precision = 0.05;

	float currentSum = lines[0][1];
	int currentCount = 1;
	float resSum = 0.0;
	int resCount = 0;

	for (size_t i = 1; i < lines.size(); i++)
	{
		if (abs(lines[i][1] - lines[i - 1][1]) < precision)
		{
			currentSum += lines[i][1];
			currentCount++;
		}
		else
		{
			if (currentCount > resCount)
			{
				resCount = currentCount;
				resSum = currentSum;
			}
			currentSum = lines[i][1];
			currentCount = 1;
		}
	}
	if ( abs(resSum) < 0.000001 )
	{
		resSum = currentSum;
		resCount = currentCount;
	}
	//imshow("Lines", img);
	//waitKey();
	return resSum/resCount - CV_PI/2;
}

int SaveImg(Mat mat, int flag)
{
	//flag=1 不新建文件夹；flag=0新建文件夹
	char path[100] = "E:\\焊接定位\\pictures\\";
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
	strcat(path, tmp);
	if (0 == flag && (_access(path, 0) == -1))
	{
		_mkdir(path);
	}
	strftime(tmp, sizeof(tmp), "%H_%M_%S", localtime(&t));
	strcat(path, "\\");
	strcat(path, tmp);
	strcat(path, ".jpg");
	int save = cv::imwrite(path, mat);
	cout << path << (save ? " Saved!" : "Can't be saved") << endl;
	//imwrite(fileName,img);
	return save;
}

/*vector<Point3i> findCenters()
{
	;
}
*/


bool ProcessImg(Mat& img, Point& contourCenter, double& contourTheta, double& contourWidth, double& contourLength)
{
	
	Mat res;
	res = img.clone();

	//转灰度
	Mat grayimg;
	Mat binaryimg;
	cvtColor(img, grayimg, CV_RGB2GRAY);
	
	//平滑
	GaussianBlur(grayimg, grayimg, Size(13, 13), 0.5, 0.5);
	medianBlur(grayimg, grayimg, 7);
	
	//大津法阈值分割
	cv::threshold(grayimg,grayimg,0,255,THRESH_OTSU);
	//grayimg = grayimg < 45;
	//imshow("ret",res);
	imshow("bw", grayimg);

	//
	


	
	//

	waitKey();

	//轮廓识别
	vector<vector<Point>> contours;
	vector<vector<Point>> resContours;
	findContours(grayimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	/*drawContours(grayimg, contours, 3, Scalar(200, 100, 100), CV_FILLED, 8);
	imshow("b", grayimg);
	waitKey();*/

	//找出符合条件的所有轮廓线
	for (int i = 0; i < contours.size(); i++){
		//设定轮廓面积的上限与下限
		int maxArea = 9000000;
		int minArea = 1000;
		double tmpContourArea = contourArea(contours[i]);
		double ratioOfPerimeterArea = 1.0 * contours[i].size() / tmpContourArea;
		double ratioOfPerimeterAreaThreshold = 0.02;

		if (tmpContourArea < maxArea && tmpContourArea > minArea )
				//&& ratioOfPerimeterArea > ratioOfPerimeterAreaThreshold)
		{
			resContours.push_back(contours[i]);
		}
	}

	//无符合条件的轮廓则返回空点
	if ( resContours.empty() )
	{
		return false;
	}


	
	Mat houghImg(img.size(), CV_8U, Scalar::all(0));
	vector<Point> contourPoints;
	for (size_t i = 0; i < resContours.size(); i++)
	{
		//在空白图上绘制所有符合条件的轮廓，用于求总体的斜率
		drawContours(houghImg, resContours, i, 255, 1);
		drawContours(img, resContours, i, Scalar(0, 0, 255), 1);
		//合并符合条件的所有轮廓用于计算中心和宽度
		contourPoints.insert(contourPoints.end(), resContours[i].begin(), resContours[i].end());
	}




	contourTheta = GetTheta(houghImg);
	imshow("houghImg", houghImg);
	
	////////
	
	struct edge
	{
		int x[4];
		int y[4];
		int count;
	};
	
	//edge* pos = (edge *)malloc(int((houghImg.size()).width) * sizeof(edge));
	//edge* pos_y = (edge *)malloc(int((houghImg.size()).width) * sizeof(edge));
	
	//int *pos_c = (int *)malloc(int((houghImg.size()).width) * sizeof(int)); // 记录图像的每一列有几个边线点
	
	//memset(pos_c, 0, int((houghImg.size()).width) * sizeof(int));  
	
	vector<edge> edges;
	edge edg;

	for (size_t i = 0; i < int((houghImg.size()).width); ++i)
	{
		edg.count = 0;
		for (size_t j = int((houghImg.size()).height * 0.1 ) ; j < int((houghImg.size()).height); ++j)
		{
			uchar* p = houghImg.ptr<uchar>(j);
			if (p[i] > 100)
			{
				if (edg.count >= 4) break;
				edg.x[edg.count] = i;
				edg.y[edg.count] = j;
				edg.count++;
			}

		}

		if (edg.count == 3)
			edges.push_back(edg);

	}


	for (int i = 0; i < edges.size(); i++)
	{
		edg = edges[i];
		int x = edg.x[0];
		int y = (edg.y[0] + edg.y[1] + 0.5) / 2;
		
		//houghImg.ptr<Vec3b>(y)[x] = Scalar(255,255,255);
		img.at<Vec3b>(y,x) = Vec3b(0, 255, 0);
	}


	imshow("houghImg2", houghImg);

	////////
	
	
	waitKey();
	
	bool findCenter = GetCenterAndWidthAndLength(contourPoints, contourTheta, contourCenter, contourWidth, contourLength);
	//circle(img, contourCenter, contourWidth / 2, Scalar(255, 255, 255), 1);
	imshow("res", img);
	waitKey();
	return true;
	
}

Mat localOTSU(Mat img, Size block)
{
	Mat res = Mat::zeros((img.size()).height, (img.size()).width, CV_8U);
	cout << (img.size()).width << endl;
	for (int i = 0; i < (int)(img.size()).height / block.height; i++)
	{
		for (int j = 0; j < (int)(img.size()).width / block.width; j++)
		{
			cout << j*block.width << ' ' << i*block.height << ' ' << block.width << ' ' << block.height << endl;
			//Rect r(0,300,1024,400);
			Rect r(j*block.width, i*block.height, block.width, block.height);
			threshold(img(r), img(r), 1, 100, THRESH_OTSU);
			//imshow("img",img(r));
			//cout << j*block.height << ' ' << i*block.width << ' ' << block.width << ' ' << block.height << endl;
			//waitKey(1000);
		}

	}
	imshow("res", img);
	waitKey();
	//Mat res = img.clone();
	return res;
}

Vector<Point> getBound(Vector<Point> vP)
{
	int top = 9999, bottom = 0, left = 9999, right = 0;
	for (int i = 0; i < vP.size(); i++)
	{
		if (vP[i].x < left)
		{
			left = vP[i].x;
		}
		if (vP[i].x > right)
		{
			right = vP[i].x;
		}
		if (vP[i].y < top)
		{
			top = vP[i].y;
		}
		if (vP[i].y > bottom)
		{
			bottom = vP[i].y;
		}
	}
	Vector<Point> resVec;
	resVec.push_back(Point(left, top));
	resVec.push_back(Point(right, bottom));
	return resVec;
}

void RankLines(vector<Vec4i>& lines)
{
	for (int i = 0; i<lines.size(); i++)
	{
		Vec4i l = lines[i];
		if (l[0] > l[2])
		{
			int tempX, tempY;
			tempX = l[2], tempY = l[3];

			l[2] = l[0], l[3] = l[1];
			l[0] = tempX, l[1] = tempY;
		}
	}
}

bool CompareSlop(const Vec2f &l1, const Vec2f &l2)
{
	return l2[1] > l1[1];
}

bool CompareTheta(const Vec2f &l1, const Vec2f &l2)
{
	return l1[1] > l2[1];
}

bool ComparePointX(const Point &p1, const Point &p2)
{
	return p1.x < p2.x;
}

#endif
