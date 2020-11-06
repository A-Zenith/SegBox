//
//  after_net.cpp
//  SegBox
//
//  Created by 周锦凡 on 2020/11/2.
//  Copyright © 2020 zjf. All rights reserved.
//

#include "after_net.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <opencv2/core/types_c.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;

//cv::Mat MyImage::get_seg()
//{
//
//}

cv::Mat to_bin(const cv::Mat img, string type)
{
    cv::Mat gray, bin;
    int thr_type = type=="sal" ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV;
    double thr =  type=="sal" ? 200 : 220;
    cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, bin, thr, 255, thr_type);
    return bin;
}

void MyImage::load_backgourd(const cv::Mat bg)
{
    this->background = bg;
}
