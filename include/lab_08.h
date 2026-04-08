#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

//Camera.cpp -------------------------------------------------
class CameraRead {
private:
    cv::VideoCapture cap;

public:
    CameraRead(int camera_index);  // Just the "signature"
    ~CameraRead();
    bool GetFrame(cv::Mat &frame);
};

//distance.cpp-------------------------------------------------
//initial calibration
void onMouse(int event, int x, int y, int flags, void* userdata);
std::vector<cv::Point3f> createIdealGrid(const cv::Size& patternSize, float squareSize);
void runInitialChessboardCalibration();

//get height of camera
double getHeight(std::string matrix_path, cv::Point2f pixel);

//get distance of pixel !!!
cv::Point2f getDistance(cv::Point2f pixel, cv::Mat K, double H);

//read XML helper! can be done once 
cv::Mat readCameraMatrix(std::string matrix_path);

// Lane.cpp --------------------------------------------------

//single function, lane detector
cv::Mat laneDetector(const cv::Mat* inputImgPtr);

// Detection.cpp --------------------------------------------------
