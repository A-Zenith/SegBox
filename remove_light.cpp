//
//  remove_light.cpp
//  SegBox
//
//  Created by 周锦凡 on 2020/11/5.
//  Copyright © 2020 zjf. All rights reserved.
//

#include "remove_light.hpp"

#include <stdio.h>
#include <opencv2/core/types_c.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

cv::Mat remove_light(cv::Mat img, cv::Mat bg)
{
    cv::Mat img_0, bg_0;
    cv::medianBlur(img, img_0, 7);
    cv::medianBlur(bg, bg_0, 7);
    cv::Mat aux;
    aux = bg_0 - img_0  ;
    return aux;
}
