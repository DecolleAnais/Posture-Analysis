//
// Created by ad on 24/05/17.
//

#ifndef POSTURE_ANALYSIS_BLOBEXTRACTOR_H
#define POSTURE_ANALYSIS_BLOBEXTRACTOR_H

#include "opencv2/opencv.hpp"

class BlobExtractor {
public:
    /**
     * Fill a vector of contours of skin-colored regions and their hierarchy
     * @param foreground frame to analyze
     * @param contours a vector of contours of skin-colored regions
     * @param hierarchy hierarchy of the contours
     * @return the number of contours of skin-colored regions detected
     */
    unsigned int getSkinBlobs(cv::Mat foreground, std::vector< std::vector< cv::Point > > &contours, std::vector<cv::Vec4i> &hierarchy);

    /**
     * Fill a vector of contours of red-colored regions and their hierarchy
     * @param foreground frame to analyze
     * @param contours a vector of contours of red-colored regions
     * @param hierarchy hierarchy of the contours
     * @return the number of contours of red-colored regions detected
     */
    unsigned int getRedBlobs(cv::Mat foreground, std::vector< std::vector< cv::Point > > &contours, std::vector<cv::Vec4i> &hierarchy);

    /**
     * Sort contours by the position on the x axis (left to right on the screen)
     * @param contours vector of contours to sort
     */
    void sortContoursByPositionX(std::vector< std::vector< cv::Point > > &contours);

private:
};



#endif //POSTURE_ANALYSIS_BLOBEXTRACTOR_H
