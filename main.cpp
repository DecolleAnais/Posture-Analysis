#include <iostream>
#include <opencv2/opencv.hpp>

#include "BackgroundExtractor.h"
#include "BlobExtractor.h"
#include "Skeleton.h"

// display booleans
bool skeleton_display = true;
bool boxes_display = true;
bool contours_display = false;

char interract(cv::Mat &frame){
    // Display keys
    cv::putText(frame, "Pause/Play : P    Skeleton : S    Bounding Boxes : B", cv::Point(frame.cols/2 , 15), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255,255,255), 1);
    cv::putText(frame, "Contours : C    Quit : Q", cv::Point(frame.cols/2, 30), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255,255,255), 1);

    char key = cv::waitKey(30);

    // Pause
    bool pause = false;
    if(key == 'p' || key == 'P')
        pause = true;
    while(pause){
        key = cv::waitKey(30);
        if(key == 'p' || key == 'P')
            pause = false;
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
    while(key != 'Q' && key != 'q') {
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
            key = interract(frame);
            imshow("Result", frame);
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
            key = interract(frame);
            imshow("Result", frame);
            continue;
        }
        // Get the OBB of the shirt
        cv::RotatedRect rect = cv::minAreaRect(redContours[0]);


        /***** Skin Blob analysis *****/
        // Generate skeleton
        Skeleton sk(skinContours, rect.center);

        if(sk.getLeftArm().getEnd().y < sk.getHead().getEnd().y &&
                sk.getRightArm().getEnd().y < sk.getHead().getEnd().y)
        {
            cv::putText(frame, "Action : STOP", cv::Point(10,30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255), 2);
        }
        else if(sk.getLeftArm().getEnd().x > sk.getCenter().x + 1.0 * (float)rect.boundingRect().width)
        {
            cv::putText(frame, "Action : SWIPE LEFT", cv::Point(10,30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255), 2);
        }
        else if(sk.getRightArm().getEnd().x < sk.getCenter().x - 1.0 * (float)rect.boundingRect().width)
        {
            cv::putText(frame, "Action : SWIPE RIGHT", cv::Point(10,30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255), 2);
        }
        else
        {
            cv::putText(frame, "Action : NONE", cv::Point(10,30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255), 2);
        }


        /***** Interactions *****/
        key = interract(frame);

        /***** Image drawing *****/
        // contours display
        if(contours_display) {
            // draw the skin contours
            cv::drawContours(frame, skinContours, -1, cv::Scalar(255,0,0), 1, 8, skinHierarchy);
            // draw the red contours
            cv::drawContours(frame, redContours, -1, cv::Scalar(0,0,255), 1, 8, redHierarchy);
        }

        // bounding boxes display
        if(boxes_display) {
            // Draw the AABB of the shirt
            cv::rectangle(frame, rect.boundingRect(), cv::Scalar(0,0,255), 1, 8, 0);
            // draw the center of the contour
            cv::circle(frame, rect.center, 5, cv::Scalar(0,255,0), -1, 8, 0);
        }

        // Draw skeleton
        sk.draw(frame, skeleton_display, boxes_display);
        // Display picture
        imshow("Result", frame);


        /***** Frame Cleanup *****/
        foreground = cv::Mat();
    }

    /***** Cleanup ******/
    cap.release();
    return 0;
}