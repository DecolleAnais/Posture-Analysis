//
// Created by ad on 24/05/17.
//

#include "BlobExtractor.h"


unsigned int BlobExtractor::getSkinBlobs(cv::Mat foreground, std::vector< std::vector< cv::Point > > &contours, std::vector<cv::Vec4i> &hierarchy) {
    // create a mask to filter the skin color
    cv::Mat hsv_foreground, mask_color;
    cv::cvtColor(foreground, hsv_foreground, cv::COLOR_BGR2HSV);
    // skin color mask in HSV
    cv::Scalar low_color_hsv = cv::Scalar(0,30,60,0);
    cv::Scalar high_color_hsv = cv::Scalar(20,150,255,0);

    // keep (white) the pixels which are between the 2 hsv value
    inRange(hsv_foreground, low_color_hsv, high_color_hsv, mask_color);

    // increase the quality with erode (decrease the noise) and dilate (fill the holes)
    erode(mask_color, mask_color, cv::Mat::ones(3,3,CV_32F), cv::Point(-1,-1), 2, 1, 1);
    dilate(mask_color, mask_color, cv::Mat::ones(9,9,CV_32F), cv::Point(-1,-1), 4, 1, 1);

    // contours detection
    cv::findContours(mask_color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    if(contours.empty())
        return 0;
    return contours.size();
}


unsigned int BlobExtractor::getRedBlobs(cv::Mat foreground, std::vector< std::vector< cv::Point > > &contours, std::vector<cv::Vec4i> &hierarchy) {
    // create a mask to filter the skin color
    cv::Mat hsv_foreground, mask_color;
    cv::cvtColor(foreground, hsv_foreground, cv::COLOR_BGR2HSV);
    // red color mask in HSV
    cv::Scalar low_color_hsv = cv::Scalar(160,60,0,0);
    cv::Scalar high_color_hsv = cv::Scalar(180,255,255,0);

    // keep (white) the pixels which are between the 2 hsv value
    inRange(hsv_foreground, low_color_hsv, high_color_hsv, mask_color);

    // increase the quality with erode (decrease the noise) and dilate (fill the holes)
    //erode(mask_color, mask_color, cv::Mat::ones(3,3,CV_32F), cv::Point(-1,-1), 2, 1, 1);
    dilate(mask_color, mask_color, cv::Mat::ones(9,9,CV_32F), cv::Point(-1,-1), 2, 1, 1);

    // contours detection
    cv::findContours(mask_color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    if(contours.empty())
        return 0;
    return contours.size();
}


bool BlobExtractor::isClose(std::vector< cv::Point > contour1, std::vector< cv::Point > contour2) {
    for(unsigned int id = 0;id < contour1.size();id++) {

    }
}