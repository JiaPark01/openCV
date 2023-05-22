#include <opencv2/opencv.hpp>

void matop()
{
    cv::Mat img1;       // default empty matrix
    cv::Mat img2(480, 640, CV_8UC1);    // 640 x 480 pixel img, unsigned char, 1-channel
    cv::Mat img3(cv::Size(640, 480), CV_8UC3);

    cv::Mat img5(480, 640, CV_8UC1, cv::Scalar(128.0));     // 128 grayscale
    cv::Mat img6(480, 640, CV_8UC3, cv::Scalar(0, 0, 255)); // red
    
    cv::Mat mat1 = cv::Mat::zeros(100, 100, CV_8UC1);
    cv::Mat mat2 = cv::Mat::ones(100, 100, CV_8UC1) * 255;     // white if * 255
    cv::Mat mat3 = cv::Mat::eye(200, 200, CV_8UC1) * 255;
    
    float data[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    cv::Mat mat4(2, 3, CV_32FC1, data);

    cv::Mat mat5 = (cv::Mat_<float>(2, 3) << 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
    cv::Mat mat6 = cv::Mat_<uchar>({2, 3}, {1, 2, 3, 4, 5, 6});

    //cv::imshow("Image5", img5);
    //cv::imshow("Image6", img6);
    //cv::imshow("Mat1", mat1);
    //cv::imshow("Mat2", mat2);
    //cv::imshow("Mat3", mat3);

    cv::waitKey(0);
    cv::destroyAllWindows();
}

void matop2()
{
    cv::Mat img1 = cv::imread("dog.bmp");
    cv::Mat img2 = img1(cv::Rect(90, 30, 100, 120));    // w start, h start, w range, h range
    //cv::Mat img2 = img1;          // shallow copy
    //cv::Mat img4 = img1.clone();  // deep copy

    //img1.setTo(cv::Scalar(0, 255, 255));
    cv::imshow("img1", img1);
    cv::imshow("img2", img2);
    //cv::imshow("img4", img4);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void matop3()
{
    cv::Mat img1 {cv::imread("cat.bmp")};
    if (img1.empty()) {
        std::cout << "file not found" << std::endl;
        return;
    }

    cv::Mat img2 = img1(cv::Rect(220, 120, 340, 240));
    cv::Mat img3 = img1(cv::Rect(220, 120, 340, 240)).clone();
    img2 = ~img2;

    cv::imshow("img1", img1);
    cv::imshow("img2", img2);
    cv::imshow("img3", img3);

    cv::waitKey(0);
    cv::destroyAllWindows();

}

auto matop4() -> void
{
    cv::Mat mat1 = cv::Mat::zeros(3, 4, CV_8UC1);

    for (int j = 0; j < mat1.rows; ++j) {   // since y first in matrix
        for (int i = 0; i < mat1.cols; ++i) {
            mat1.at<uchar>(j, i)++;
        }
    }

    for (int j = 0; j < mat1.rows; ++j) {   // faster than .at but difficult to use
        uchar* p = mat1.ptr<uchar>(j);
        for (int i = 0; i < mat1.cols; ++i) {
            p[i]++;
        }
    }

    std::cout << mat1 << std::endl;

}

auto matop5() -> void
{
    // auto int a = 10;      // auto means a local var in C++
    cv::Mat img1 = cv::imread("lenna.bmp");
    if (img1.empty()) return;
    std::cout << "cols : " << img1.cols << std::endl;
    std::cout << "rows : " << img1.rows << std::endl;
    std::cout << "channel : " << img1.channels() << std::endl;

    if (img1.type() == CV_8UC1) std::cout << "Grayscale!" << std::endl;
    else if (img1.type() == CV_8UC3) std::cout << "colour!" << std::endl;

}

void matop6()
{
    float data[] = {1.0f, 1.0f, 2.0f, 3.0f};
    cv::Mat mat1(2, 2, CV_32FC1, data);
    std::cout << "mat1:\n" << mat1 << std::endl;

    cv::Mat mat2 = mat1.inv();
    std::cout << "mat2:\n" << mat2 << std::endl;

    cv::Mat mat3 = mat1.t();        // transpose - col row swap
    std::cout << "mat3:\n" << mat3 << std::endl;

    std::cout << "mat1 + 3:\n" << mat1 + 3 << std::endl;
    std::cout << "mat1 + mat2:\n" << mat1 + mat2 << std::endl;
    std::cout << "mat1 * mat2:\n" << mat1 * mat2 << std::endl; 
}
