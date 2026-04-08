#include lab_08.h

//check the logic to make sure we can use it lol
int main() {
    //height of camera
    double cameraHeight = 133.408; //mm consider moving to get distance method

    //initialize overall variables 
    int cameraIndex = 2; //camera number, 2 is standrd
    cv::Mat frame{}; //frame at each timestep

    //initialize camera
    cam = CameraRead(camera_index);

    //Read XML for camera parameters
    std::string xmlPath = "/home/devin_ml/work/f1_lab08/config/cam_properties.xml";
    cv::Mat cameraMatrix = readCameraMatrix(xmlPath);

    //shouldnt be necessary?
    // //initialize parameters for camera
    // double fy = K.at<double>(1, 1);
    // double cy = K.at<double>(1, 2);

    //initialize YOLO model
    std::string yoloPath = "";
    //Load using TensorRT

    while(true) {
        //get an image frame 
        cam.GetFrame(frame);

        //run lane detector
        cv::Mat edgeFrame = laneDetector(frame);

        //run YOLO model
        //what should it return?

        //or just the central lower pixel (probably this guy!)
        cv::Point2f obstaclePixel = obstaclePoint{}; //add function call

        //get distance! (mm)
        cv::Point2f distance = getDistance(obstaclePixel, cameraMatrix, cameraHeight);

        //print out distance for user
        double xDistance = distance[0];
        double yDistance = distance[1];

        cout:: << "Distance from Car is x: " << xDistance << ", y: " << yDistance <std::endl; 
    }
}
