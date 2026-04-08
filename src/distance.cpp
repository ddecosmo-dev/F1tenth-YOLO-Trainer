#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main() {
    const cv::Size patternSize(6, 8);
    const cv::Size imageSize(960, 540);
    const float totalWidthMm = 250.0f;
    const float squareSize = totalWidthMm / (patternSize.width + 1.0f);

    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;

    std::vector<cv::Point3f> idealGrid;
    for (int row = 0; row < patternSize.height; ++row) {
        for (int col = 0; col < patternSize.width; ++col) {
            idealGrid.emplace_back(col * squareSize, row * squareSize, 0.0f);
        }
    }

    std::vector<cv::String> imageFiles;
    cv::glob("/home/devin_ml/work/f1_lab08/IMGs/calibration/*", imageFiles, false);

    if (imageFiles.empty()) {
        std::cout << "No calibration images found in IMGs/calibration.\n";
        return 0;
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
        return 0;
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

    return 0;
}