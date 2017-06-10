//
// Created by ad on 24/05/17.
//

#include "BlobExtractor.h"


unsigned int BlobExtractor::getSkinBlobs(cv::Mat foreground, std::vector< std::vector< cv::Point > > &contours, std::vector<cv::Vec4i> &hierarchy) {
    // create a mask to filter the skin color
    cv::Mat hsv_foreground, mask_color, low_mask_color, high_mask_color;
    cv::cvtColor(foreground, hsv_foreground, cv::COLOR_BGR2HSV);
    // skin color mask in HSV
    // first mask : low red in hsv
    cv::Scalar low_color_hsv = cv::Scalar(160,30,60,0);
    cv::Scalar middle_low_color_hsv = cv::Scalar(180,150,255,0);
    // second mask : upper red in hsv
    cv::Scalar middle_high_color_hsv = cv::Scalar(0,30,60,0);
    cv::Scalar high_color_hsv = cv::Scalar(20,150,255,0);

    // keep (white) the pixels which are between the 2 hsv value
    inRange(hsv_foreground, low_color_hsv, middle_low_color_hsv, low_mask_color); // first mask : lower
    inRange(hsv_foreground, middle_high_color_hsv, high_color_hsv, high_mask_color); // second mask : upper
    bitwise_or(low_mask_color, high_mask_color, mask_color); // merge of the two masks
    cv::imshow("Mask color", mask_color);

    // increase the quality with erode (decrease the noise) and dilate (fill the holes)
    erode(mask_color, mask_color, cv::Mat::ones(9, 9,CV_32F), cv::Point(-1,-1), 1, 1, 1);
    dilate(mask_color, mask_color, cv::Mat::ones(11,11,CV_32F), cv::Point(-1,-1), 2, 1, 1);

    cv::imshow("Mask color opened", mask_color);

    // contours detection
    cv::findContours(mask_color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    mask_color = cv::Mat();

    if(contours.empty())
        return 0;
    return contours.size();
}

bool BlobExtractor::isClose(std::vector< cv::Point > contour1, std::vector< cv::Point > contour2) {
    for(unsigned int id = 0;id < contour1.size();id++) {

    }
}