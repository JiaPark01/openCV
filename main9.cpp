#include <opencv2/opencv.hpp>

void labeling_basic()
{
    uchar data[] = {
        0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 1, 0,
        1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 1, 1, 0,
        0, 0, 0, 1, 0, 0, 1, 0, 
        0, 0, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };

    cv::Mat src = cv::Mat(8, 8, CV_8UC1, data) * 255;

    cv::Mat labels;

    int cnt = connectedComponents(src, labels);

    std::cout << "src:\n" << src << std::endl;
    std::cout << "labels:\n" << labels << std::endl;
    std::cout << "Num of labels : " << cnt << std::endl;
}

void labeling_stats()
{
    cv::Mat src = cv::imread("keyboard.bmp", cv::IMREAD_GRAYSCALE);

    if (src.empty()) {
        std::cerr << "img load failed" << std::endl;
        return;
    }

    cv::Mat bin;
    threshold(src, bin, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::Mat labels, stats, centroids;
    int cnt = cv::connectedComponentsWithStats(bin, labels, stats, centroids);

    cv::Mat dst;
    cvtColor(src, dst, cv::COLOR_GRAY2BGR);

    for (int i = 1; i < cnt; i++) {
        int* p = stats.ptr<int>(i);
        if (p[4] < 20) continue;    // ignore pixels below 20
        cv::rectangle(dst, cv::Rect(p[0], p[1], p[2], p[3]), cv::Scalar(0, 255, 255), 2);
    }

    cv::imshow("SRC", src);
    cv::imshow("DST", dst);

    cv::waitKey();
    cv::destroyAllWindows();
}

void contours_basic()
{
    cv::Mat src = cv::imread("contours.bmp", cv::IMREAD_GRAYSCALE);

    if (src.empty()) return;

    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(src, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    cv::Mat dst;
    cv::cvtColor(src, dst, cv::COLOR_GRAY2BGR);

    for (int i = 0; i < contours.size(); ++i) {
        cv::Scalar c(rand() & 255, rand() & 255, rand() & 255);
        cv::drawContours(dst, contours, i, c, 2);
    }

    cv::imshow("SRC", src);
    cv::imshow("DST", dst);

    cv::waitKey();
    cv::destroyAllWindows();
}

void contours_hier()
{
    cv::Mat src = cv::imread("contours.bmp", cv::IMREAD_GRAYSCALE);

    if (src.empty()) return;

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(src, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat dst;
    cv::cvtColor(src, dst, cv::COLOR_GRAY2BGR);

    for (int i = 0; i >= 0; i = hierarchy[i][0]) {
        cv::Scalar c(rand() & 255, rand() & 255, rand() & 255);
        cv::drawContours(dst, contours, i, c, -1, cv::LINE_8, hierarchy);
    }

    cv::imshow("SRC", src);
    cv::imshow("DST", dst);

    cv::waitKey();
    cv::destroyAllWindows();
}

void setLabel(cv::Mat& img, const std::vector<cv::Point>&pts, const std::string& label)
{
    cv::Rect rc = cv::boundingRect(pts);
    cv::rectangle(img, rc, cv::Scalar(0, 0, 255), 1);
    cv::putText(img, label, rc.tl(), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));
}

void find_polygon()
{
    cv::Mat src = cv::imread("polygon.bmp", cv::IMREAD_COLOR);

    if (src.empty()) return;

    cv::Mat gray_scale;
    cv::cvtColor(src, gray_scale, cv::COLOR_BGR2GRAY);

    cv::Mat binary_image;
    cv::threshold(gray_scale, binary_image, 200 /*threshold*/, 255 /*max value*/, cv::THRESH_BINARY_INV /*since bg is gray*/ | cv::THRESH_OTSU);

    std::vector<std::vector<cv::Point> > contours;  // vector for outlines
    cv::findContours(binary_image, contours, cv::RETR_EXTERNAL /*mode*/, cv::CHAIN_APPROX_NONE /*method*/);

    for (auto&& pts: contours) {
        if (cv::contourArea(pts) < 400) {
            continue;   // skip areas smaller than 400 pixels
        }

        std::vector<cv::Point> approx;
        approxPolyDP(pts, approx, cv::arcLength(pts, true)*0.02, true);

        int vtc = (int)approx.size();

        if (vtc == 3) {
            setLabel(src, pts, "TRI");
        } else if (vtc == 4) {
            setLabel(src, pts, "RECT");
        } else if (vtc > 4) {
            double len = cv::arcLength(pts, true);
            double area = contourArea(pts);
            double ratio = 4.0 * CV_PI * area / (len * len);

            if (ratio > 0.8) {
                setLabel(src, pts, "CIR");
            }
        }
    }

    cv::imshow("SRC", src);

    cv::waitKey();
}

