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

    // keep (white) the pixels which are between the 2 hsv values
    inRange(hsv_foreground, low_color_hsv, middle_low_color_hsv, low_mask_color); // first mask : lower
    inRange(hsv_foreground, middle_high_color_hsv, high_color_hsv, high_mask_color); // second mask : upper
    bitwise_or(low_mask_color, high_mask_color, mask_color); // merge of the two masks

    // increase the quality with erode (decrease the noise) and dilate (fill the holes)
    erode(mask_color, mask_color, cv::Mat::ones(9, 9,CV_32F), cv::Point(-1,-1), 1, 1, 1);
    dilate(mask_color, mask_color, cv::Mat::ones(11,11,CV_32F), cv::Point(-1,-1), 2, 1, 1);

    // contours detection
    cv::findContours(mask_color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    if(contours.empty())
        return 0;
    return contours.size();
}


unsigned int BlobExtractor::getRedBlobs(cv::Mat foreground, std::vector< std::vector< cv::Point > > &contours, std::vector<cv::Vec4i> &hierarchy) {
    // create a mask to filter the red color
    cv::Mat hsv_foreground, mask_color, low_mask_color, high_mask_color;
    cv::cvtColor(foreground, hsv_foreground, cv::COLOR_BGR2HSV);
    // red color mask in HSV
    // first mask : low red in hsv
    cv::Scalar low_color_hsv = cv::Scalar(160,200,70,0);
    cv::Scalar middle_low_color_hsv = cv::Scalar(180,255,255,0);
    // second mask : upper red in hsv
    cv::Scalar middle_high_color_hsv = cv::Scalar(0,200,70,0);
    cv::Scalar high_color_hsv = cv::Scalar(20,255,255,0);

    // keep (white) the pixels which are between the 2 hsv values
    inRange(hsv_foreground, low_color_hsv, middle_low_color_hsv, low_mask_color); // first mask : lower
    inRange(hsv_foreground, middle_high_color_hsv, high_color_hsv, high_mask_color); // second mask : upper
    bitwise_or(low_mask_color, high_mask_color, mask_color); // merge of the two masks

    // increase the quality with erode (decrease the noise) and dilate (fill the holes)
    erode(mask_color, mask_color, cv::Mat::ones(3, 3,CV_32F), cv::Point(-1,-1), 1, 1, 1);
    dilate(mask_color, mask_color, cv::Mat::ones(9,9,CV_32F), cv::Point(-1,-1), 2, 1, 1);

    // contours detection
    cv::findContours(mask_color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // search the center of the red torso
    if(contours.size() > 0) {
        cv::Moments mu = moments(contours[0]);
        cv::Point center(mu.m10/mu.m00 , mu.m01/mu.m00);
        cv::rectangle(foreground, cv::Point(center.x-5, center.y-5), cv::Point(center.x+5, center.y+5), cv::Scalar(0,255,0), 1, 8, 0);
    }

    if(contours.empty())
        return 0;
    return contours.size();
}

void BlobExtractor::sortContoursByPositionX(std::vector< std::vector< cv::Point > > &contours) {
    std::sort(contours.begin(), contours.end(),
              // function of comparison (position on the x axis, left to right)
              [](const std::vector< cv::Point > contour1, const std::vector< cv::Point > contour2) {
                  cv::RotatedRect rect1 = cv::minAreaRect(contour1);
                  cv::RotatedRect rect2 = cv::minAreaRect(contour2);
                  return rect1.center.x < rect2.center.x;
              });
}
