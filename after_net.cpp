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

cv::Rect change_size(cv::Rect rect, float ratio);

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

void MyImage::set_roi(const cv::Mat removed_bg)
{
    int x1=this->width, x2=0, y1=this->height, y2=0;
    for(int i = 0; i < this->width; i++)
        for(int j = 0; j < this->height; j++)
            if(removed_bg.at<int>(i, j) != 0)
            {
                if(i < x1) x1 = i;
                if(i > x1) x2 = i;
                if(j < y1) y1 = j;
                if(j > y2) y2 = j;
            }
    
    int roi_width = x2 - x1;
    int roi_height = y2 - y1;
    cv::Rect rect(x1, y1, roi_width, roi_height);
    rect = change_size(rect , 1.2);
    cout << "***************************" << endl;
    cout << "x1:" << x1 << " x2:" << x2 << " width:" << width << " height" << height << endl;
    this->roi = raw_image(rect);
    
}

cv::Rect MyImage::change_size(cv::Rect rect, float ratio)
{
    int center_x = rect.x + rect.width/2;
    int center_y = rect.y + rect.height/2;
    int new_x = center_x - static_cast<int>(ratio*rect.width/2);
    int new_y = center_y - static_cast<int>(ratio*rect.height/2);
    int new_width = static_cast<int>(ratio*rect.width);
    int new_height = static_cast<int>(ratio*rect.height);
    
    if(new_x < 0)
        new_x = 0;
    if(new_y < 0)
        new_y = 0;
    if(new_width > this->width)
        new_width = this->width;
    if(new_height > this->height)
        new_height = this->height;
    
    return cv::Rect(new_x, new_y, new_width, new_height);
    
}
