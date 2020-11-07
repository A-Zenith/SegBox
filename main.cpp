//
//  main.cpp
//  SegBox
//
//  Created by 周锦凡 on 2020/11/2.
//  Copyright © 2020 zjf. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <opencv2/core/types_c.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "camera.hpp"
#include "after_net.hpp"
#include "remove_light.hpp"

using namespace std;

int main(int argc, const char * argv[])
{
    cv::Mat photo;
    cout << "Please take a photo of the object" << endl;
    photo = take_photo();
    if(photo.cols == 0)
        throw "Failed to take a photo of the object";
    cout << "Successfully took the photo of the object" << endl;
    MyImage image(photo);
    
//    cv::imshow("object", image.raw_image);
//    cv::waitKey(0);
    
    cout << "Now remove the object and take a photo of the background" << endl;
    photo = take_photo();
    if(photo.cols == 0)
        throw "Failed to take a photo of the background";
    cout << "Successfully took the photo of the background" << endl;
    image.load_backgourd(photo);
    
    cv::Mat removed_bg = remove_light(image.raw_image, image.background);
//    cv::imshow("remove_light", removed_bg);
//    cv::waitKey(0);
    
    cv::cvtColor(removed_bg, removed_bg, cv::COLOR_BGR2GRAY);
    cv::threshold(removed_bg, removed_bg, 1, 255, cv::THRESH_BINARY);
    
    cout << removed_bg.dims;

    cv::imshow("remove_light", removed_bg);
    cv::waitKey(0);
    
    image.set_roi(removed_bg);
    cv::imshow("roi", image.roi);
    cv::waitKey(0);
    
    
}
