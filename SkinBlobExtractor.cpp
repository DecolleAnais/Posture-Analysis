//
// Created by ad on 24/05/17.
//

#include "SkinBlobExtractor.h"


void setSkinMask(cv::Mat foreground, cv::Mat mask_color) {
    // create a mask to filter the skin color
    cv::Mat hsv_foreground;
    cv::cvtColor(foreground, hsv_foreground, cv::COLOR_BGR2HSV);
    // skin color mask in HSV
    cv::Scalar low_color_hsv = cv::Scalar(0,30,60,0);
    cv::Scalar high_color_hsv = cv::Scalar(20,150,255,0);

    // keep (white) the pixels which are between the 2 hsv value
    inRange(hsv_foreground, low_color_hsv, high_color_hsv, mask_color);

    // increase the quality with erode (decrease the noise) and dilate (fill the holes)
    erode(mask_color, mask_color, cv::Mat::ones(2,2,CV_32F), cv::Point(-1,-1), 2, 1, 1);
    dilate(mask_color, mask_color, cv::Mat::ones(3,3,CV_32F), cv::Point(-1,-1), 5, 1, 1);
}

bool isClose(std::vector< cv::Point > contour1, std::vector< cv::Point > contour2) {
    for(unsigned int id = 0;id < contour1.size();id++) {

    }
}