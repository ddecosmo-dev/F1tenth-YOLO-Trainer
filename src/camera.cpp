#include "lab_08.h"

class CameraRead{
    private:
        cv::VideoCapture cap{};

    public: 
        CameraRead(int camera_index) : cap(camera_index){
            //set desired camera params
            cap.set(cv::CAP_PROP_FRAME_WIDTH, 960);
            cap.set(cv::CAP_PROP_FRAME_HEIGHT, 450);
            cap.set(cv::CAP_PROP_FPS, 60);
        
            // Check if the webcam was opened successfully
            if (!cap.isOpened()) {
                std::cout << "Error: Could not access the webcam." << std::endl;
            } else {
                std::cout << "Webcam accessed successfully!" << std::endl;
            }
        }

        ~CameraRead{
            if (cap.isOpened()) {
            cap.release();
            std::cout << "Camera released safely." << std::endl;
            }
        }

        bool GetFrame(cv::Mat &frame) {

            //retrieve new frame when requested
            bool ret = cap.read(frame);
            
            //if no frame is captured return with error
            if (!ret) {
                std::cout << "Error retrieving frame" <<std::endl;
                return ret;
            }

            return ret;
        }
};
