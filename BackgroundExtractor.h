//
// Created by protoke on 26/04/17.
//

#include "opencv2/opencv.hpp"
#include "opencv2/bgsegm.hpp"


#ifndef POSTURE_ANALYSIS_BACKGROUNDEXTRACTOR_H
#define POSTURE_ANALYSIS_BACKGROUNDEXTRACTOR_H


class BackgroundExtractor {
public:
    BackgroundExtractor();
    BackgroundExtractor(const std::string &reference);

    void setBGReference(const std::string &reference);
    void learnFromBGReference();
    void learnFromBGReference(const std::string &reference);
    void getMask(const cv::InputArray &source, cv::OutputArray &mask);

    cv::Ptr<cv::BackgroundSubtractor> mog;
    std::string bGReference;
};


#endif //POSTURE_ANALYSIS_BACKGROUNDEXTRACTOR_H
