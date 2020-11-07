//
//  after_net.hpp
//  SegBox
//
//  Created by 周锦凡 on 2020/11/2.
//  Copyright © 2020 zjf. All rights reserved.
//

#ifndef after_net_hpp
#define after_net_hpp

#include <stdio.h>
#include <opencv2/core/types_c.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

#endif /* after_net_hpp */

using namespace std;

cv::Mat to_bin(const cv::Mat img, string type);

class MyImage
{
    
    friend cv::Mat to_threshold(cv::Mat img);
    
public:
    MyImage() = default;
    MyImage(const cv::Mat image): raw_image(image), height(image.rows), width(image.cols) {}; // init function
    
    void load_backgourd(const cv::Mat bg);
    void set_roi(const cv::Mat removed_bg);
    //vector<cv::Mat> get_edge_n_sal(const cv::Mat image);
    //cv::Mat get_seg();
    cv::Mat raw_image;
    cv::Mat background;
    cv::Mat roi;


private:
    cv::Rect change_size(cv::Rect rect, float ratio);
    int height;
    int width;
    int actual_height;
    //cv::Mat raw_image;
    //cv::Mat roi;
    cv::Mat sal;
    cv::Mat sal_gray;
    cv::Mat sal_bin;
    cv::Mat edge;
    cv::Mat edge_gray;
    cv::Mat edge_bin;
    cv::Mat seg;
};

