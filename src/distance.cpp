#include "lab_08.h"

void onMouse(int event, int x, int y, int flags, void* userdata) {
        if (event == cv::EVENT_LBUTTONDOWN) {
            // Cast the userdata back to a Point2f pointer and save the values
            cv::Point2f* ptr = (cv::Point2f*)userdata;
            ptr->x = (float)x;
            ptr->y = (float)y;
            std::cout << "Click registered at: " << x << ", " << y << std::endl;
            std::cout << "Press any key to continue..." << std::endl;
        }
    }

double getHeight(std::string matrix_path, cv::Point2f pixel) {
    double distance = 400.0; // Known distance in mm
    
    // 1. Read the XML
    cv::FileStorage fs(matrix_path, cv::FileStorage::READ);
    cv::Mat K;
    fs["K"] >> K;
    fs.release();

    if (K.empty()) {
        std::cerr << "Could not load camera matrix!" << std::endl;
        return -1.0;
    }

    // 2. Extract cy and fy from the Matrix
    // K = [fx 0 cx]
    //     [0 fy cy]
    //     [0  0  1]
    double fy = K.at<double>(1, 1);
    double cy = K.at<double>(1, 2);

    // 3. The Math
    // H = d * tan(theta)
    double height = distance * (pixel.y - cy) / fy;

    return height;
}

cv::Point2f getDistance(cv::Point2f pixel, cv::Mat K, double H) {
    //get matrix params
    double fx = K.at<double>(0, 0);
    double fy = K.at<double>(1, 1);
    double cx = K.at<double>(0, 2);
    double cy = K.at<double>(1, 2);

    //x distance 
    double x = (H * fy) / (pixel.y - cy);

    //y distance
    double y = (x * (pixel.x - cx)) / fx;

    return cv::Point2f(x, y);


}

std::vector<cv::Point3f> createIdealGrid(const cv::Size& patternSize, float squareSize) {
    std::vector<cv::Point3f> idealGrid;
    for (int row = 0; row < patternSize.height; ++row) {
        for (int col = 0; col < patternSize.width; ++col) {
            idealGrid.emplace_back(col * squareSize, row * squareSize, 0.0f);
        }
    }
    return idealGrid;
}

void runInitialChessboardCalibration() {
    const cv::Size patternSize(6, 8);
    const cv::Size imageSize(960, 540);
    const float totalWidthMm = 250.0f;
    const float squareSize = totalWidthMm / (patternSize.width + 1.0f);

    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;
    std::vector<cv::Point3f> idealGrid = createIdealGrid(patternSize, squareSize);

    std::vector<cv::String> imageFiles;
    cv::glob("/home/devin_ml/work/f1_lab08/IMGs/calibration/*", imageFiles, false);

    if (imageFiles.empty()) {
        std::cout << "No calibration images found in IMGs/calibration.\n";
        return;
    }

    cv::Size calibrationImageSize = imageSize;
    for (const auto& fileName : imageFiles) {
        cv::Mat image = cv::imread(fileName);
        if (image.empty()) {
            continue;
        }

        calibrationImageSize = image.size();

        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Point2f> corners;
        const bool found = cv::findChessboardCorners(
            gray,
            patternSize,
            corners,
            cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK
        );

        if (!found) {
            continue;
        }

        cv::cornerSubPix(
            gray,
            corners,
            cv::Size(11, 11),
            cv::Size(-1, -1),
            cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1)
        );

        imagePoints.push_back(corners);
        objectPoints.push_back(idealGrid);
    }

    if (imagePoints.empty()) {
        std::cout << "No chessboard corners were detected.\n";
        return;
    }

    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;

    const double rms = cv::calibrateCamera(
        objectPoints,
        imagePoints,
        calibrationImageSize,
        cameraMatrix,
        distCoeffs,
        rvecs,
        tvecs
    );

    std::cout << "RMS Error: " << rms << '\n';
    std::cout << "Camera Matrix:\n" << cameraMatrix << '\n';
    std::cout << "Distortion Coeffs:\n" << distCoeffs << '\n';

    cv::FileStorage fs("cam_properties.xml", cv::FileStorage::WRITE);
    fs << "K" << cameraMatrix;
    fs << "D" << distCoeffs;
    fs.release();
}

cv::Mat readCameraMatrix(std::string matrix_path) {
    cv::FileStorage fs(matrix_path, cv::FileStorage::READ);
    cv::Mat K;
    fs["K"] >> K;
    fs.release();
    return K;
}

// int main() {
//     // initial chessboard calibration now lives in runInitialChessboardCalibration()

//     //calculate mounting height

//         // Simple mouse callback to print coordinates
//     // This must match the OpenCV callback signature exactly
    
//     std::string image_path = "/home/devin_ml/work/f1_lab08/IMGs/resource/cone_unknown.png";
//     cv::Mat img = cv::imread(image_path);

//     if (img.empty()) {
//         std::cerr << "Error: Could not find image at " << image_path << std::endl;
//         return -1;
//     }

//     cv::Point2f clicked_pixel(-1.0f, -1.0f);
    
//     cv::namedWindow("Select Cone Base");
//     cv::imshow("Select Cone Base", img);
//     cv::setMouseCallback("Select Cone Base", onMouse, &clicked_pixel); 
    
//     cv::waitKey(0); 

//     if (clicked_pixel.y > 0) {
//         std::string matrix_path = "/home/devin_ml/work/f1_lab08/config/cam_properties.xml";
        
//         // 2. DO NOT call getHeight here anymore! 
//         // Use the height you found during your 40cm test.
//         double h_calibrated = 133.408; // <--- REPLACE THIS with your actual result from the 40cm test

//         cv::Mat K = readCameraMatrix(matrix_path);

//         // 3. Now getDistance will return the ACTUAL distance based on the fixed height
//         cv::Point2f dist = getDistance(clicked_pixel, K, h_calibrated);

//         std::cout << "\n--- RESULTS FOR UNKNOWN CONE ---" << std::endl;
//         std::cout << "Using Calibrated Height (H): " << h_calibrated << " mm" << std::endl;
//         std::cout << "Forward Distance (x): " << dist.x << " mm" << std::endl;
//         std::cout << "Lateral Offset (y): " << dist.y << " mm" << std::endl;
//     }

//     //std::vector<

//     return 0;
// }


