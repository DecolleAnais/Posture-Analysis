//
// Created by protoke on 26/04/17.
//

#include "BackgroundExtractor.h"

BackgroundExtractor::BackgroundExtractor() :
        bgsm2(cv::createBackgroundSubtractorMOG2(/*100, 64, false*/)) {}

BackgroundExtractor::BackgroundExtractor(const std::string &reference) :
        bgsm2(cv::createBackgroundSubtractorMOG2(100, 64, false)),
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
        bgsm2->apply(frameReference, maskTemp, 1);
    }

    cap.release();
    std::cout << "Learned" << std::endl;
}

void BackgroundExtractor::getMask(const cv::InputArray &source, cv::OutputArray &mask){
    bgsm2->apply(source, mask, 0);
}


