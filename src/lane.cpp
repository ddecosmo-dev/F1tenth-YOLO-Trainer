#include "lab_08.h"

//change to pointer
cv::Mat laneDetector(const cv::Mat* inputImgPtr) {
    if (inputImgPtr == nullptr || inputImgPtr->empty()) {
        return cv::Mat();
    }

    const cv::Mat& inputImg = *inputImgPtr;

    //get image size (should be 960 x 540)
    int pixelHeight = inputImg.rows;
    int pixelWidth = inputImg.cols;

    //focus on lower 50% of image
    cv::Rect roi(0, pixelHeight / 2, pixelWidth, pixelHeight / 2 );
    cv::Mat roiImg = inputImg(roi);

    cv::Mat hsvImg, hsvMask;

    //Create an HSV image play with the HBV values
    cv::cvtColor(roiImg, hsvImg, cv::COLOR_BGR2HSV);

    //define ranges of yellow!
    cv::Scalar lowerYellow(15, 50, 90);
    cv::Scalar upperYellow(35, 255, 255);
    cv::inRange(hsvImg, lowerYellow, upperYellow, hsvMask);

    //clean up map!
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(hsvMask, hsvMask, cv::MORPH_OPEN, kernel); //remove noise
    cv::morphologyEx(hsvMask, hsvMask, cv::MORPH_CLOSE, kernel); //remove gaps

    //apply to image 
    cv::Mat result;
    cv::bitwise_and(roiImg, roiImg, result, hsvMask);

    //find the edges using find contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(hsvMask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    //apply draw contours
    // Use a copy of the ROI as our canvas so we don't mess up the original
    cv::Mat finalOutput = inputImg.clone();

    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        
        //only draw contours of certain pixel size, tuned to avoid noise occuring
        if (area > 20) { 
            cv::drawContours(finalOutput, contours, (int)i, cv::Scalar(0, 255, 0), 3, 
                            cv::LINE_8, cv::noArray(), 0, cv::Point(0, pixelHeight / 2));
        }
    }

    //return image
    return finalOutput;
}


// int main() {
//     //get clean and final image paths
//     const std::string lanePath = "/home/devin_ml/work/f1_lab08/IMGs/resource/lane.png";
//     const std::string laneSamplePath = "/home/devin_ml/work/f1_lab08/IMGs/imgs/lane_sample.png";

//     //load both images
//     const cv::Mat inputImg = cv::imread(lanePath, cv::IMREAD_COLOR);
//     const cv::Mat compareImg = cv::imread(laneSamplePath, cv::IMREAD_COLOR);

//     if (inputImg.empty() || compareImg.empty()) {
//         std::cerr << "Error: Check image paths." << std::endl;
//         return -1;
//     }

//     //CALL THE MAIN FUNCTION
//         //do all the intermedaite stuff

//     //display the final images 
//     cv::Mat result = laneDetector(&inputImg);

//     //save the final image, for testing!
//     cv::imwrite("test.png",result);

//     return 0;
// }