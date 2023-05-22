import cv2
from picamera.array import PiRGBArray
from picamera import PiCamera
import time

# init camera

'''camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))'''
cam = cv2.VideoCapture(0)

# warm up camera
time.sleep(0.1)

cv2.namedWindow("FRAME")
img_cnt = 0

try:
    '''
    # take image
    camera.capture(rawCapture, format="bgr")
    image = rawCapture.array

    # display image and wait until key pressed
    cv2.imshow("IMG", image)
    cv2.waitKey(0)
    '''
    '''
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
        # take raw NumPy array of the img and initialise timestamp & occupied/unoccupied txt
        image = frame.array

        # show frame
        cv2.imshow("FRAME", image)
        key = cv2.waitKey(1) & 0xFF

        # clear stream for next frame
        rawCapture.truncate(0)

        # if esc pressed, break
        if key == 27:
            break
        
        if key == 32:
    '''
    while True:
        ret, frame = cam.read()
        if not ret:
            print("Frame grab failed")
            break
        cv2.imshow("test", frame)

        key = cv2.waitKey(1)

        if key % 256 == 27:
            print("Good bye")
            break
        elif key % 256 == 32:
            img_name = "opencv_frame_{}.png".format(img_cnt)
            cv2.imwrite(img_name, frame)
            print(f"{img_name} saved")
            img_cnt += 1

except KeyboardInterrupt:
    pass