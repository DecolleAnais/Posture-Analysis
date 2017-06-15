#include <iostream>
#include <opencv2/opencv.hpp>

#include "BackgroundExtractor.h"
#include "BlobExtractor.h"
#include "Skeleton.h"

// display booleans
bool skeleton_display = true;
bool boxes_display = true;
bool contours_display = false;
bool pause = false;
bool run = true;

char interract(){
    char key = cv::waitKey(30);

    // Pause
    if(key == 'p' || key == 'P'){
        pause ? pause = false : pause = true;
    }

    // skeleton display
    if(key == 'S' || key == 's') {
        skeleton_display ? skeleton_display = false : skeleton_display = true;
    }
    // bounding boxes display
    if(key == 'B' || key == 'b') {
        boxes_display ? boxes_display = false : boxes_display = true;
    }
    // contours display
    if(key == 'C' || key == 'c') {
        contours_display ? contours_display = false : contours_display = true;
    }

    // exit
    if(key == 'Q' || key == 'q'){
        run = false;
    }

    return key;
}

int main() {
    cv::Mat frame, result, foreground, mask_foreground;
    char key;
    BlobExtractor blobExtractor;
    BackgroundExtractor bgext("Analyse Référence.avi");

    std::cout << "Analysing" << std::endl;

    cv::VideoCapture cap("Analyse analyse.avi");

    // Main loop
    while(run) {
        bool readed = cap.read(frame);
        if(!readed)
            break;


        /***** Foreground extraction *****/
        // Foreground extractor
        bgext.getMask(frame, mask_foreground);
        // Increase the quality with dilate
        dilate(mask_foreground, mask_foreground, cv::Mat::ones(5,5,CV_32F), cv::Point(-1,-1), 5, 1, 1);
        // Apply the foreground extractor on the original frame
        cv::bitwise_and(frame, frame, foreground, mask_foreground);


        /***** Blob extraction *****/
        // Skin-colored blobs extractor
        std::vector<std::vector<cv::Point>> skinContours;
        std::vector<cv::Vec4i> skinHierarchy;
        unsigned int nbSkinBlobs = blobExtractor.getSkinBlobs(foreground, skinContours, skinHierarchy);
        if(nbSkinBlobs != 3) { // We don't detect two arms and the head. The person is not facing the camera.
            /***** Frame Cleanup *****/
            foreground = cv::Mat();
            /***** Interactions *****/
            do {
                cv::Mat result; frame.copyTo(result);

                key = interract();

                /***** Image drawing *****/
                // Display keys
                cv::putText(result, "Pause/Play : P    Skeleton : S    Bounding Boxes : B", cv::Point(frame.cols/2 , 15), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255,255,255), 1);
                cv::putText(result, "Contours : C    Quit : Q", cv::Point(frame.cols/2, 30), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255,255,255), 1);

                // Display picture
                imshow("Result", result);
            }while(pause && run);
            continue;
        }
        // sort contours by the position on the x axis
        blobExtractor.sortContoursByPositionX(skinContours);


        // Red blobs extractor
        std::vector<std::vector<cv::Point>> redContours;
        std::vector<cv::Vec4i> redHierarchy;
        unsigned int nbRedBlobs = blobExtractor.getRedBlobs(foreground, redContours, redHierarchy);
        if(nbRedBlobs == 0) { // We don't detect the shirt. There is no one is the camera's field.
            /***** Frame Cleanup *****/
            foreground = cv::Mat();
            /***** Interactions *****/
            do {
                cv::Mat result; frame.copyTo(result);

                key = interract();

                /***** Image drawing *****/
                // Display keys
                cv::putText(result, "Pause/Play : P    Skeleton : S    Bounding Boxes : B", cv::Point(frame.cols/2 , 15), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255,255,255), 1);
                cv::putText(result, "Contours : C    Quit : Q", cv::Point(frame.cols/2, 30), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255,255,255), 1);

                // Display picture
                imshow("Result", result);
            }while(pause && run);
            continue;
        }
        // Get the OBB of the shirt
        cv::RotatedRect rect = cv::minAreaRect(redContours[0]);


        /***** Skin Blob analysis *****/
        // Generate skeleton
        Skeleton sk(skinContours, rect.center);

        cv::String action;
        if(sk.getLeftArm().getEnd().y < sk.getHead().getEnd().y &&
                sk.getRightArm().getEnd().y < sk.getHead().getEnd().y)
        {
            action = "Action : STOP/PLAY";
        }
        else if(sk.getLeftArm().getEnd().x > sk.getCenter().x + 1.0 * (float)rect.boundingRect().width)
        {
            action = "Action : SWIPE LEFT";
        }
        else if(sk.getRightArm().getEnd().x < sk.getCenter().x - 1.0 * (float)rect.boundingRect().width)
        {
            action = "Action : SWIPE RIGHT";
        }
        else
        {
            action = "Action : NONE";
        }


        /***** Interactions *****/
        do {
            cv::Mat result; frame.copyTo(result);

            key = interract();

            /***** Image drawing *****/
            // action display
            cv::putText(result, action, cv::Point(10,30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255), 2);

            // Display keys
            cv::putText(result, "Pause/Play : P    Skeleton : S    Bounding Boxes : B", cv::Point(frame.cols/2 , 15), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255,255,255), 1);
            cv::putText(result, "Contours : C    Quit : Q", cv::Point(frame.cols/2, 30), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255,255,255), 1);

            // contours display
            if (contours_display) {
                // draw the skin contours
                cv::drawContours(result, skinContours, -1, cv::Scalar(255, 0, 0), 1, 8, skinHierarchy);
                // draw the red contours
                cv::drawContours(result, redContours, -1, cv::Scalar(0, 0, 255), 1, 8, redHierarchy);
            }

            // bounding boxes display
            if (boxes_display) {
                // Draw the AABB of the shirt
                cv::rectangle(result, rect.boundingRect(), cv::Scalar(0, 0, 255), 1, 8, 0);
                // draw the center of the contour
                cv::circle(result, rect.center, 5, cv::Scalar(0, 255, 0), -1, 8, 0);
            }

            // Draw skeleton
            sk.draw(result, skeleton_display, boxes_display);

            // Display picture
            imshow("Result", result);
        }while(pause && run);


        /***** Frame Cleanup *****/
        foreground = cv::Mat();
    }

    /***** Cleanup ******/
    cap.release();
    return 0;
}