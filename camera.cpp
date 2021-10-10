//
//  camera.cpp
//  SegBox
//
//  Created by 周锦凡 on 2020/11/5.
//  Copyright © 2020 zjf. All rights reserved.
//

#include "camera.hpp"
#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace std;

cv::Mat take_photo(void)
{
    cout << "Pressing s to save a frame" << endl;
    cv::VideoCapture capture(2);
    cv::Mat result;
    while(true)
    {
        cv::Mat frame;
        capture >> frame;
        cv::imshow("Camera", frame);
        char key = cv::waitKey(1);
        if(key == 's')
        {
            result = frame;
            cv::destroyAllWindows();
//            cv::imshow("ooo", result);
//            cv::waitKey(0);
            return result;
        }
    }
}


