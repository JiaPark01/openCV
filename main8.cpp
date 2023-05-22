#include <opencv2/opencv.hpp>

void on_threshold(int pos, void* userdata)
{
    cv::Mat src = *(cv::Mat*)userdata;

    cv::Mat dst;
    cv::threshold(src, dst, pos, 255.0, cv::THRESH_BINARY);

    cv::imshow("DST", dst);
}

void using_threshold()
{
    cv::Mat src = cv::imread("neutrophils.png", cv::IMREAD_GRAYSCALE);

    if (src.empty()) {
        std::cerr << "img load fail" << std::endl;
        return;
    }

    cv::imshow("SRC", src);

    cv::namedWindow("DST");
    cv::createTrackbar("Threshold", "DST", 0, 255, on_threshold, (void*)&src);
    cv::setTrackbarPos("Threshold", "DST", 128);

    cv::waitKey();
    cv::destroyAllWindows();

    return;
}

static void on_trackbar(int pos, void* userdata)
{
    cv::Mat src = *(static_cast<cv::Mat*>(userdata));

    int block_size = pos;
    if (pos % 2 == 0) --block_size;
    if (block_size < 3) block_size = 3;

    cv::Mat dst;
    cv::adaptiveThreshold(src, dst, 255.0, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, block_size, 5.0);

    cv::imshow("DST", dst);
}

void adaptive_threshold()
{
    cv::Mat src = cv::imread("sudoku.jpg", cv::IMREAD_GRAYSCALE);

    if (src.empty()) return;

    cv::imshow("SRC", src);

    cv::namedWindow("DST");
    cv::createTrackbar("BlockSize", "DST", nullptr, 200, on_trackbar, (void*)&src);
    cv::setTrackbarPos("BlockSize", "DST", 11);
    
    cv::waitKey();
    cv::destroyAllWindows();
}

void erode_dilate()
{
    cv::Mat src = imread("milkdrop.bmp", cv::IMREAD_GRAYSCALE);

    if (src.empty()) {
        std::cerr << "img load fail" << std::endl;
        return;
    }

    cv::Mat bin;
    cv::threshold(src, bin, 0.0, 255.0, cv::THRESH_BINARY | cv::THRESH_OTSU);   // use 0.0 since leaving up to OTSU

    cv::Mat dst1, dst2;
    cv::erode(bin, dst1, cv::Mat());
    cv::dilate(bin, dst2, cv::Mat());

    cv::imshow("SRC", src);
    cv::imshow("BIN", bin);
    cv::imshow("ERODE", dst1);
    cv::imshow("DILATE", dst2);

    cv::waitKey();
    cv::destroyAllWindows();
}
