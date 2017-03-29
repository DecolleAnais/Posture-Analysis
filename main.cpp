#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    VideoCapture cap(0);
    Mat frame;
    char key;
    while(key != 'Q' && key != 'q') {
        cap.read(frame);
        imshow("Posture Analysis", frame);
        key = waitKey(30);
    }
    return 0;
}