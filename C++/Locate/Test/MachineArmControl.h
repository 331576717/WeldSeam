////////*****************author:wilson*********************************
////////*****************email:wilson_91@foxmail.com*******************
////////*****************date:2014-07-24*******************************
#ifndef _MOVE_MACHINE_ARM
#define _MOVE_MACHINE_ARM
#include <iostream>
#include <cassert>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <vector>
#include "MotionPath.h"
#include "SendData.h"
using namespace std;

void RotateCenter(cv::Point3i &p, const double theta);

//接受焊缝坐标信息，生成机械臂移动轨迹坐标
void MoveMachineArm(cv::Point point, const double theta, const double weld_line_width, int MotionFlag, char* buffer);

#endif