#include <opencv2/opencv.hpp>

static cv::Ptr<cv::ml::KNearest> train_knn()
{
    cv::Mat digits = cv::imread("digits.png", cv::IMREAD_GRAYSCALE);
    if (digits.empty()) {
        std::cerr << "file load failed" << std::endl;
        return nullptr;
    }

    cv::Mat train_images;
    cv::Mat train_labels;

    for (int j = 0; j < 50; ++j) {
        for (int i = 0; i < 100; ++i) {
            cv::Mat roi = digits(cv::Rect(i * 20, j * 20, 20, 20));
            cv::Mat roi_float;
            roi.convertTo(roi_float, CV_32FC1);
            cv::Mat roi_flatten = roi_float.reshape(1, 1);

            train_images.push_back(roi_flatten);
            train_labels.push_back(j / 5);
        }
    }

    cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create(); // cv::Ptr = smart pointer, doesn't need delete
    knn->train(train_images, cv::ml::ROW_SAMPLE, train_labels);
    
    return knn;
}

static cv::Point point_Prev(-1, -1);    // when mouse button released, set axis to (-1, -1)

static void knn_on_mouse(int event, int x, int y, int flags, void* userdata)
{
    cv::Mat image = *(static_cast<cv::Mat*>(userdata));
    if (event == cv::EVENT_LBUTTONDOWN) {
        point_Prev = cv::Point(x, y);
    } else if (event == cv::EVENT_LBUTTONUP) {
        point_Prev = cv::Point(-1, -1);
    } else if (event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_LBUTTON)) {
        cv::line(image, point_Prev, cv::Point(x, y), cv::Scalar::all(255), 40, cv::LINE_AA, 0);
        point_Prev = cv::Point(x, y);
        cv::imshow("IMAGE", image);
    }
}

void do_knn_machine_learning()
{
    cv::Ptr<cv::ml::KNearest> knn = ::train_knn();
    if (knn.empty()) {
        std::cerr << "Training failed" << std::endl;
        return;
    }

    cv::Mat img(400, 400, CV_8UC1);
    cv::imshow("IMAGE", img);
    cv::setMouseCallback("IMAGE", knn_on_mouse, static_cast<void*>(&img));

    while (true) {
        int keyboard_key = cv::waitKey();
        if (keyboard_key == 27) break;
        else if (keyboard_key == ' ') {
            cv::Mat image_resize;
            cv::Mat image_float;
            cv::Mat image_flatten;
            cv::Mat resource;
            cv::resize(img, image_resize, cv::Size(20, 20), 0, 0, cv::INTER_AREA);
            image_resize.convertTo(image_float, CV_32FC1);
            image_flatten = image_float.reshape(1, 1);

            knn->findNearest(image_flatten, 5, resource);   // k = 3
            std::cout << cvRound(resource.at<float>(0, 0)) << std::endl;    // print result
            img.setTo(0);   // reset input screen
            cv::imshow("IMAGE", img);
        }
    }
    
    cv::destroyAllWindows();
}
