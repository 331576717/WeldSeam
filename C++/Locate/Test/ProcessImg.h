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

Point ProcessImg(Mat img);

Mat localOTSU(Mat img, Size block);

Vector<Point> getBound(Vector<Point> vP);

void RankLines(vector<Vec4i>& lines);

bool CompareSlop(Vec2f &l1, Vec2f &l2);

bool CompareTheta(Vec2f &l1, Vec2f &l2);

bool ComparePointX(Point &p1, Point &p2);

bool GetCenterAndWidth(vector<Point>& vecPoints, double theta, Point& center, double& width);

double GetTheta(Mat img);

void RotateContour(vector<Point>& p, double theta);

void RotatePoint(Point &p, double theta);

bool GetCenterAndWidth(vector<Point>& vecPoints, double theta, Point& center, double& width)
{
	RotateContour(vecPoints, theta);

	sort(vecPoints.begin(), vecPoints.end(), ComparePointX);
	
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
			if (abs(vecPoints[i].x - vecPoints[currentStart].x - step) < 5){
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

	Point minYPoint(vecPoints[maxStart]), maxYPoint(vecPoints[maxStart]);

	for (int i = 0; i < maxCount; i++)
	{
		if (vecPoints[maxStart + i].y < minYPoint.y)
			minYPoint = vecPoints[maxStart + i];
		else if (vecPoints[maxStart + i].y > maxYPoint.y)
			maxYPoint = vecPoints[maxStart + i];
	}

	center.x = (minYPoint.x + maxYPoint.x) / 2 + 0.5;
	center.y = (minYPoint.y + maxYPoint.y) / 2 + 0.5;

	RotatePoint(center, 0-theta);
	return true;
}

void RotateContour(vector<Point>& vp, double theta)
{
	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	for (int i = 0; i < vp.size(); i++)
	{
		int tempX = vp[i].x;
		int tempY = vp[i].y;
		vp[i].y = tempX*sinTheta + tempY*cosTheta + 0.5;
		vp[i].x = tempX*cosTheta - tempY*sinTheta + 0.5;
	}
}

void RotatePoint(Point &p, double theta)
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
	HoughLines(img, lines, 1, CV_PI / 360, 100);
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
	if ( abs(resSum) < 0.001 )
	{
		resSum = currentSum;
		resCount = currentCount;
	}
	//imshow("Lines", img);
	//waitKey();
	return resSum/resCount;
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

Point ProcessImg(Mat img)
//int proc(string picName)
{
	//统计程序用时
	//double time = 0;
	//time = (double)getTickCount();
	
	Mat res;
	res = img.clone();
	//addWeighted( img, 1, background, -1, 0.0, img);
	Mat grayimg;
	Mat binaryimg;
	cvtColor(img, grayimg, CV_RGB2GRAY);
	
	GaussianBlur(grayimg, grayimg, Size(13, 13), 0.5, 0.5);
	
	//imshow("gray", grayimg);
	//waitKey();
	
	medianBlur(grayimg, grayimg, 7);
	//blur(grayimg, grayimg, Size(5, 5));
	//equalizeHist(grayimg, grayimg);
	//medianBlur(grayimg, grayimg, 7);
	cv::threshold(grayimg,grayimg,50,70,THRESH_OTSU);
	//grayimg = grayimg > 240;
	//imshow("bw", grayimg);
	//waitKey();
	vector<vector<Point>> contous;

	vector<vector<Point>> resContous;
	findContours(grayimg, contous, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	for (int i = 0; i < contous.size(); i++){
		if (contourArea(contous[i]) > 1000){
			
			resContous.push_back(contous[i]);
			
			//imshow("houghImg", houghImg);
			//waitKey();
			
			//circle(res, Point(cvRound(center.x), cvRound(center.y)), 5, Scalar(255,0,0), 1);
			//imshow("res", res);
			//waitKey();
		}
		else
			drawContours(grayimg, contous, i, 0, -1);
	}

	Point center;
	double width;
	Mat houghImg(img.size().width, img.size().height, CV_8U, Scalar::all(0));

	vector<Point> contourPoints;
	for (size_t i = 0; i < resContous.size(); i++)
	{
		drawContours(houghImg, resContous, i, 255, 1);
		contourPoints.insert(contourPoints.end(), resContous[i].begin(), resContous[i].end());
	}

	double theta = GetTheta(houghImg);
	
	bool findCenter = GetCenterAndWidth(contourPoints, theta, center, width);
	return center;
	/*Canny(grayimg, binaryimg, 15, 30);
	
	imshow("Canny", binaryimg);
	waitKey();
	//= grayimg < 100;
	//localOTSU(grayimg,Size(60,60));
	
	//cv::adaptiveThreshold(grayimg,binaryimg,255,cv::THRESH_BINARY_INV,cv::ADAPTIVE_THRESH_GAUSSIAN_C,31,30);
	//binaryimg = (grayimg<30);
	//medianBlur(binaryimg, binaryimg, 5);
	imshow("bw",binaryimg);
	waitKey();
	vector<vector<Point>> contous;
	findContours(binaryimg,contous,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	//contous.
	//测试总共检测到的轮廓数
	//cout << "contous size:" << contous.size() << endl;
	Vector<Point> bound;
	for (int i=0; i < contous.size(); i++)
	{
		//通过轮廓边缘像素数做第一步过滤（还差像素区域平均值过滤）
		if (contourArea(contous[i]) > 100)
		{
			//cout << "contou area:" << cv::contourArea(*iter)<<endl;
			//tarcontous.push_back(*iter);
			drawContours(res,contous,i,Scalar(0,255,0),-1);
			bound = getBound(contous[i]);
			//框定扫描范围
			line(res, bound[0], Point(bound[1].x, bound[0].y), Scalar(0,0,255),1);
			line(res, bound[0], Point(bound[0].x, bound[1].y), Scalar(0,0,255),1);
			line(res, Point(bound[0].x, bound[1].y), bound[1], Scalar(0,0,255),1);
			line(res, Point(bound[1].x, bound[0].y), bound[1], Scalar(0,0,255),1);
			//imshow("bound",res);
			//waitKey();
			cout << contourArea(contous[i]) << endl;
		}
		else
			drawContours(res,contous,i,Scalar(255,255,255),-1);

		i++;
	}
	//medianBlur(binaryimg,binaryimg,3);
	
	
	vector<Vec4i> lines;
	HoughLinesP(binaryimg, lines, 1, CV_PI / 180, 150, 100, 1000);
	//vector<Vec4i> lines;
	//HoughLinesP(binaryimg, lines, 1, CV_PI / 180, 50, 50, 10);
	//RankLines(lines);
	//sort(lines.begin(), lines.end(), CompareSlop);
	//sort(lines.begin(), lines.end(), [](Vec2f const& a, Vec2f const& b){ return a[1] > b[1]; });

	//imshow("Res", res);
	//waitKey();
	vector<Point> linesP;
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i tempLine = lines[i];
		//line(res, Point(tempLine[0], tempLine[1]), Point(tempLine[2], tempLine[3]), Scalar(255, 255,0), 1);
		linesP.push_back(Point(tempLine[0], tempLine[1]));
		linesP.push_back(Point(tempLine[2], tempLine[3]));

	}
	Vector<Point> bound;
	bound = getBound(linesP);
	//框定扫描范围
	line(res, bound[0], Point(bound[1].x, bound[0].y), Scalar(0, 0, 255), 1);
	line(res, bound[0], Point(bound[0].x, bound[1].y), Scalar(0, 0, 255), 1);
	line(res, Point(bound[0].x, bound[1].y), bound[1], Scalar(0, 0, 255), 1);
	line(res, Point(bound[1].x, bound[0].y), bound[1], Scalar(0, 0, 255), 1);
	imshow("lines", res);
	waitKey();
	Vec2f line1, line2;
	for (int i = 0; i<2; i++)
		line1[i] = line2[i] = 0.0;

	//for (int i = 0; i<lines.size() - 1; i++)
	//{
	//	//Vec4i line1 = lines[i];
	//	//line(res,Point(line1[0], line1[1]), Point(line1[2], line1[3]), Scalar(255, 255, 255), 1);
	//	Vec2f tempLine1 = lines[i], tempLine2 = lines[i + 1];
	//	
	//	line1[0] += tempLine1[0];
	//	line1[1] += tempLine1[1];
	//	
	//	//cout << (double)tempSlop1 << endl << (double)tempSlop2 << endl;
	//	if (abs(tempLine1[1] - tempLine2[1]) < 0.3)
	//	{
	//		continue;
	//	}
	//	else
	//	{
	//		line1[0] /= (i + 1);
	//		line1[1] /= (i + 1);
	//		
	//		for (int j = i + 1; j<lines.size(); j++)
	//		{
	//			tempLine2 = lines[j];
	//			line2[0] += tempLine2[0] / (lines.size() - i - 1);
	//			line2[1] += tempLine2[1] / (lines.size() - i - 1);
	//		}
	//		break;
	//	}
	//}

	//line(res, Point(line1[0], line1[1]), Point(line1[2], line1[3]), Scalar(255, 255, 255), 2);
	//line(res, Point(line2[0], line2[1]), Point(line2[2], line2[3]), Scalar(255, 255, 255), 2);
	//cout << 1.0*(line1[3]-line1[1])/(line1[2]-line1[0]) * line1[0];
	//double k1 = 0.0, k2 = 0.0;
	//k1 = 1.0*(line1[3] - line1[1]) / (line1[2] - line1[0]);
	//k2 = 1.0*(line2[3] - line2[1]) / (line2[2] - line2[0]);
	//double interSectionX = (k1*line1[0] - k2*line2[0] + line2[1] - line1[1]) / (k1 - k2);
	//double interSectionY = k1*(interSectionX - line1[0]) + line1[1];

	//cout << "X:" << cvRound(interSectionX) << "  Y:" << cvRound(interSectionY) << endl;
	double interSectionX = (line2[0] * sin(line1[1]) - line1[0] * sin(line2[1])) / 
					(sin(line1[1]) * cos(line2[1]) - sin(line2[1]) * cos(line1[1]));
	double interSectionY = (line1[0] * cos(line2[1]) - line2[0] * cos(line1[1])) /
					(sin(line1[1]) * cos(line2[1]) - sin(line2[1]) * cos(line1[1]));
	//circle(res, Point(cvRound(interSectionX), cvRound(interSectionY)), 5, Scalar(255, 255, 255), 3);
	//imshow("RES", res);
	//waitKey();
	Point p(cvRound(interSectionX), cvRound(interSectionY));
	return p;
	//imshow("res", res);
	//waitKey();

	//namedWindow("binary",1);
	vector<Vec4i> lines;
	HoughLinesP(binaryimg,lines,1,CV_PI/36,50,50,10);
	//求得Hough变换获得的直线的均值
	cout << lines.size();
	Vec4f resLine;
	resLine[0] = 0.0;
	resLine[1] = 0.0;
	resLine[2] = 0.0;
	resLine[3] = 0.0;
	for( size_t i = 0; i < lines.size(); i++ )
	{
	Vec4i l = lines[i];
	resLine[0] += l[0];
	resLine[1] += l[1];
	resLine[2] += l[2];
	resLine[3] += l[3];
	}
	resLine[0] /= 7;
	resLine[1] /= 7;
	resLine[2] /= 7;
	resLine[3] /= 7;
	double k = -1/((resLine[3]-resLine[1])/(resLine[2]-resLine[0]));
	Point start = bound[0], end = bound[0];
	//开始逐步进行扫描，step控制两条扫描路径之间的间隔
	while(start.x <= bound[1].x && start.x >= bound[0].x)
	{
	int step = 5;
	int j=1;
	int count=0;
	Point center = Point(0,0);
	while(end.y <= bound[1].y && end.y >= bound[0].y)
	{
	end.x -= 1;
	end.y = cvRound(k * (start.x-1*j) + start.y - k * start.x);
	if(255 == (int)binaryimg.at<uchar>(Point(end.x,end.y)) && count < 2)
	{
	center.x += end.x/2;
	center.y += end.y/2;
	count++;
	if(2 == count)
	circle(res,Point(center.x,center.y),1,Scalar(255,255,255));
	}

	//circle(res,Point(end.x,end.y),0.5,Scalar(255,255,255));
	//line(binaryimg, Point(cvRound(start.x), cvRound(start.y)), Point(cvRound(end.x), cvRound(end.y)), Scalar(255,255,255), 1, CV_AA);
	j++;
	}
	start.x += step;
	end = start;
	//cout << i << endl;
	}
	imshow("process",res);
	waitKey();

	//float theta = 0;
	/*for( size_t i = 0; i < lines.size(); i++ )
	{
	float rho = lines[i][0];
	float theta = lines[i][1];
	theta = lines[i][1];
	//cout << theta;
	double a = cos(theta), b = sin(theta);
	double x0 = a*rho, y0 = b*rho;
	Point pt1(cvRound(x0 + 1000*(-b)),
	cvRound(y0 + 1000*(a)));
	Point pt2(cvRound(x0 - 1000*(-b)),
	cvRound(y0 - 1000*(a)));
	line( res, pt1, pt2, Scalar(0,0,255), 1, 8 );
	}

	//将轮廓画出   

	//drawContours(img,tarcontous,-1,Scalar(0,255,0),CV_FILLED);
	//cout<<"bianary chanel:"<<binaryimg.channels()<<endl;
	//cout<<"grayimg chanel:"<<grayimg.channels()<<endl;

	//将目标区域标注成特定灰度值，然后按既定灰度值寻找元素
	//drawContours(binaryimg,tarcontous,0,Scalar(targraycolor1),CV_FILLED);
	//drawContours(binaryimg,tarcontous,0,Scalar(targraycolor1),);
	//drawContours(binaryimg,tarcontous,1,Scalar(targraycolor2),CV_FILLED);

	namedWindow("result",1);
	imshow("result",binaryimg);

	//time = 1000*((double)getTickCount() - time)/getTickFrequency();
	//cout <<"用时："<< time <<endl;
	//cout << "总共用时：" <<　time <<endl;

	//namedWindow("final",1);*/
	

return center;
	
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

bool CompareSlop(Vec2f &l1, Vec2f &l2)
{
	return l2[1] > l1[1];
}

bool CompareTheta(Vec2f &l1, Vec2f &l2)
{
	return l1[1] > l2[1];
}

bool ComparePointX(Point &p1, Point &p2)
{
	return p1.x < p2.x;
}