//
// Created by protoke on 26/04/17.
//

#include "BackgroundExtractor.h"

BackgroundExtractor::BackgroundExtractor() :
        mog(cv::bgsegm::createBackgroundSubtractorMOG()) {}

BackgroundExtractor::BackgroundExtractor(const std::string &reference) :
        mog(cv::bgsegm::createBackgroundSubtractorMOG()),
        bGReference(reference) {
    learnFromBGReference();
}

void BackgroundExtractor::setBGReference(const std::string &reference) {
    bGReference = reference;
}

void BackgroundExtractor::learnFromBGReference(const std::string &reference){
    setBGReference(reference);
    learnFromBGReference();
}

void BackgroundExtractor::learnFromBGReference(){
    std::cout << "Loading " << bGReference << std::endl;
    cv::VideoCapture cap(bGReference);
    bool reading;
    cv::Mat frameReference, maskTemp;
    int i = 0;

    // Read single image from video
    /*reading = cap.read(frame);
    if(!reading)
        bgsm2->apply(frame, mask, -1);*/

    // Read full video
    while(true) {
        reading = cap.read(frameReference);
        if (!reading)
            break;
        mog->apply(frameReference, maskTemp);
    }

    cap.release();
    std::cout << "Learned" << std::endl;
}

void BackgroundExtractor::getMask(const cv::InputArray &source, cv::OutputArray &mask){
    mog->apply(source, mask, 0);
}


