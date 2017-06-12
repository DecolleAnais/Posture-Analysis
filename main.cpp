#include <iostream>
#include <opencv2/opencv.hpp>

#include "BackgroundExtractor.h"
#include "BlobExtractor.h"

#define PI 3.14159265

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
        unsigned int nbSkinBlobs = blobExtractor.getSkinBlobs(foreground, contours, hierarchy);
        if(nbSkinBlobs == 3) {
            // sort contours by the position on the x axis
            blobExtractor.sortContoursByPositionX(contours);

            std::vector<std::vector<cv::Point>> redContours;
            std::vector<cv::Vec4i> redHierarchy;
            unsigned int nbRedBlobs = blobExtractor.getRedBlobs(foreground, redContours, redHierarchy);
            // red blobs
            if(nbRedBlobs > 0) {
                // draw the contour
                // cv::drawContours(frame, contours, -1, cv::Scalar(0,0,255), 1, 8, hierarchy);
                // draw the center of the contour
                cv::RotatedRect rect = cv::minAreaRect(redContours[0]);
                cv::rectangle(frame, rect.boundingRect(), cv::Scalar(0,0,255), 1, 8, 0);
                cv::circle(frame, rect.center, 5, cv::Scalar(0,255,0), -1, 8, 0);
            }


            for(unsigned int i = 0;i < nbSkinBlobs;i++) {
                // Get OBB of contours
                cv::RotatedRect OBB = cv::minAreaRect(contours[i]);

                // draw the center
                cv::circle(frame, OBB.center, 5, cv::Scalar(0,255,0), -1, 8, 0);
                // put the number
                //cv::putText(frame, std::to_string(i), OBB.center, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255,0,0));

                // Get four vertex of OBB
                std::vector<cv::Point2f> vertex;
                cv::Point2f vertexArray[4];
                OBB.points(vertexArray);
                for(int i = 0; i < 4; ++i){
                    vertex.push_back(vertexArray[i]);
                }

                // Get direction points of OBB
                cv::Point2f start1 = cv::Point2f(OBB.center.x + sin(-PI * OBB.angle / 180.0) * (-OBB.size.height/2.0),
                                                OBB.center.y + cos(-PI * OBB.angle / 180.0) * (-OBB.size.height/2.0));
                cv::Point2f end1 = cv::Point2f(OBB.center.x + sin(-PI * OBB.angle / 180.0) * (OBB.size.height/2.0),
                                              OBB.center.y + cos(-PI * OBB.angle / 180.0) * (OBB.size.height/2.0));
                cv::Point2f start2 = cv::Point2f(OBB.center.x + sin(-PI * (OBB.angle + 90) / 180.0) * (-OBB.size.width/2.0),
                                                OBB.center.y + cos(-PI * (OBB.angle + 90) / 180.0) * (-OBB.size.width/2.0));
                cv::Point2f end2 = cv::Point2f(OBB.center.x + sin(-PI * (OBB.angle + 90) / 180.0) * (OBB.size.width/2.0),
                                              OBB.center.y + cos(-PI * (OBB.angle + 90) / 180.0) * (OBB.size.width/2.0));

                float dist1 = sqrt(pow(end1.x - start1.x, 2) + pow(end1.y - start1.y, 2));
                float dist2 = sqrt(pow(end2.x - start2.x, 2) + pow(end2.y - start2.y, 2));

                cv::Point2f start, end;
                if(dist1 > dist2){
                    start = start1;
                    end = end1;
                }
                else{
                    start = start2;
                    end = end2;
                }

                // Draw OBB & direction
                cv::arrowedLine(frame, start, end, (255,0,0), 1, 8 );
                for( int j = 0; j < 4; j++ )
                    cv::line(frame, vertex[j], vertex[(j+1)%4], (0,0,255), 1, 8 );

            }

            //cv::drawContours(frame, contours, -1, cv::Scalar(255,0,0), 1, 8, hierarchy);
        }

        imshow("Result", frame);

        foreground = cv::Mat();

        // Interactions
        key = cv::waitKey(30);

        bool pause = false;
        if(key == 'p' || key == 'P')
            pause = true;
        while(pause){
            key = cv::waitKey(30);
            if(key == 'p' || key == 'P')
                pause = false;
        }
    }

    cap.release();
    return 0;
}