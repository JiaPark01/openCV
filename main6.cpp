#include <opencv2/opencv.hpp>

void arithmetic()
{
    cv::Mat src1 = cv::imread("lenna256.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat src2 = cv::imread("square.bmp", cv::IMREAD_GRAYSCALE);

    if (src1.empty() or src2.empty()) return;

    cv::imshow("SRC1", src1);
    cv::imshow("SRC2", src2);

    cv::Mat dst1;
    cv::add(src1, src2, dst1);
    cv::imshow("ADD", dst1);

    cv::Mat dst2;
    cv::addWeighted(src1, 0.5, src2, 0.5, 0, dst2); // weight = 0.5. both : find avg
    cv::imshow("ADDWEIGHTEd", dst2);

    cv::Mat dst3;
    cv::subtract(src1, src2, dst3);
    cv::imshow("SUBTRACT", dst3);

    cv::Mat dst4;
    cv::absdiff(src1, src2, dst4);  // order doesn't matter in absolute diff
    cv::imshow("ABSDIFF", dst4);

    cv::waitKey();
    cv::destroyAllWindows();
}

void logical()
{
    cv::Mat src1 = cv::imread("lenna256.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat src2 = cv::imread("square.bmp", cv::IMREAD_GRAYSCALE);
    if (src1.empty() or src2.empty()) return;

    std::cout<< "1" << std::endl;

    cv::imshow("SRC1", src1);
    cv::imshow("SRC2", src2);

    cv::Mat dst1;
    cv::bitwise_and(src1, src2, dst1);
    std::cout << "1" << std::endl;
    cv::imshow("AND", dst1);

    cv::Mat dst2;
    cv::bitwise_or(src1, src2, dst2);
    std::cout << "1" << std::endl;
    cv::imshow("OR", dst2);

    cv::Mat dst3;
    cv::bitwise_xor(src1, src2, dst3);
    std::cout << "1" << std::endl;
    cv::imshow("XOR", dst3);

    cv::Mat dst4;
    cv::bitwise_not(src1, dst4);
    std::cout << "1" << std::endl;
    cv::imshow("NOT", dst4);
    
    cv::waitKey();
    cv::destroyAllWindows();
}

void filter_embossing()
{
    cv::Mat src = cv::imread("rose.bmp", cv::IMREAD_GRAYSCALE);
    if (src.empty()) {
        std::cerr << "file not found" << std::endl;
        return;
    }

    float filter_data[] = {-1.0f, -1.0f, 0.f, -1.0f, 0.f, 1.0f, 0.f, 1.0f, 1.0f};
    cv::Mat emboss(3, 3, CV_32FC1, filter_data);    // 3x3 matrix filter

    cv::Mat dst;
    cv::filter2D(src, dst, -1, emboss, cv::Point(-1, 1), 128/* brighten by adding 128 */);
    //          src, dst, depth of dst, filtering kernel, anchor axis, dleta to add afterwards, borderType
    
    cv::imshow("SRC", src);
    cv::imshow("DST", dst);

    cv::waitKey();
    cv::destroyAllWindows();
}

void blurring_mean()
{
    cv::Mat src = cv::imread("rose.bmp", cv::IMREAD_GRAYSCALE);
    if (src.empty()) {
        std::cerr << "file not loaded" << std::endl;
        return;
    }

    cv::Mat dst;
    for (int ksize = 3; ksize <= 7; ksize += 2) {   // 3x3 matrix, 5x5, 7x7
        cv::blur(src, dst, cv::Size(ksize, ksize));
        cv::String desc = cv::format("Mean : %d x %d", ksize, ksize);
        cv::putText(dst, desc, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255), 1, cv::LINE_AA);
        cv::imshow("DST", dst);
        cv::waitKey();
    }

    cv::destroyAllWindows();
}

void blurring_gaussian()
{
    cv::Mat src = cv::imread("rose.bmp", cv::IMREAD_GRAYSCALE);
    if (src.empty()) return;
    cv::imshow("SRC", src);

    cv::Mat dst;
    for (int sigma = 1; sigma <= 5; ++sigma) {
        cv::GaussianBlur(src, dst, cv::Size(), static_cast<double>(sigma)); // sigmaX, std div in x axis
        cv::String text = cv::format("Sigma = %d", sigma);
        cv::putText(dst, text, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255), 1, cv::LINE_AA);
        cv::imshow("DST", dst);
        cv::waitKey();
    }

    cv::destroyAllWindows();
}

void unsharp_mask()
{
    cv::Mat src = cv::imread("rose.bmp", cv::IMREAD_GRAYSCALE);
    if (src.empty()) return;
    cv::imshow("SRC", src);

    cv::Mat blurred;
    float alpha = 1.0f;

    for (int sigma = 1; sigma <= 5; ++sigma) {
        cv::GaussianBlur(src, blurred, cv::Size(), sigma);
        cv::Mat dst = (1+alpha)*src - (alpha*blurred);
        cv::String desc = cv::format("sigma : %d", sigma);
        cv::putText(dst, desc, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255), 1, cv::LINE_AA);
        imshow("DST", dst);
        cv::waitKey();
    }

    cv::destroyAllWindows();
}

void noise_gaussian()
{
    cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
    if (src.empty()) return;

    cv::imshow("Lenna", src);

    cv::Mat dst;
    for (int stddev = 10; stddev <= 30; stddev += 10) {
        cv::Mat noise(src.size(), CV_32SC1);
        cv::randn(noise, 0, stddev);
        cv::add(src, noise, dst, cv::Mat(), CV_8UC1);
        cv::String desc = cv::format("stddev = %d", stddev);
        cv::putText(dst, desc, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255), 1, cv::LINE_AA);

        cv::imshow(desc, dst);
        cv::waitKey();
    }

    cv::destroyAllWindows();
}

void filter_bilateral()
{
    cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
    if (src.empty()) return;

    cv::Mat dst1;   // Gaussian filter
    cv::Mat dst2;   // Bilateral filter

    cv::Mat noise(src.size(), CV_32SC1);
    cv::randn(noise, 0, 5);
    cv::add(src, noise, dst2, cv::noArray(), CV_8UC1);  // src+noise . cv::noArray() == cv::Mat()

    cv::GaussianBlur(src, dst1, cv::Size(), 5);
    cv::bilateralFilter(src, dst2, -1, 1, 5);

    cv::imshow("Noise SRC", src);
    cv::imshow("Gaussian", dst1);
    cv::imshow("Bilateral", dst2);

    cv::waitKey();
    cv::destroyAllWindows();
}

void filter_median()
{
    cv::Mat src = cv::imread("lenna.bmp", cv::IMREAD_GRAYSCALE);
    int num = (int)(src.total() * 0.1);
    
    for (int i = 0; i < num; ++i) {
        int x = rand() % src.cols;
        int y = rand() % src.rows;
        src.at<uchar>(y,x) = (i%2) * 255;
    }

    cv::Mat dst1;
    cv::GaussianBlur(src, dst1, cv::Size(), 1);

    cv::Mat dst2;
    cv::medianBlur(src, dst2, 3);   // 3x3

    cv::imshow("Salt & Pepper", src);
    cv::imshow("Gaussian", dst1);
    cv::imshow("Median", dst2);

    cv::waitKey();
    cv::destroyAllWindows();
}

void affine_transform()
{
    cv::Mat src = cv::imread("tekapo.bmp");
    if (src.empty()) return;

    cv::Point2f srcPts[3];  //float point
    cv::Point2f dstPts[3];

    srcPts[0] = cv::Point2f(0.f, 0.f);
    srcPts[1] = cv::Point2f(src.cols - 1.0f, 0.0f);
    srcPts[2] = cv::Point2f(src.cols - 1.0f, src.rows - 1.0f);

    dstPts[0] = cv::Point2f(50.f, 50.f);
    dstPts[1] = cv::Point2f(src.cols - 100.0f, 100.0f);
    dstPts[2] = cv::Point2f(src.cols - 50.0f, src.rows - 50.0f);

    cv::Mat M = cv::getAffineTransform(srcPts, dstPts);

    cv::Mat dst;
    cv::warpAffine(src, dst, M, cv::Size());
    cv::imshow("SRC", src);
    cv::imshow("DST", dst);
    
    cv::waitKey();
    cv::destroyAllWindows();
}

void affine_translation()
{
    cv::Mat src = cv::imread("tekapo.bmp");
    cv::Mat M = cv::Mat_<float>({2, 3}, {1.0, 0.0, 50.0, 0.0, 1.0, 150.0});
    
    cv::Mat dst;
    cv::warpAffine(src, dst, M, cv::Size());
    
    cv::imshow("SRC", src);
    cv::imshow("DST", dst);

    cv::waitKey();
    cv::destroyAllWindows();
}
