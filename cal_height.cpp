//
// Created by wy on 2020/11/17.
//
#include <cmath>
float cal_center_height(int v1,int v2,float H0)
{
    float h; /*h为光心距地面高度*/
    float v0 = 382.4139; /*光心坐标*/
    h = H0 * 1.0 / (1 + 1.0 * fabs(v1 - v0) / fabs(v2 - v0));
    return h;
}


float cal_1(float v1,float v2,float h)
{
    float H;/*h为光心距地面高度，H为物体的实际高度*/
    float v0 = 382.4139; /*光心坐标*/
    if(v1 <= v0)
        H = (1+1.0*fabs(v1-v0)/fabs(v2-v0))*h;
    else
        H = (1-1.0*fabs(v1-v0)/fabs(v2-v0))*h;
    return H;
}