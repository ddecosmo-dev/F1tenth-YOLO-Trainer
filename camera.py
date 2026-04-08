import cv2

#get the camera
class CameraRead:
    def __init__(self, camera_index = 2):
        self.cap = cv2.VideoCapture(2)
        #set desired camera params
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 960)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 450)
        self.cap.set(cv2.CAP_PROP_FPS, 60)

    #method to retrieve images in integrate
    def get_img(self):
        ret, frame = self.cap.read()

        if ret is None:
            print("CAMERA ERROR: no image retrieved")
            return False, None
        
        print("IMG FOUND")
        return ret, frame
    
    #method to release webcam at runtime close
    def release(self):
        self.cap.release()

#single testing
#comment out once going to int
if __name__ == "__main__":
    cam = CameraRead()
    ret, frame = cam.get_img()