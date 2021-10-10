//
// Created by 周锦凡 on 2020/11/8.
//

#include "calib.h"
#include <iostream>
#include "opencv2/opencv.hpp"

#include "camera.hpp"
#include "cal_height.h"
#include "after_net.hpp"

using namespace  std;

float calib_proc()
{
    float center_height;
    int height, y1, y2;
    cv::Mat bg;
    cout << "Now starting the calibration procedure." << endl;
    cout << "Please enter the actual height(mm) of the calibration object." << endl;
    cin >> height;

    cout << "Now put the calibration object in the box." << endl;
    cout << "Now take a photo of the object." << endl;
    cv::Mat calib_img = take_photo();
    MyImage calib(calib_img);
    cout << "Now take a photo of the background" << endl;
    bg = take_photo();
    MyImage::set_bg(bg);
    cv::Mat removed_bg = calib.remove_light();
    calib.set_roi(removed_bg);


//    cv::imshow("rgb", removed_bg);
//    cv::waitKey(0);

//    cv::imshow("roi", calib.roi);
//    cv::waitKey(0);

    calib.set_sal();
    vector<int> ys = find_cord(calib);  // some bug here
    y1 = ys[0];
    y2 = ys[1];
    center_height = cal_center_height(y1, y2, height);

    return center_height;

}

