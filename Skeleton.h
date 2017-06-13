//
// Created by protoke on 13/06/17.
//

#ifndef POSTURE_ANALYSIS_SKELETON_H
#define POSTURE_ANALYSIS_SKELETON_H

#include "opencv2/opencv.hpp"

class Member{
public:
    Member();
    Member(cv::RotatedRect OBB, cv::Point2f center);

    cv::Point2f getStart();
    cv::Point2f getEnd();
    cv::RotatedRect getOBB();

private:
    cv::RotatedRect OBB_;
    cv::Point2f start_;
    cv::Point2f end_;
};

class Skeleton {
public:
    Skeleton(std::vector<std::vector<cv::Point>> skinBlob, cv::Point center);

    void draw(cv::Mat frame);

    cv::Point getCenter();
    Member getLeftArm();
    Member getHead();
    Member getRightArm();

private:
    cv::Point center_;
    Member leftArm_;
    Member head_;
    Member rightArm_;
};


#endif //POSTURE_ANALYSIS_SKELETON_H
