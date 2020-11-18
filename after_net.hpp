//
//  after_net.hpp
//  SegBox
//
//  Created by 周锦凡 on 2020/11/2.
//  Copyright © 2020 zjf. All rights reserved.
//

#ifndef after_net_hpp
#define after_net_hpp

#include "opencv2/opencv.hpp"
#include <vector>

#endif /* after_net_hpp */

using namespace std;

cv::Mat to_bin(cv::Mat img, string type);

class MyImage
{
    
    //friend cv::Mat to_threshold(cv::Mat img);
    friend vector<int> find_cord(MyImage img);

public:
    MyImage() = default;
    MyImage(const cv::Mat image): raw_image(image), height(image.rows), width(image.cols) {}; // init function
    static cv::Mat background;

    static void set_bg(cv::Mat& bg);

    void set_roi(const cv::Mat removed_bg);
    void set_sal();
    cv::Mat remove_light();
    cv::Mat raw_image;
    cv::Mat roi;
    int actual_height;
    int height;
    int width;
    cv::Mat sal_bin;



private:
    cv::Rect change_size(cv::Rect rect, float ratio);

    int start_x, start_y;

    //cv::Mat raw_image;
    //cv::Mat roi;
    cv::Mat sal;
    cv::Mat sal_gray;

    cv::Mat edge;
    cv::Mat edge_gray;
    cv::Mat edge_bin;
    cv::Mat seg;
};

vector<int> find_cord(MyImage img);
