#include <iostream>
#include <opencv2/opencv.hpp>

#include "BackgroundExtractor.h"
#include "BlobExtractor.h"

int main() {
    cv::Mat frame, result, foreground, mask_foreground;
    char key;
    BackgroundExtractor bgext("Analyse Référence.avi");

    std::cout << "Analysing" << std::endl;

    cv::VideoCapture cap("Analyse analyse.avi");

    BlobExtractor blobExtractor;


    while(key != 'Q' && key != 'q') {
        bool readed = cap.read(frame);
        if(!readed)
            break;

        // foreground extractor
        bgext.getMask(frame, mask_foreground);
        // increase the quality with dilate
        dilate(mask_foreground, mask_foreground, cv::Mat::ones(5,5,CV_32F), cv::Point(-1,-1), 5, 1, 1);
        // apply the foreground extractor on the original frame
        dilate(mask_foreground, mask_foreground, cv::Mat::ones(5,5,CV_32F), cv::Point(-1,-1), 2, 1, 1);
        imshow("Mask", mask_foreground);
        cv::bitwise_and(frame, frame, foreground, mask_foreground);


        // skin-colored blobs extractor
        std::vector< std::vector< cv::Point > > contours;
        std::vector<cv::Vec4i> hierarchy;
        unsigned int nbBlobs = blobExtractor.getSkinBlobs(foreground, contours, hierarchy);

        // skin blobs
        if(nbBlobs > 0) {
            // draw contours
            cv::drawContours(frame, contours, -1, cv::Scalar(255,0,0), 1, 8, hierarchy);
            // sort contours by the position on the x axis
            blobExtractor.sortContoursByPositionX(contours);
            // draw the center for each contour
            for(unsigned int i = 0;i < nbBlobs;i++) {
                cv::RotatedRect rect = cv::minAreaRect(contours[i]);
                // draw the center
                cv::circle(frame, rect.center, 5, cv::Scalar(0,255,0), -1, 8, 0);
                // put the number
                cv::putText(frame, std::to_string(i), rect.center, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255,0,0));
            }
        }

        nbBlobs = blobExtractor.getRedBlobs(foreground, contours, hierarchy);
        // red blobs
        if(nbBlobs > 0) {
            // draw the contour
            cv::drawContours(frame, contours, -1, cv::Scalar(0,0,255), 1, 8, hierarchy);
            // draw the center of the contour
            cv::RotatedRect rect = cv::minAreaRect(contours[0]);
            cv::rectangle(frame, rect.boundingRect(), cv::Scalar(0,0,255), 1, 8, 0);
            cv::circle(frame, rect.center, 5, cv::Scalar(0,255,0), -1, 8, 0);
        }



        imshow("Result", frame);

        foreground = cv::Mat();

        key = cv::waitKey(30);
    }

    cap.release();
    return 0;
}