#include <iostream>
#include <vector>
#include <deque>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <direct.h>
#include <time.h>
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
using namespace cv;
using namespace std;
Vector<Point> getBound(Vector<Point> vP)
{
	int top = 9999, bottom = 0, left = 9999, right = 0;
	for(int i=0; i < vP.size(); i++)
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
		if(vP[i].y > bottom)
		{
			bottom = vP[i].y;
		}
	}
	Vector<Point> resVec;
	resVec.push_back(Point(left,top));
	resVec.push_back(Point(right,bottom));
	return resVec;
}

Mat localOTSU(Mat img, Size block)
{
	Mat res = Mat::zeros((img.size()).height,(img.size()).width,CV_8U);
	cout << (img.size()).width << endl;
	for(int i = 0; i < (int)(img.size()).height / block.height; i++)
	{
		for (int j = 0; j < (int)(img.size()).width / block.width; j++)
		{ 
			cout << j*block.width << ' ' << i*block.height << ' ' << block.width << ' ' << block.height << endl;
			//Rect r(0,300,1024,400);
			Rect r(j*block.width, i*block.height, block.width, block.height);
			threshold(img(r),img(r),1,100,THRESH_OTSU);
			//imshow("img",img(r));
			//cout << j*block.height << ' ' << i*block.width << ' ' << block.width << ' ' << block.height << endl;
			//waitKey(1000);
		}
		
	}
	imshow("res",img);
	waitKey();
	//Mat res = img.clone();
	return res;
}
#define  targraycolor1 250
#define  targraycolor2 200
int saveImg(Mat mat,int flag)
{
	//flag=1 不新建文件夹；flag=0新建文件夹
	char path[100] = "E:\\焊接定位\\pictures\\";
	time_t t = time(0); 
	char tmp[64]; 
	strftime(tmp, sizeof(tmp), "%Y%m%d",localtime(&t) ); 
	strcat(path,tmp);
	if(0 == flag && (_access(path,0)==-1))
	{
		_mkdir(path);
	}
	strftime(tmp, sizeof(tmp), "%H_%M_%S",localtime(&t)); 
	strcat(path,"\\");
	strcat(path,tmp);
	strcat(path,".jpg");
	int save = cv::imwrite(path,mat);
	cout << path << (save? " Saved!" : "Can't be saved") << endl;
	//imwrite(fileName,img);
	return save;
}
int proc(Mat img);
int main()
{
	
	//std::ios::sync_with_stdio(false);
	//IplImage* pFrame = NULL;

	//获取摄像头
	//CvCapture* pCapture = cvCreateCameraCapture(-1);

	VideoCapture cap(0); // open the default camera
	//cap.set(CV_CAP_PROP_FRAME_WIDTH,1024.0);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT,768.0);
	
	int intCount  = 0;
	for(;;)
	{
		//pFrame=cvQueryFrame( pCapture );
		//if(!pFrame)break;
		//cvShowImage("video",pFrame);
		Mat frame = imread("E:\\焊接定位\\pictures\\20140526\\Color_120_4.jpg");
		//cap >> frame; // get a new frame from camera
		//cvtColor(frame, frame, CV_BGR2GRAY);
		//GaussianBlur(frame, frame, Size(7,7), 1.5, 1.5);
		//Canny(frame, frame, 0, 30, 3);
		
		proc(frame);
		//imshow("img",frame);
		char c=cvWaitKey(5);
		
		if(c=='s')
		{
			saveImg(frame,intCount);
			intCount++;
		}
	}
	
	
	return 0;
}

int proc(Mat img)
//int proc(string picName)
{
	//统计程序用时
	//double time = 0;
	//time = (double)getTickCount();

	//声明IplImage指针
	
	//Mat img = imread(picName,CV_LOAD_IMAGE_COLOR);
	if (!img.data)
	{
		cout << "read image error!" << endl;
		return 1;
	}
	/*namedWindow("sourse",1);
	imshow("sourse", img);*/
	//Mat background = imread("background.jpg");
	
	Mat res;
	res = img.clone();
	//addWeighted( img, 1, background, -1, 0.0, img);
	//imshow("res",img);
	//waitKey();
	Mat grayimg;
	cvtColor(img,grayimg,CV_RGB2GRAY);
	
	GaussianBlur(grayimg,grayimg,Size(13,13),0.5,0.5);

	medianBlur(grayimg,grayimg,7);
	//namedWindow("grayimg",1);
	//imshow("grayimg", grayimg);
	//waitKey();

	Mat binaryimg; //= grayimg < 100;
	//localOTSU(grayimg,Size(60,60));
	//cv::threshold(grayimg,binaryimg,0,60,THRESH_OTSU);
	cv::adaptiveThreshold(grayimg,binaryimg,255,cv::THRESH_BINARY_INV,cv::ADAPTIVE_THRESH_GAUSSIAN_C,51,21);
	//binaryimg = (grayimg>10);
	
	vector<vector<Point>> contous;
	findContours(binaryimg,contous,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	//contous.
	//测试总共检测到的轮廓数
	//cout << "contous size:" << contous.size() << endl;
	Vector<Point> bound;
	for (int i=0; i < contous.size(); i++)
	{
		//通过轮廓边缘像素数做第一步过滤（还差像素区域平均值过滤）
		if (contourArea(contous[i]) < 18000 && contourArea(contous[i]) > 100)
		{
			//cout << "contou area:" << cv::contourArea(*iter)<<endl;
			//tarcontous.push_back(*iter);
			drawContours(binaryimg,contous,i,Scalar(255,255,255),-1);
			bound = getBound(contous[i]);
			line(res, bound[0], Point(bound[1].x, bound[0].y), Scalar(255,0,0),1);
			line(res, bound[0], Point(bound[0].x, bound[1].y), Scalar(255,0,0),1);
			line(res, Point(bound[0].x, bound[1].y), bound[1], Scalar(255,0,0),1);
			line(res, Point(bound[1].x, bound[0].y), bound[1], Scalar(255,0,0),1);
			//imshow("bound",res);
			//waitKey();
			cout << contourArea(contous[i]) << endl;
		}
		else
			drawContours(binaryimg,contous,i,Scalar(0,0,0),-1);
		i++;
	}
	medianBlur(binaryimg,binaryimg,3);
	
	Canny(binaryimg,binaryimg,10,80);
	
	//namedWindow("binary",1);
	vector<Vec4i> lines;
	HoughLinesP(binaryimg,lines,1,CV_PI/36,50,50,10);
	
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
	}*/
	
		
	
	imshow("res",res);
	waitKey();

	
	
	//将轮廓画出   
	
	//drawContours(img,tarcontous,-1,Scalar(0,255,0),CV_FILLED);
	//cout<<"bianary chanel:"<<binaryimg.channels()<<endl;
	//cout<<"grayimg chanel:"<<grayimg.channels()<<endl;
	
	//将目标区域标注成特定灰度值，然后按既定灰度值寻找元素
	//drawContours(binaryimg,tarcontous,0,Scalar(targraycolor1),CV_FILLED);
	//drawContours(binaryimg,tarcontous,0,Scalar(targraycolor1),);
	//drawContours(binaryimg,tarcontous,1,Scalar(targraycolor2),CV_FILLED);

	/*namedWindow("result",1);
	imshow("result",binaryimg);*/

	//time = 1000*((double)getTickCount() - time)/getTickFrequency();
	//cout <<"用时："<< time <<endl;
	//cout << "总共用时：" <<　time <<endl;

	//namedWindow("final",1);
	imshow("final",res);
	waitKey(100);

	return 0;
}