#include <iostream>
#include <opencv2/opencv.hpp>

#include "BackgroundExtractor.h"

int main() {
    cv::Mat frame, result, back;
    char key;
    BackgroundExtractor bgext("Support analyse - Reference.avi");

    std::cout << "Analysing" << std::endl;

    cv::VideoCapture cap("Support analyse bras reference.avi");


    while(key != 'Q' && key != 'q') {
        bool readed = cap.read(frame);
        if(!readed)
            break;

        bgext.getMask(frame, result);

        bgext.bgsm2->getBackgroundImage(back);
        imshow("Background", back);

        imshow("Result", result);

        key = cv::waitKey(30);
    }

    cap.release();
    return 0;
}