//
// Created by protoke on 26/04/17.
//

#include "opencv2/opencv.hpp"
#include "opencv2/bgsegm.hpp"


#ifndef POSTURE_ANALYSIS_BACKGROUNDEXTRACTOR_H
#define POSTURE_ANALYSIS_BACKGROUNDEXTRACTOR_H


class BackgroundExtractor {
public:

    /**
     * Default constructor of BackgroundExtractor. Generate a MOG background substractor.
     */
    BackgroundExtractor();

    /**
     * Constructor of BackgroundExtractor. Generate a MOG background substractor and initialise its learning with the
     * given video.
      * @param reference Relative path to the background learning video.
      */
    BackgroundExtractor(const std::string &reference);

    /**
     * Setter of the relative path to the background learning video.
     * @param reference Relative path to the background learning video.
     */
    void setBGReference(const std::string &reference);

    /**
     * Launch the learning of the background extractor.
     */
    void learnFromBGReference();

    /**
     * Extract the mask describing the foreground from a frame, considering what has been learned before.
     * @param source Frame from which we extract the mask.
     * @param mask Binary picture describing the area considered as foreground.
     */
    void getMask(const cv::InputArray &source, cv::OutputArray &mask);

private:
    cv::Ptr<cv::BackgroundSubtractor> mog;
    std::string bGReference;
};


#endif //POSTURE_ANALYSIS_BACKGROUNDEXTRACTOR_H
