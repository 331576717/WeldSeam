#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "BFS.h"
using namespace std;
using namespace cv;
 Mat& processImage(Mat& mat);
 
int main()
{
	//声明IplImage指针
  IplImage* pFrame = NULL;

 //获取摄像头
 CvCapture* pCapture = cvCreateCameraCapture(0);
 ////
 //// //创建窗口
  cvNamedWindow("video", 1);
	int intCount=0;
  //cvNamedWindow("result",1);	
 
  //显示视屏
  
   //getchar();
  while(1)
  {
	 cv::Mat image;
	 
	double duration;
	duration = static_cast<double>(cv::getTickCount());
	
	image = processImage(imread("E:\\焊接定位\\pictures\\straightSeam\\Color_120_1.jpg"));

	duration = static_cast<double>(cv::getTickCount()) - duration;
	duration /= cv::getTickFrequency();
	cout << duration << endl;
   	 imshow("hello",image);
	 cv::waitKey();
     pFrame=cvQueryFrame( pCapture );
	  Mat fr(pFrame);
	  
	  fr = processImage(fr);
      if(!pFrame)break;
	 // vector<Mat> planes;
	 // cv::split(fr,planes);
      imshow("video",fr);
	  //waitKey();
	  //waitKey(100);
      char c=cvWaitKey(100);
	  //int intCount = 0;
	  string s = "Color_120_";
	  if(c=='s')
	  {
		  stringstream streamCount;
		  streamCount << intCount;
		  string stringCount;
		  streamCount >> stringCount;
		  string fileName = s + stringCount+".jpg";
		  cout << (cvSaveImage(fileName.c_str(),pFrame)? fileName+" Saved!" : "Can't be saved") << endl;
		 
		 //image = processImage(imread(fileName));
		  
		 imwrite(fileName,fr);
		// imshow("result",image);
		 intCount++;
		  
	 }

  }

	////cvReleaseCapture(&pCapture);
	////cvDestroyWindow("video");
	////cvDestroyWindow("result");
  
  return 0;

}


 Mat& processImage(Mat& mat)
 {

	 
	
	 Mat tempMat = mat.clone();
	//高斯滤波和中值滤波
	cvtColor(mat,tempMat,CV_RGB2GRAY);
	
	GaussianBlur(tempMat,tempMat,Size(13,13),0.5,0.5);
	
	medianBlur(tempMat,tempMat,7);
	
	
	//阴影归一化
	/*cv::Mat element(19,9,CV_8U,cv::Scalar(1));
	Mat tophatMat;
	cv::morphologyEx(tempMat,tophatMat,cv::MORPH_TOPHAT,element);
	Mat blackhatMat;
	cv::morphologyEx(tempMat,blackhatMat,cv::MORPH_BLACKHAT,element);
	cv::add(tempMat,tophatMat,tophatMat);
	cv::addWeighted(tophatMat,1,blackhatMat,-1,0,tempMat);*/
	
	Mat bw;// = (tempMat < 110);
	
	cv::adaptiveThreshold(tempMat,bw,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::THRESH_BINARY_INV,71,61);
	//imshow("mat",bw);
	//waitKey();
	//imwrite("bw.jpg",bw);
	
	bw = bwLabelForce(bw,0.001);

	for(int i=1; i<(mat.size()).height; ++i)
	{
		uchar* pMat = mat.ptr<uchar>(i);
		uchar* pBw = bw.ptr<uchar>(i);
		for(int j=1; j<(mat.size()).width; ++j)
		{
			if(255 ==pBw[j])
			{
				pMat[3*j] = 0;
				pMat[3*j+1] = 0;
				pMat[3*j+2] = 255;
			}
		}
	}

	//imshow("mat",mat);
	//waitKey();

	//vector<Point> location = getLocate(bw);

	
	/*if(!location.empty())
	{
		cv::circle(mat,location[0],10,Scalar(0,255,255));
		cv::circle(mat,location[1],10,Scalar(0,255,255));
		
		//double disX0 = (768-(int)location[0].y)/19.8 + 6.2;
		//double disY0 = (int)location[0].x/19.8 - 28.8;

		//double disX1 = (768-(int)location[1].y)/19.8 + 6.2;
		//double disY1 = (int)location[1].x/19.8 -28.8;
		//unsigned int x0Pulse = disX0 / 3 * 10000;
		//unsigned int y0Pulse = disY0 / 3 * 10000;
		//unsigned int x1Pulse = disX1 / 3 * 10000;
		//unsigned int y1Pulse = disY1 / 3 * 10000;

		//string x0;
		//stringstream tempStream;
		//tempStream << location[0].x;
		//x0 = tempStream.str();
		////cout << x0 << endl;
		//tempStream << x1Pulse;
		//tempStream << y0Pulse;
		//tempStream << y1Pulse;
		//cv::putText(mat,x0,Point(location[0].x,location[0].y-20),0,0.6,Scalar(255,0,255),1);
		//cout << "x0Pulse: " << x0Pulse << "  " << "y0Pulse: " << y0Pulse << "   " << "x1Pulse: " << x1Pulse << "  " << "y1Pulse: " << y1Pulse<< endl;

		vector<string> locationText = getLocationText(location);
		for(int i=0; i < locationText.size(); ++i)
		{
			cv::putText(mat,locationText[i],Point(location[i].x-250,location[i].y-40),0,0.4,Scalar(0,0,255),1);
			//cv::putText(mat,locationText[i],Point(location[i].x-500,location[i].y-20),FONT_HERSHEY_DUPLEX,0.4,Scalar(0,255,255),1);
			cout << locationText[i] << endl;
		}
	}
	else
		cout << "No fitable area!" << endl;


	//imshow("mat",mat);
	//waitKey();
	//vector<vector<Point> >contours,resContours;
	//cv::findContours(bw,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	//	
	//for(int i=0; i < contours.size(); ++i)
	//{
	//	//cout << i << ": " << contourArea(contours[i]) << endl;
	//	if(cv::contourArea(contours[i])<2000 && cv::contourArea(contours[i])>1000)
	//	{
	//		resContours.push_back(contours[i]);
	//	}
	//	
	//}
	//cv::Mat result(bw.size(),CV_8U,cv::Scalar(255));
	//cv::drawContours(mat,resContours,-1,cv::Scalar(255, 255,255),1);
	////imshow("mat",mat);
	//imshow("result1",mat);
	*/
	return mat;
 }