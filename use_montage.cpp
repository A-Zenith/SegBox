#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>

#include "maxflow/graph.h"
#include "collage.h"
#include "data.h"
#include "after_net.hpp"


#define PATH "my_imgs_1027/demo.png"

using namespace std;
using namespace cv;

const String winName = "Image";
void static computeGradient(const Mat &I, Mat *Gx, Mat *Gy);
void gbr2bin(Mat &img);
cv::Mat input_brush(string path, int N, struct Data *D, MyImage image);

cv::Mat  computePhotomontage(Data* D) {
    Collage C(D);
    cout << "Computing photo montage...";
    cv::Mat tmp = C.computePhotomontage(DESIGNATED_SOURCE, COLORS_AND_GRADIENTS);
    cout << "Done.";
    return tmp;
}

void static onMouse(int event, int x, int y, int foo, void* p) {
    Data* D = (Data*)p;
    if (foo == EVENT_FLAG_LBUTTON + EVENT_MOUSEMOVE) {
        Point p0(x, y);
        circle(D->Draw, p0, 2, D->colors[D->selectSource], 2);
        circle(D->SourceConstraints, p0, 2, D->selectSource, 2);
        imshow(winName, D->Draw);
    }
    else if (event == EVENT_RBUTTONDOWN) {
        D->selectSource++;
        if (D->selectSource < D->sources.size()) {
            Vec2d offset = D->offsets[D->selectSource];
            D->Draw = D->sources[0].clone();
            for (int i = 0; i < D->height; i++) {
                for (int j = 0; j < D->width; j++) {
                    if (Collage::isInImage(i, j, offset[0], offset[1], D->sources[D->selectSource])) {
                        D->Draw.at<Vec3b>(i, j) = D->sources[D->selectSource].at<Vec3b>(i - offset[0], j - offset[1]);
                    }
                    if (D->SourceConstraints.at<uchar>(i,j) != 255) {
                        D->Draw.at<Vec3b>(i, j) = D->colors[D->SourceConstraints.at<uchar>(i, j)];
                    }
                }
            }
            imshow(winName, D->Draw);
        }
        else {
            setMouseCallback(winName, NULL, NULL);
            computePhotomontage(D);
            cout << "Photomontage computed." << endl;
        }
    }
}

cv::Mat use_montage(MyImage image)
{
    namedWindow(winName);
    Data D;
    int N = 2;
    D.sources = vector<Mat>(N);
    D.gradientYSources = vector<Mat>(N);
    D.gradientXSources = vector<Mat>(N);
    D.offsets = vector<Vec2d>(N);

    /*int N = 3;
    D.sources[0] = imread("../riviere.jpg");
    D.sources[1] = imread("../maison.jpg");
    D.sources[2] = imread("../cascade.jpg");
    */
    cv::Mat black(image.height, image.width, CV_8UC1);
    for(int i=0; i<black.rows; i++)
        for(int j=0; j<black.cols; j++)
            black.at<uchar>(i,j) = 0;

    D.sources[0] = image.roi;
    D.sources[1] = black;

//    cout << "YES!" << endl;
//    cout << D.sources[0] << endl;
    //D.sources[2] = imread("my_imgs/3.png");
    //D.sources[3] = imread("my_imgs/4.png");

    /* Compute gradient images for all sources */
    for (int i = 0; i < N; i ++) {
        Mat I = D.sources[i];
        int m = I.rows, n = I.cols;
        Mat Gx(m, n, CV_64FC3);
        Mat Gy(m, n, CV_64FC3);
        computeGradient(I, &Gx, &Gy);
        D.gradientXSources[i] = Gx;
        D.gradientYSources[i] = Gy;
    }

    /* Define offsets for all sources */
    for (int i = 0; i < N; i++) {
        Vec2d offset = Vec2d(0, 0);
        D.offsets[i] = offset;
    }

    /* Define result image and constraints associated */
    D.height = D.sources[0].rows;
    D.width = D.sources[0].cols;
    D.SourceConstraints = Mat(D.height, D.width, CV_8UC1);
    for (int i = 0; i < D.height; i++) {
        for (int j = 0; j < D.width; j++) {
            D.SourceConstraints.at<uchar>(i, j) = 255;
        }
    }
    D.selectSource = 0;
    D.Draw = D.sources[0].clone();

    imshow(winName, D.Draw);
    waitKey(0);
    //setMouseCallback(winName, onMouse, &D);
    cv::Mat tmp = input_brush(PATH, N, &D, image);
    imshow("img", D.sources[0]);
    waitKey(0);
//    return 0;
    return tmp;
}

void static computeGradient(const Mat &I, Mat *Gx, Mat *Gy) {
    int m = I.rows, n = I.cols;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            Vec3d gx, gy;
            if (i == 0 || i == m-1) {
                gy = Vec3d(0, 0, 0);
            } else {
                gy = ((Vec3d)I.at<Vec3b>(i + 1, j) - (Vec3d)I.at<Vec3b>(i - 1, j)) / 2;
            }

            if (j == 0 || j == n - 1) {
                gx = Vec3d(0, 0, 0);
            } else {
                gx = ((Vec3d)I.at<Vec3b>(i,j+1)- (Vec3d)I.at<Vec3b>(i,j-1)) / 2;
            }
            Gx->at<Vec3d>(i, j) = gx;
            Gy->at<Vec3d>(i, j) = gy;
        }
    }
}


cv::Mat input_brush(const string path, int N, struct Data *D, MyImage image)
{
    Mat saliency = image.sal_bin;
    imshow("saliency", saliency);
    waitKey(0);
    for(int n = 0; n < N; n++)
    {
        //D->selectSource = n;
        for(int i = 0; i < D->height; i++)
            for(int j = 0; j< D->width; j++)
                if(saliency.at<uchar>(i,j) != 0 && n==0 )//&& i%10== 0 && j%10 == 0)
                {
                    Point p0(j, i);
                    circle(D->Draw, p0, 1, D->colors[D->selectSource], 2);
                    circle(D->SourceConstraints, p0, 1, D->selectSource, 2);
                }
                else if(saliency.at<uchar>(i,j) == 0 && n==1 )//&& i%40== 0 && j%40 == 0)
                {
                    Point p0(j, i);
                    circle(D->Draw, p0, 1, D->colors[D->selectSource], 2);
                    circle(D->SourceConstraints, p0, 1, D->selectSource, 2);
                }


//        imshow(to_string(n) + "draw", D->Draw);
//        waitKey(0);

        D->selectSource++;
        Vec2d offset = D->offsets[D->selectSource];
        D->Draw = D->sources[0].clone();
        for(int i = 0; i < D->height; i++)
        {
            for (int j = 0; j < D->width; j++)
            {
                if (Collage::isInImage(i, j, offset[0], offset[1],
                                       D->sources[D->selectSource]))
                {
                    D->Draw.at<Vec3b>(i, j) = D->sources[D->selectSource].at<Vec3b>(
                            i - offset[0], j-offset[1]);
                }
                if (D->SourceConstraints.at<uchar>(i,j) != 255)
                    D->Draw.at<Vec3b>(i, j) = D->colors[D->SourceConstraints.at<uchar>(i, j)];

            }
        }

    }

    cv::Mat tmp = computePhotomontage(D);
    cout << "Photomontage computed." << endl;
    return tmp;
}

void gbr2bin(Mat &img)
{
    cvtColor(img, img, COLOR_BGR2GRAY);
    threshold(img, img, 1, 255, THRESH_BINARY);
}
