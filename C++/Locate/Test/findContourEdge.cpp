#include <iostream>
#include <vector>
#include <deque>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#define  targraycolor1 250
#define  targraycolor2 200
int main()
{
	IplImage *pFrame = NULL;

	while(1)
	{
		CvCapture *pCapture = cvCreateCameraCapture(0);
		pFrame = cvQueryFrame(pCapture);

		Mat frame(pFrame);

		int proc(Mat);
		proc(frame);
	}
	
	//int proc(string);
	//string picName;
	//picName = "C:\\Users\\KIN\\Desktop\\straightSeasm\\Color_120_1.jpg";
	//proc(picName);
	return 0;
}

int proc(Mat img)
//int proc(string picName)
{
	//ͳ�Ƴ�����ʱ
	double time = 0;
	time = (double)getTickCount();

	//����IplImageָ��
	//Mat img = imread("F:\\������\\���ղ��������\\Color_120_13.jpg",CV_LOAD_IMAGE_COLOR);
	//Mat img = imread(picName,CV_LOAD_IMAGE_COLOR);
	if (!img.data)
	{
		cout << "read image error!" << endl;
		return 1;
	}
	/*namedWindow("sourse",1);
	imshow("sourse", img);*/

	Mat grayimg;
	cvtColor(img,grayimg,CV_RGB2GRAY);
	
	/*namedWindow("grayimg",1);
	imshow("grayimg", grayimg);*/

	Mat binaryimg; //= grayimg < 100;
	cv::adaptiveThreshold(grayimg,binaryimg,255,cv::THRESH_BINARY_INV,cv::ADAPTIVE_THRESH_GAUSSIAN_C,71,61);

	/*namedWindow("binary",1);
	imshow("binary",binaryimg);*/

	vector<vector<Point>> contous,tarcontous;
	//deque<deque<Point>> contous,tarcontous;
	findContours(binaryimg,contous,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	//�����ܹ���⵽��������
	cout << "contous size:" << contous.size() << endl;

	auto iter = contous.begin();
	while(iter != contous.end())
	{
		//ͨ��������Ե����������һ�����ˣ�������������ƽ��ֵ���ˣ�
		double area = cv::contourArea(*iter);
		//cout << "contou area:" << area <<endl;
		if (area < 50000 && area > 1000)
		{
			//cout << "contou area:" << cv::contourArea(*iter)<<endl;
			tarcontous.push_back(*iter);
		}
		iter++;
	}
	
	//����������   
	drawContours(img,tarcontous,0,Scalar(0,0,255),0);
	//drawContours(img,tarcontous,-1,Scalar(0,255,0),CV_FILLED);
	cout<<"bianary chanel:"<<binaryimg.channels()<<endl;
	cout<<"grayimg chanel:"<<grayimg.channels()<<endl;
	
	//��Ŀ�������ע���ض��Ҷ�ֵ��Ȼ�󰴼ȶ��Ҷ�ֵѰ��Ԫ��
	//drawContours(binaryimg,tarcontous,0,Scalar(targraycolor1),CV_FILLED);
	//drawContours(binaryimg,tarcontous,0,Scalar(targraycolor1),);
	//drawContours(binaryimg,tarcontous,1,Scalar(targraycolor2),CV_FILLED);

	/*namedWindow("result",1);
	imshow("result",binaryimg);*/

	time = 1000*((double)getTickCount() - time)/getTickFrequency();
	cout <<"��ʱ��"<< time <<endl;
	//cout << "�ܹ���ʱ��" <<��time <<endl;

	namedWindow("final",1);
	imshow("final",img);
	cvWaitKey(50);

	return 0;
}