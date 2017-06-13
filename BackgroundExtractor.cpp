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

void BackgroundExtractor::learnFromBGReference(){
    std::cout << "Learning from " << bGReference << std::endl;

    cv::VideoCapture cap(bGReference);

    bool reading;
    cv::Mat frameReference, maskTemp;
    int i = 0;

    // Read full video
    while(true) {
        reading = cap.read(frameReference);
        if (!reading) // End of learning video
            break;

        // Extract the foreground mask with auto learning
        mog->apply(frameReference, maskTemp);
    }

    cap.release();
    std::cout << "Finished learning" << std::endl;
}

void BackgroundExtractor::getMask(const cv::InputArray &source, cv::OutputArray &mask){
    // Extract the foreground mask WITHOUT learning
    mog->apply(source, mask, 0);
}


