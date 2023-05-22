#include <opencv2/opencv.hpp>

void template_matching()
{
    cv::Mat img = cv::imread("circuit.bmp", cv::IMREAD_COLOR);
    cv::Mat template_img = cv::imread("crystal.bmp", cv::IMREAD_COLOR);

    if (img.empty() or template_img.empty()) return;

    cv::Mat resource;
    cv::Mat resolution_norm;
    cv::matchTemplate(img, template_img, resource, cv::TM_CCOEFF_NORMED);
    cv::normalize(resource, resolution_norm, 0.0, 255.0, cv::NORM_MINMAX, CV_8UC1);

    double max_value = 0.0;
    cv::Point max_location;
    cv::minMaxLoc(resource, nullptr, static_cast<double*>(&max_value), nullptr, static_cast<cv::Point*>(&max_location));

    std::cout << "Max value : " << max_value << std::endl;

    cv::rectangle(img, cv::Rect(max_location.x, max_location.y, template_img.cols, template_img.rows), cv::Scalar(0, 0, 255), 2);

    cv::imshow("Template", template_img);
    cv::imshow("resolution_norm", resolution_norm);
    cv::imshow("IMG", img);

    cv::waitKey();
    cv::destroyAllWindows();
}

void detect_face()
{
    cv::Mat src = cv::imread("lenna.bmp");

    if (src.empty()) {
        std::cerr << "img load failed" << std::endl;
        return;
    }

    cv::CascadeClassifier classifier("haarcascade_frontalface_default.xml");
    //classifier.load("haarcascade_frontalface_default.xml");
    if (classifier.empty()) {
        std::cerr << "XML load failed" << std::endl;
        return;
    }

    std::vector<cv::Rect> faces;
    classifier.detectMultiScale(src, faces);

    for (auto&& face: faces) {
        cv::rectangle(src, face, cv::Scalar(255, 0, 255), 2);
    }

    cv::imshow("SRC", src);
    
    cv::waitKey();
    cv::destroyAllWindows();
}

void detect_walkers()
{
    cv::VideoCapture capture("vtest.avi");

    if (!capture.isOpened()) {
        std::cerr << "vid open failed" << std::endl;
        return;
    }

    cv::HOGDescriptor hog;
    hog.setSVMDetector(hog.getDefaultPeopleDetector());

    cv::Mat frame;
    while (true) {
        capture >> frame;
        if (frame.empty()) break;
        std::vector<cv::Rect> detected;

        hog.detectMultiScale(frame, detected);

        for (auto&& rc : detected) {
            cv::Scalar color = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
            cv::rectangle(frame, rc, color, 3);
        }

        cv::imshow("FRAME", frame);
        
        if (cv::waitKey(10) == 27) break;
    }

    cv::destroyAllWindows();
}
