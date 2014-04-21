#include<vector>
#include<list>
#include<deque>
#include<vector>
#include<map>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
using namespace cv;
using namespace std;

Mat bwLabel(Mat mat);
Mat bwLabelForce(Mat mat, double lowerBound = 0, double upperBound = 1);
int BFSForce(Mat mat, int xIndex, int yIndex, int count);
Mat BFS(Mat mat, int xIndex, int yIndex, int count);
vector<string> getLocationText(vector<Point> p);
vector<Point> getLocate(Mat mat);

Mat bwLabelForce(Mat mat, double lowerBound , double upperBound)
{
	
	Size size = mat.size();
	copyMakeBorder(mat,mat,1,1,1,1,BORDER_CONSTANT,Scalar(0,0,0));
	
	int height = size.height;
	int width = size.width;
	int label = 1;
	
	deque<int> suitableAera;
	int maxCount=0,secondMaxCount=0,maxLabel=0,secondMaxLabel=0;
	for(int i=1; i<height; ++i)
	{
		uchar* data = mat.ptr<uchar>(i);
		for(int j=1; j<width; ++j)
		{
			if(255 == (int)data[j])
			{
				//cout << "i: " << i << "  j:" << j << endl;
				int temp = BFSForce(mat,i,j,label);
				
				if( temp > height * width * lowerBound && temp < height * width * upperBound)
				{

					suitableAera.push_back(label);
				}
				
				label++;
			}
			
		}
	}
	
	
	Mat res(mat.size().height-2,mat.size().width-2,CV_8U);
	//int it = suitableAera[0];
	for(int i=1; i<mat.size().height-1; ++i)
	{
		uchar* src = mat.ptr<uchar>(i);
		uchar* dst = res.ptr<uchar>(i-1);
		for(int j=1; j<mat.size().width-1; ++j)
		{
			for(int i=0;i<suitableAera.size();i++)
			{
				if(src[j] == suitableAera[i])
				{
					dst[j-1] = 255;
				}
				else
				{
					dst[j-1] = 0;
				}
			}
		}
	}
	mat = res.clone();
	//imshow("res",mat);
	//waitKey();
	vector<Point> vec;
	return mat;
}

int BFSForce(Mat mat, int xIndex, int yIndex, int label)
{
	int count=0;
	//cout << mat.size();
	//cout << (int)mat.at<uchar>(xIndex,yIndex);
	deque<Point> points;
	points.push_back(Point(xIndex,yIndex));
	while(!points.empty())
	{
		//cout << "empty:" << points.empty() << endl;
		//cout << (int)mat.at<uchar>(1, 591);
		Point pt = points.front();
		
		uchar* data = mat.ptr<uchar>(pt.x - 1);
		if(255 == (int)data[pt.y-1])
		{
			points.push_back(Point(pt.x - 1, pt.y - 1));
			mat.at<uchar>(pt.x - 1, pt.y-1)=label;
			count++;
		}
		if(255 == (int)data[pt.y])
		{
			points.push_back(Point(pt.x - 1, pt.y));
			mat.at<uchar>(pt.x - 1, pt.y)=label;
			count++;
		}
		if(255 == (int)data[pt.y+1])
		{
			points.push_back(Point(pt.x - 1, pt.y + 1));
			mat.at<uchar>(pt.x - 1, pt.y + 1)=label;
			count++;
		}
		data = mat.ptr<uchar>(pt.x);
		if(255 == (int)data[pt.y-1])
		{
			points.push_back(Point(pt.x, pt.y - 1));
			mat.at<uchar>(pt.x, pt.y - 1)=label;
			count++;
		}
		if(255 == (int)data[pt.y+1])
		{
			points.push_back(Point(pt.x, pt.y + 1));
			mat.at<uchar>(pt.x, pt.y + 1)=label;
			count++;
		}
		data = mat.ptr<uchar>(pt.x+1);
		if(255 == (int)data[pt.y-1])
		{
			points.push_back(Point(pt.x + 1, pt.y - 1));
			mat.at<uchar>(pt.x + 1, pt.y - 1)=label;
			count++;
		}
		if(255 == (int)data[pt.y])
		{
			points.push_back(Point(pt.x + 1, pt.y));
			mat.at<uchar>(pt.x + 1, pt.y)=label;
			count++;
		}
		if(255 == (int)data[pt.y+1])
		{
			points.push_back(Point(pt.x + 1, pt.y + 1));
			mat.at<uchar>(pt.x + 1, pt.y + 1)=label;
			count++;
		}
		////四连通情况
		//if(255 == (int)mat.at<uchar>(pt.x - 1, pt.y))
		//{
		//	points.push_back(Point(pt.x - 1, pt.y));
		//	mat.at<uchar>(pt.x - 1, pt.y)=label;
		//	count++;
		//}
		// if(255 == (int)mat.at<uchar>(pt.x, pt.y - 1))
		// {
		//	points.push_back(Point(pt.x, pt.y - 1));
		//	mat.at<uchar>(pt.x, pt.y - 1)=label;count++;
		// }
		// if(255 == (int)mat.at<uchar>(pt.x + 1, pt.y))
		// {
		//	points.push_back(Point(pt.x + 1, pt.y));
		//	mat.at<uchar>(pt.x + 1, pt.y)=label;count++;
		// }
		// if(255 == (int)mat.at<uchar>(pt.x, pt.y + 1))
		// {
		//	 points.push_back(Point(pt.x, pt.y + 1));
		//	 mat.at<uchar>(pt.x, pt.y + 1)=label;count++;
		// }

		////补充四个斜角，八连通情况
		// if(255 == (int)mat.at<uchar>(pt.x - 1, pt.y - 1))
		// {
		//	points.push_back(Point(pt.x - 1, pt.y - 1));
		//	mat.at<uchar>(pt.x - 1, pt.y - 1)=label;count++;
		// }
		// if(255 == (int)mat.at<uchar>(pt.x + 1, pt.y - 1))
		// {
		//	 points.push_back(Point(pt.x + 1, pt.y - 1));
		//	 mat.at<uchar>(pt.x + 1, pt.y - 1)=label;count++;
		// }
		// if(255 == (int)mat.at<uchar>(pt.x + 1, pt.y + 1))
		// {
		//	points.push_back(Point(pt.x + 1, pt.y + 1));
		//	mat.at<uchar>(pt.x + 1, pt.y + 1)=label;count++;
		// }
		// if(255 == (int)mat.at<uchar>(pt.x - 1, pt.y + 1))
		// {
		//	points.push_back(Point(pt.x - 1, pt.y + 1));
		//	mat.at<uchar>(pt.x - 1, pt.y + 1)=label;count++;
		// }

		//mat.at<uchar>(pt.x,pt.y) = 0;
		points.pop_front();
		//cout << "ptIndex: " << pt << "ptValue: " << (int)mat.at<uchar>(1,591) << endl;
	}
	//imshow("mat",mat);
	//waitKey();
	
	return count;
}

Mat bwLabel(Mat mat)
{
	
	Size size = mat.size();
	copyMakeBorder(mat,mat,1,1,1,1,BORDER_CONSTANT,Scalar(0,0,0));
	//cout << mat.size() << endl;
	//imshow("mat",mat);
	//waitKey();
	int height = size.height;
	int width = size.width;
	int label = 1;
	for(int i=1; i<height; ++i)
	{
		uchar* data = mat.ptr<uchar>(i);
		for(int j=1; j<width; ++j)
		{
			
			if(255 == (int)data[j])
			{
				//cout << "i: " << i << "  j:" << j << endl;
				mat = BFS(mat,i,j,label);
				label++;
			}
			
		}

	}
	//imshow("mat",mat);
	//cout << count;
	Mat res(mat.size().height-2,mat.size().width-2,CV_8U);
	for(int i=1; i<mat.size().height-1; ++i)
	{
		uchar* src = mat.ptr<uchar>(i);
		uchar* dst = res.ptr<uchar>(i-1);
		for(int j=1; j<mat.size().width-1; ++j)
		{
			
			dst[j-1] = src[j];
			/*if(src[j] != 0)
				cout <<(int)dst[j-1] << endl;*/
		}

	}
	//imwrite("res.jpg",res);
	return res;
}

Mat BFS(Mat mat, int xIndex, int yIndex, int label)
{
	
	//cout << mat.size();
	//cout << (int)mat.at<uchar>(xIndex,yIndex);
	list<Point> points;
	points.push_back(Point(xIndex,yIndex));
	while(!points.empty())
	{
		//cout << "empty:" << points.empty() << endl;
		//cout << (int)mat.at<uchar>(1, 591);
		
		
		//四连通情况
		//if(255 == (int)mat.at<uchar>(pt.x - 1, pt.y))
		//{
		//	points.push_back(Point(pt.x - 1, pt.y));
		//	mat.at<uchar>(pt.x - 1, pt.y)=label;
		//}
		// if(255 == (int)mat.at<uchar>(pt.x, pt.y - 1))
		// {
		//	points.push_back(Point(pt.x, pt.y - 1));
		//	mat.at<uchar>(pt.x, pt.y - 1)=label;
		// }
		// if(255 == (int)mat.at<uchar>(pt.x + 1, pt.y))
		// {
		//	points.push_back(Point(pt.x + 1, pt.y));
		//	mat.at<uchar>(pt.x + 1, pt.y)=label;
		// }
		// if(255 == (int)mat.at<uchar>(pt.x, pt.y + 1))
		// {
		//	 points.push_back(Point(pt.x, pt.y + 1));
		//	 mat.at<uchar>(pt.x, pt.y + 1)=label;
		// }

		////补充四个斜角，八连通情况
		// if(255 == (int)mat.at<uchar>(pt.x - 1, pt.y - 1))
		// {
		//	points.push_back(Point(pt.x - 1, pt.y - 1));
		//	mat.at<uchar>(pt.x - 1, pt.y - 1)=label;
		// }
		// if(255 == (int)mat.at<uchar>(pt.x + 1, pt.y - 1))
		// {
		//	 points.push_back(Point(pt.x + 1, pt.y - 1));
		//	 mat.at<uchar>(pt.x + 1, pt.y - 1)=label;
		// }
		// if(255 == (int)mat.at<uchar>(pt.x + 1, pt.y + 1))
		// {
		//	points.push_back(Point(pt.x + 1, pt.y + 1));
		//	mat.at<uchar>(pt.x + 1, pt.y + 1)=label;
		// }
		// if(255 == (int)mat.at<uchar>(pt.x - 1, pt.y + 1))
		// {
		//	points.push_back(Point(pt.x - 1, pt.y + 1));
		//	mat.at<uchar>(pt.x - 1, pt.y + 1)=label;
		// }

		//mat.at<uchar>(pt.x,pt.y) = 0;
		points.pop_front();
		
		//cout << "ptIndex: " << pt << "ptValue: " << (int)mat.at<uchar>(1,591) << endl;
		
	}
	//imshow("mat",mat);
	//waitKey();
	
	return mat;
}

vector<Point> getLocate(Mat mat)
{
	vector<int> labelVec;
	labelVec.push_back(0);
	for(int i=0;i<mat.size().height;i++)
	{
		uchar* src = mat.ptr<uchar>(i);
		for(int j=0;j<mat.size().width;j++)
		{
			if(src[j] != 0 && (int)src[j] < labelVec.size())
				{//cout << (int)src[j]<<endl;
					labelVec[src[j]]++;}
			else if(src[j] != 0)
			{//cout <<(int)src[j]<<endl;
				labelVec.push_back(1);}
		}
	}
	int maxCount=0,secondMaxCount=0,maxLabel=0,secondMaxLabel=0;
	for(int i=1; i <labelVec.size(); i++)
	{
		if(labelVec[i] > maxCount && labelVec[i] > secondMaxCount)
		{
			secondMaxCount = maxCount;
			secondMaxLabel = maxLabel;
			maxCount = labelVec[i];
			maxLabel = i;
		}
		else if(labelVec[i] > secondMaxCount)
		{
			secondMaxCount = labelVec[i];
			secondMaxLabel = i;
		}
	}
	
	Point maxIndex(0,0), secondMaxIndex(0,0);
	for(int i=0;i<mat.size().height;i++)
	{
		uchar* src = mat.ptr<uchar>(i);
		for(int j=0;j<mat.size().width;j++)
		{
			if(maxLabel == src[j] )
			{
				maxIndex.x += i;
				maxIndex.y += j;
				src[j] = 255;
			}
			else if(secondMaxLabel == src[j])
			{
				secondMaxIndex.x += i;
				secondMaxIndex.y += j;
				src[j] = 255;
			}
			else
				src[j] = 0;
		}
	}
	maxIndex.x = maxIndex.x / maxCount;
	maxIndex.y = maxIndex.y / maxCount;
	secondMaxIndex.x = secondMaxIndex.x / secondMaxCount;
	secondMaxIndex.y = secondMaxIndex.y / secondMaxCount;

	vector<Point> locationVec;
	locationVec.push_back(Point(maxIndex.y, maxIndex.x));
	locationVec.push_back(Point(secondMaxIndex.y, secondMaxIndex.x));
	
	mat.at<uchar>(locationVec[0]) = 0; 
	mat.at<uchar>(locationVec[1]) = 0; 

	return locationVec;
}

vector<string> getLocationText(vector<Point> location)
{
	vector<string> locationText;
	for(int i=0; i<location.size(); i++)
	{
		
		double disX = (768-(int)(location[i].y))/19.8 + 6.2+ 9.5;
		double disY = (int)(location[i].x)/19.8 - 28.8;

		
		unsigned int xPulse = disX / 3 * 10000;
		unsigned int yPulse = disY / 3 * 10000;
		
		string str;
		stringstream tempStream;
		
		tempStream << "X" << i << ": " << (int)(location[i].y) << "(" << xPulse <<  "pulse) "  <<  "Y" << i << ": " << (int)(location[i].x) << "(" << yPulse << "pulse)";
		
		/*str += "X" + tempStream.str() + ": ";

		tempStream.str("");
		tempStream << (int)(location[i].y);
		str += tempStream.str();
		
		tempStream.str("");
		tempStream << (unsigned int)xPulse;
		str += "(" + tempStream.str() + "pulse) "; 

		tempStream.str("");
		tempStream << i;
		str += "Y" + tempStream.str() + ":  ";

		tempStream.str("");
		tempStream << (int)(location[i].x);
		str += tempStream.str();

		tempStream.str("");
		tempStream << (unsigned int)yPulse;
		str += "(" + tempStream.str() + "pulse) "; */
		str = tempStream.str();
		locationText.push_back(str);
	}
	return locationText;
}