#include <opencv2/opencv.hpp>  // ver 4 but just use 2 for header

int main()
{
    std::cout << "Hello OpenCV " << CV_VERSION << std::endl;
    
    //class cv::Mat image {cv::imread("lenna.bmp")};      // {} uniform initialiser. only once when init
    class cv::Mat image;                             // default constructor
    image = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
    if(image.empty()) {
        std::cout << "file not found" << std::endl;
        return 1;
    }
    cv::namedWindow("LENNA");

    cv::imshow("LENNA", image);     // a thread. must pause after
    cv::waitKey(0);                 // wait for n sec. if 0, only after any key is pressed

    cv::destroyWindow("LENNA");
    // cv::destroyAllWindows();
    
    return 0;
}
