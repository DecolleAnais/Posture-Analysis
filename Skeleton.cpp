//
// Created by protoke on 13/06/17.
//

#include "Skeleton.h"

#define PI 3.14159265


/***** Member class *****/

Member::Member() {}

Member::Member(cv::RotatedRect OBB, cv::Point2f center) :
        OBB_(OBB)
{
    // Get the middle of each edge of OBB
    cv::Point2f start1 = cv::Point2f(OBB.center.x + sin(-PI * OBB.angle / 180.0) * (-OBB.size.height/2.0),
                                     OBB.center.y + cos(-PI * OBB.angle / 180.0) * (-OBB.size.height/2.0));
    cv::Point2f end1 = cv::Point2f(OBB.center.x + sin(-PI * OBB.angle / 180.0) * (OBB.size.height/2.0),
                                   OBB.center.y + cos(-PI * OBB.angle / 180.0) * (OBB.size.height/2.0));
    cv::Point2f start2 = cv::Point2f(OBB.center.x + sin(-PI * (OBB.angle + 90) / 180.0) * (-OBB.size.width/2.0),
                                     OBB.center.y + cos(-PI * (OBB.angle + 90) / 180.0) * (-OBB.size.width/2.0));
    cv::Point2f end2 = cv::Point2f(OBB.center.x + sin(-PI * (OBB.angle + 90) / 180.0) * (OBB.size.width/2.0),
                                   OBB.center.y + cos(-PI * (OBB.angle + 90) / 180.0) * (OBB.size.width/2.0));

    // Find the most distant points : they are the extremes points of the arm & head
    float dist1 = sqrt(pow(end1.x - start1.x, 2) + pow(end1.y - start1.y, 2));
    float dist2 = sqrt(pow(end2.x - start2.x, 2) + pow(end2.y - start2.y, 2));
    if(dist1 > dist2){
        start_ = start1;
        end_ = end1;
    }
    else{
        start_ = start2;
        end_ = end2;
    }

    // Check the order of these points : start is the nearest from the tee-shirt (center)
    dist1 = sqrt(pow(start_.x - center.x, 2) + pow(start_.y - center.y, 2));
    dist2 = sqrt(pow(end_.x - center.x, 2) + pow(end_.y - center.y, 2));
    if(dist1 > dist2){ // start & end are reversed, we need to flip them
        cv::Point2f temp = start_;
        start_ = end_;
        end_ = temp;
    }
}

cv::Point2f Member::getStart(){
    return start_;
}

cv::Point2f Member::getEnd(){
    return end_;
}

cv::RotatedRect Member::getOBB(){
    return OBB_;
}

/***** Skeleton class *****/

Skeleton::Skeleton(std::vector<std::vector<cv::Point>> skinBlob, cv::Point center) :
        center_(center)
{
    for(unsigned int i = 0;i < 3;i++) {
        // Get OBB of skin blob
        cv::RotatedRect OBB = cv::minAreaRect(skinBlob[i]);
        // Create the member from OBB
        Member member(OBB, center);

        switch(i){
            case 0 :
                rightArm_ = member;
                break;
            case 1 :
                head_ = member;
                break;
            case 2 :
                leftArm_ = member;
                break;
        }
    }
}

void Skeleton::draw(cv::Mat frame){
    Member member;
    for(unsigned int i = 0;i < 3;i++) {
        switch(i){
            case 0 :
                member = rightArm_;
                break;
            case 1 :
                member = head_;
                break;
            case 2 :
                member = leftArm_;
                break;
        }

        // Draw the center
        cv::circle(frame, member.getOBB().center, 5, cv::Scalar(0,255,0), -1, 8, 0);

        // Draw skeleton
        cv::line(frame, member.getStart(), member.getEnd(), (255, 0, 0), 1, 8);
        cv::line(frame, member.getStart(), center_, (255, 0, 0), 1, 8);
        cv::circle(frame, member.getStart(), 5, cv::Scalar(255, 0, 0), -1, 8, 0);
        cv::circle(frame, member.getEnd(), 5, cv::Scalar(255, 0, 0), -1, 8, 0);

        // Draw OBB
        cv::Point2f vertex[4];
        member.getOBB().points(vertex);
        for( int j = 0; j < 4; j++ )
            cv::line(frame, vertex[j], vertex[(j+1)%4], (0,0,255), 1, 8 );
    }
}

cv::Point Skeleton::getCenter(){
    return center_;
}

Member Skeleton::getLeftArm(){
    return leftArm_;
}

Member Skeleton::getHead(){
    return head_;
}

Member Skeleton::getRightArm(){
    return rightArm_;
}