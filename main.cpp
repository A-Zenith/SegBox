//
//  main.cpp
//  SegBox
//
//  Created by 周锦凡 on 2020/11/2.
//  Copyright © 2020 zjf. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "opencv2/opencv.hpp"

#include "camera.hpp"
#include "after_net.hpp"
#include "calib.h"
#include "cal_height.h"
#include "use_montage.h"

using namespace std;

int main(int argc, const char * argv[])
{
    //cv::Mat bg;
    float calib_height = calib_proc();

    cv::Mat photo;
    cout << "Please take a photo of the object" << endl;
    photo = take_photo();
    if(photo.cols == 0)
        throw "Failed to take a photo of the object";
    cout << "Successfully took the photo of the object" << endl;
    MyImage image(photo);

//    cv::imshow("object", image.raw_image);
//    cv::waitKey(0);

//    cout << "Now remove the object and take a photo of the background" << endl;
//    photo = take_photo();
//    if(photo.cols == 0)
//        throw "Failed to take a photo of the background";
//    cout << "Successfully took the photo of the background" << endl;

    cv::Mat removed_bg = image.remove_light();
//    cv::imshow("remove_light", removed_bg);
//    cv::waitKey(0);

//    cv::cvtColor(removed_bg, removed_bg, cv::COLOR_BGR2GRAY);
//    cv::threshold(removed_bg, removed_bg, 1, 255, cv::THRESH_BINARY);
//
//    cout << removed_bg.dims;
//
//    cv::imshow("remove_light", removed_bg);
//    cv::waitKey(0);
    cout << "happy?>>>>" << endl;
    image.set_roi(removed_bg);
//    cv::imshow("roi", image.roi);
//    cv::waitKey(0);
    image.set_sal();
    vector<int> ys = find_cord(image);  // some bug here
    int y1 = ys[0];
    int y2 = ys[1];
    image.actual_height = cal_1(y1, y2, calib_height);
    cout << "the height of the object is:" << image.actual_height << endl;

    cv::Mat lala = image.sal_bin / 255;
    //lala = 1- lala;
    cv::Mat black(image.roi.rows, image.roi.cols, CV_8UC1);
    for(int i=0; i<image.roi.rows; i++)
        for(int j=0; j<image.roi.cols; j++)
            black.at<uchar>(i,j) = 0;

    cv::Mat tmp = image.roi.clone();
    tmp.copyTo(black, lala);
    cv::Mat m_result = use_montage(image);
    //cv::imshow("result", black);
//    cv::imshow("m_result", m_result);
//    cv::waitKey(0);

    cv::imwrite("/home/wy/CLionProjects/untitled/seg_box(copy)/seg_result.png", m_result);
    ofstream out("/home/wy/CLionProjects/untitled/seg_box(copy)/height.txt");
    out << to_string(image.actual_height);

}
