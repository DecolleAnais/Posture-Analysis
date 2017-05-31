//
// Created by ad on 24/05/17.
//

#ifndef POSTURE_ANALYSIS_SKINBLOBEXTRACTOR_H
#define POSTURE_ANALYSIS_SKINBLOBEXTRACTOR_H

#include "opencv2/opencv.hpp"


void setSkinMask(cv::Mat foreground, cv::Mat mask_color);
bool isClose(std::vector< cv::Point > contour1, std::vector< cv::Point > contour2);


#endif //POSTURE_ANALYSIS_SKINBLOBEXTRACTOR_H
