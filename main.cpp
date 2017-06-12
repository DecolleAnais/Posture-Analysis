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
        cv::bitwise_and(frame, frame, foreground, mask_foreground);


        // skin-colored blobs extractor
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        unsigned int nbBlobs = blobExtractor.getSkinBlobs(foreground, contours, hierarchy);
        if(nbBlobs == 3) {
            // Get OBB of each contours
            cv::RotatedRect leftArmOBB = cv::minAreaRect(contours[0]);
            cv::RotatedRect headOBB = cv::minAreaRect(contours[1]);
            cv::RotatedRect rightArmOBB = cv::minAreaRect(contours[2]);

            // Get four vertex of OBB
            cv::Point2f leftArmPoints[4];
            leftArmOBB.points(leftArmPoints);
            // Get direction points of OBB
            cv::Point2f leftArmStart = cv::Point2f(leftArmOBB.center.x + cos(leftArmOBB.angle) * (-leftArmOBB.size.height/2.0),
                                                   leftArmOBB.center.y + sin(leftArmOBB.angle) * (-leftArmOBB.size.height/2.0));
            cv::Point2f leftArmEnd = cv::Point2f(leftArmOBB.center.x + cos(leftArmOBB.angle) * (leftArmOBB.size.height/2.0),
                                                 leftArmOBB.center.y + sin(leftArmOBB.angle) * (leftArmOBB.size.height/2.0));

            cv::Point2f headPoints[4];
            headOBB.points(headPoints);
            cv::Point2f rightArmPoints[4];
            rightArmOBB.points(rightArmPoints);

            cv::arrowedLine(frame, leftArmStart, leftArmEnd, (0,0,255), 1, 8 );
            for( int j = 0; j < 4; j++ )
                cv::line(frame, leftArmPoints[j], leftArmPoints[(j+1)%4], (0,0,255), 1, 8 );
            for( int j = 0; j < 4; j++ )
                cv::line(frame, headPoints[j], headPoints[(j+1)%4], (0,0,255), 1, 8 );
            for( int j = 0; j < 4; j++ )
                cv::line(frame, rightArmPoints[j], rightArmPoints[(j+1)%4], (0,0,255), 1, 8 );



            cv::drawContours(frame, contours, -1, cv::Scalar(255,0,0), 1, 8, hierarchy);
        }

        nbBlobs = blobExtractor.getRedBlobs(foreground, contours, hierarchy);
        if(nbBlobs > 0) {
            cv::drawContours(frame, contours, -1, cv::Scalar(0,0,255), 1, 8, hierarchy);
        }



        imshow("Result", frame);

        foreground = cv::Mat();

        key = cv::waitKey(30);
    }

    cap.release();
    return 0;
}