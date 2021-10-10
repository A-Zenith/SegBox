  //
//  after_net.cpp
//  SegBox
//
//  Copyright © 2020 zjf. All rights reserved.
//

#include "after_net.hpp"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include "torch/torch.h"
#include "torch/script.h"

#include "dfi.h"

#define PATH "/home/zjf/CLionProjects/untitled/torch_test/cpp_model.pt"

using namespace std;

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


void MyImage::set_roi(const cv::Mat removed_bg)
{
    cv::Mat copy;
    removed_bg.copyTo(copy);
    cv::cvtColor(copy, copy, cv::COLOR_RGB2GRAY);
    cv::Mat element = getStructuringElement( 0,cv::Size(3,3), cv::Point(-1, -1));
    cv::erode(copy, copy, element, cv::Point(-1,-1), 10);
    cv::threshold(copy, copy, 1, 255, cv::THRESH_BINARY);

    unsigned short x1=this->width, x2=0, y1=this->height, y2=0;
    cout << this->width << " " << this->height << endl;
    uchar zero = 255;
    for(unsigned short i = 0; i < this->width; i++)
        for(unsigned short j = 0; j < this->height; j++)
        {
            if (copy.at<uchar>(j, i) == zero)
            {
                //printf("%u\n", copy.at<uchar>(j, i));
                if (i < x1) x1 = i;
                if (i > x2) x2 = i;
                if (j < y1) y1 = j;
                if (j > y2) y2 = j;
            }
        }


    int roi_width = x2 - x1;
    int roi_height = y2 - y1;
    cout << "***************************" << endl;
    cout << "x1:" << x1 << " y1:" << y1 << " width:" << roi_width << " height:" << roi_height << endl;
    cout << "x2:" << x2 << " y2:" << y2 << endl;
    cout << "raw_width:" << width << " raw_height:" << height << endl;
    cv::Rect rect(x1, y1, roi_width, roi_height);
    rect = change_size(rect , 1.7);

    this->roi = raw_image(rect);
    this->start_x = x1;
    this->start_y = y1;
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
    if(new_width + new_x > this->width)
        new_width = this->width -1 - new_x;
    if(new_height + new_y > this->height)
        new_height = this->height -1 - new_y;
    cout << "new_x:" << new_x << " new_y:" << new_y << " new_width:" << new_width << " new_height" <<new_height <<endl;

    
    return cv::Rect(new_x, new_y, new_width, new_height);
    
}

void MyImage::set_sal()
{

    cv::imwrite("tmp.png", this->roi);
    this->roi = cv::imread("tmp.png");
    std::vector<int64_t> sizes = {1, this->roi.rows, this->roi.cols, 3};
    torch::TensorOptions options(at::kByte);
    torch::Tensor input_tensor = torch::from_blob(this->roi.data, at::IntList(sizes), options);
    input_tensor = input_tensor.to(at::kFloat);
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(input_tensor.permute({0, 3, 1, 2}));
    at::Tensor output = inference_with_nn(inputs, PATH);


    output = output.squeeze();
    size_t width = output.size(1);
    size_t height = output.size(0);
    cv::Mat output_tmp(cv::Size(width, height) , CV_32FC1, output.data<float>());
    cv::Mat network_output(output_tmp.size(), CV_8UC1);
    float scale_factor_up = 255.0;
    output_tmp.convertTo(network_output, CV_8U, scale_factor_up);
    cv::Mat out = network_output;
    this->sal_gray = out;
    cv::threshold(out, this->sal_bin, 1, 255, cv::THRESH_BINARY);
}

vector<int> find_cord(MyImage img)
{
    int y1=img.sal_bin.rows, y2=0;

    for(unsigned short i = 0; i < img.sal_bin.cols; i++)
        for(unsigned short j = 0; j <img.sal_bin.cols; j++)
        {
            if (img.sal_bin.at<uchar>(j, i) == 255)
            {
                if(j < y1)
                    y1 = j;
                if(j > y2)
                    y2 = j;
            }
        }
    y1 += img.start_y;
    y2 += img.start_y;
    vector<int> ys = {y1, y2};
    return ys;
}

cv::Mat MyImage::remove_light()
{
    cv::Mat img_0, bg_0;

    cv::medianBlur(this->raw_image, img_0, 3);
    cv::medianBlur(background, bg_0, 3);
    cv::medianBlur(img_0, img_0, 3);
    cv::medianBlur(bg_0, bg_0, 3);


    cv::Mat aux;
    aux = img_0 - bg_0  ;
    return aux;
}

void MyImage::set_bg(cv::Mat& bg)
{
    background = bg;
}

cv::Mat MyImage::background = cv::Mat::zeros(4, 4, 0);
