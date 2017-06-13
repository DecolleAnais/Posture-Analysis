//
// Created by protoke on 13/06/17.
//

#ifndef POSTURE_ANALYSIS_SKELETON_H
#define POSTURE_ANALYSIS_SKELETON_H

#include "opencv2/opencv.hpp"

/**
 * Class representing a member in a skeleton.
 */
class Member{
public:

    /**
     * Default constructor, does nothing special.
     */
    Member();

    /**
     * Constructor of Member. Generate description points of member from OBB and the center of the skeleton.
     * @param OBB Oriented Bounding Box of the member.
     * @param center Center (origin) of the skeleton of the member.
     */
    Member(cv::RotatedRect OBB, cv::Point2f center);


    /***** Getters *****/
    cv::Point2f getStart();
    cv::Point2f getEnd();
    cv::RotatedRect getOBB();

private:

    /**
     * Oriented Bounding Box of the member.
     */
    cv::RotatedRect OBB_;

    /**
     * Start point of the member : the nearest point to the center of the skeleton in the single-line representation of
     * the member.
     */
    cv::Point2f start_;
    /**
     * End point of the member : the farest point to the center of the skeleton in the single-line representation of
     * the member.
     */
    cv::Point2f end_;
};

/**
 * Class representing a skeleton composed in members and a origin.
 */
class Skeleton {
public:

    /**
     * Constructor of Skeleton. Generate a member for each skin blob (contour extracted from skin segmentation).
     * @param skinBlob List of blobs representing skin areas. There must be 3 blobs : the left arm, the right arm and
     * the head
     * @param center Origin of the skeleton. In general, it is the center of the torso, of the basis of the neck.
     */
    Skeleton(std::vector<std::vector<cv::Point>> skinBlob, cv::Point center);

    /**
     * Drawing function for the skeleton.
     * @param frame Picture on which the skeleton will be drawn.
     */
    void draw(cv::Mat frame);

    cv::Point getCenter();
    Member getLeftArm();
    Member getHead();
    Member getRightArm();

private:

    /**
     * Origin of the skeleton.
     */
    cv::Point center_;

    Member leftArm_;
    Member head_;
    Member rightArm_;
};


#endif //POSTURE_ANALYSIS_SKELETON_H
