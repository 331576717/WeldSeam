#include<vector>
#include <deque>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <algorithm>
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

	//�Է������������������㰴��X��������
	sort(vecPoints.begin(), vecPoints.end(), ComparePointX);
	length = abs(vecPoints[0].x - vecPoints[vecPoints.size() - 1].x);

	int nextStart = 0;
	int currentStart = 0;
	int windowSize = 100;
	int pCount = 0;
	int step = 20;
	int maxCount = 0;
	int maxStart = 0;
	for (int i = 0; i < vecPoints.size(); i++){
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
	}

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
	//flag=1 ���½��ļ��У�flag=0�½��ļ���
	char path[100] = "E:\\���Ӷ�λ\\pictures\\";
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

bool ProcessImg(Mat& img, Point& contourCenter, double& contourTheta, double& contourWidth, double& contourLength)
{
	
	Mat res;
	res = img.clone();

	//ת�Ҷ�
	Mat grayimg;
	Mat binaryimg;
	cvtColor(img, grayimg, CV_RGB2GRAY);
	
	//ƽ��
	GaussianBlur(grayimg, grayimg, Size(13, 13), 0.5, 0.5);
	medianBlur(grayimg, grayimg, 7);
	
	//�����ֵ�ָ�
	cv::threshold(grayimg,grayimg,50,70,THRESH_OTSU);
	//imshow("bw", grayimg);
	//waitKey();

	//����ʶ��
	vector<vector<Point>> contours;
	vector<vector<Point>> resContours;
	findContours(grayimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	//�ҳ���������������������
	for (int i = 0; i < contours.size(); i++){
		//�趨�������������������
		int maxArea = 90000;
		int minArea = 1000;
		double tmpContourArea = contourArea(contours[i]);
		double ratioOfPerimeterArea = 1.0 * contours[i].size() / tmpContourArea;
		double ratioOfPerimeterAreaThreshold = 0.02;

		if (tmpContourArea < maxArea && tmpContourArea > minArea 
				&& ratioOfPerimeterArea > ratioOfPerimeterAreaThreshold){
			resContours.push_back(contours[i]);
		}
	}

	//�޷��������������򷵻ؿյ�
	if ( resContours.empty() )
	{
		return false;
	}

	
	Mat houghImg(img.size(), CV_8U, Scalar::all(0));

	vector<Point> contourPoints;
	for (size_t i = 0; i < resContours.size(); i++)
	{
		//�ڿհ�ͼ�ϻ������з��������������������������б��
		drawContours(houghImg, resContours, i, 255, 1);
		
		//�ϲ����������������������ڼ������ĺͿ��
		contourPoints.insert(contourPoints.end(), resContours[i].begin(), resContours[i].end());
	}

	contourTheta = GetTheta(houghImg);
	//imshow("houghImg", houghImg);
	//waitKey();
	
	bool findCenter = GetCenterAndWidthAndLength(contourPoints, contourTheta, contourCenter, contourWidth, contourLength);
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