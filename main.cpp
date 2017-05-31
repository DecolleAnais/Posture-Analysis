#include <iostream>
#include <opencv2/opencv.hpp>

#include "BackgroundExtractor.h"

int main() {
    cv::Mat frame, result, foreground, mask_foreground;
    char key;
    BackgroundExtractor bgext("Analyse Référence.avi");

    std::cout << "Analysing" << std::endl;

    cv::VideoCapture cap("Analyse analyse.avi");


    while(key != 'Q' && key != 'q') {
        bool readed = cap.read(frame);
        if(!readed)
            break;

        // foreground extractor
        bgext.getMask(frame, mask_foreground);
        cv::bitwise_and(frame, frame, foreground, mask_foreground);

        // skin color extractor
        // create a mask to filter the skin color
        cv::Mat hsv_foreground, mask_color;
        cvtColor(foreground, hsv_foreground, cv::COLOR_BGR2HSV);
        // skin color mask in HSV
        cv::Scalar low_color_hsv = cv::Scalar(0,30,60,0);
        cv::Scalar high_color_hsv = cv::Scalar(20,150,255,0);

        inRange(hsv_foreground, low_color_hsv, high_color_hsv, mask_color);

        // increase the quality with dilate
        erode(mask_color, mask_color, cv::Mat::ones(2,2,CV_32F), cv::Point(-1,-1), 2, 1, 1);
        dilate(mask_color, mask_color, cv::Mat::ones(3,3,CV_32F), cv::Point(-1,-1), 5, 1, 1);

        // contours detection
        std::vector< std::vector< cv::Point > > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(mask_color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

        if(!contours.empty()) {
            // fusion of close contours
            // TODO https://dsp.stackexchange.com/questions/2564/opencv-c-connect-nearby-contours-based-on-distance-between-them
            /*unsigned int id_contour = 0;
            float min_area = 0;
            for(std::vector< cv::Point > contour : contours) {
                // calculation of the area of the detected blob
                cv::Moments mu = moments(contours[id_contour]);
                float area = mu.m00;
                if(area > min_area) {

                }
                id_contour++;
            }*/



            cv::drawContours(frame, contours, -1, cv::Scalar(255,0,0), 1, 8, hierarchy);
        }





        imshow("Result", frame);

        foreground = cv::Mat();

        key = cv::waitKey(30);
    }

    cap.release();
    return 0;
}