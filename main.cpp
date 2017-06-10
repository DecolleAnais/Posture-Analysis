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
        cv::bitwise_and(frame, frame, foreground, mask_foreground);


        // skin-colored blobs extractor
        std::vector< std::vector< cv::Point > > contours;
        std::vector<cv::Vec4i> hierarchy;
        unsigned int nbBlobs = blobExtractor.getSkinBlobs(foreground, contours, hierarchy);

        if(nbBlobs > 0) {
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