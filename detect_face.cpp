#include <opencv2/opencv.hpp>

static const cv::String model = "res10_300x300_ssd_iter_140000_fp16.caffemodel";
static const cv::String config = "deploy.prototxt";

void detect_dnn_face()
{
    cv::VideoCapture capture(-1, cv::CAP_V4L2);
    if (!capture.isOpened()) {
        std::cerr << "camera not opened" << std::endl;
        return;
    }

    cv::dnn::Net net = cv::dnn::readNet(model, config);
    if (net.empty()) {
        std::cerr << "Net open failed" << std::endl;
        return;
    }

    cv::Mat frame;
    cv::namedWindow("FRAME");

    while (true) {
        capture >> frame;
        cv::Mat blob = cv::dnn::blobFromImage(frame, 1, cv::Size(300, 300), cv::Scalar(104, 177, 123)); // skin colour for Asians
        net.setInput(blob);     // 입력
        cv::Mat result = net.forward();

        cv::Mat detect(result.size[2], result.size[3], CV_32FC1, result.ptr<float>());  // result.size[2], [3] for top left and bottom right axis of face for rectangle

        for (int i = 0; i < detect.rows; ++i) {
            float CONFIDENCE = detect.at<float>(i, 2);  // column 2 = accuracy
            if (CONFIDENCE < 0.7) break;
            int x1 = cvRound(detect.at<float>(i, 3) * frame.cols);
            int y1 = cvRound(detect.at<float>(i, 4) * frame.rows);
            int x2 = cvRound(detect.at<float>(i, 5) * frame.cols);
            int y2 = cvRound(detect.at<float>(i, 6) * frame.rows);

            cv::rectangle(frame, cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)), cv::Scalar(0, 255, 0));

            cv::String label = cv::format("Face : %5.3f", CONFIDENCE);
            cv::putText(frame, label, cv::Point(x1, y1 - 1), cv::FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(0, 255, 0));
        }

        cv::imshow("FRAME", frame);
        if (cv::waitKey(24) == 27) break;
    }
    
    cv::destroyAllWindows();

    return;
}
