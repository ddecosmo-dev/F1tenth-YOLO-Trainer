#include <opencv2/opencv.hpp>

//Camera.cpp
class CameraRead {
private:
    cv::VideoCapture cap;

public:
    CameraRead(int camera_index);  // Just the "signature"
    ~CameraRead();
    bool GetFrame(cv::Mat &frame);
};