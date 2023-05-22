#include <opencv2/opencv.hpp>
using namespace cv;

void affine_scale()
{
    Mat src = imread("rose.bmp");
    if (src.empty()) return;

    Mat dst1, dst2, dst3, dst4;
    resize(src, dst1, Size(), 4, 4, INTER_NEAREST);
    resize(src, dst2, Size(1920, 1280));    // inter-linear. indicated the desired size, doesn't need the multiples
    resize(src, dst3, Size(1920, 1280), 0, 0, INTER_CUBIC);
    resize(src, dst4, Size(1920, 1280), 0, 0, INTER_LANCZOS4);

    imshow("SRC", src);
    imshow("DST1", dst1(Rect(400, 500, 400, 400)));
    imshow("DST2", dst2(Rect(400, 500, 400, 400)));
    imshow("DST3", dst3(Rect(400, 500, 400, 400)));
    imshow("DST4", dst4(Rect(400, 500, 400, 400)));
    
    waitKey();
    destroyAllWindows();
}

void affine_rotation()
{
    Mat src = imread("tekapo.bmp");

    Point2f cp(src.cols / 2.f, src.rows / 2.f); // centre point
    Mat M = getRotationMatrix2D(cp, 20, 1);     // 20 degrees
    Mat dst;
    warpAffine(src, dst, M, Size());

    imshow("SRC", src);
    imshow("DST", dst);

    waitKey();
    destroyAllWindows();
}

void affine_flip()
{
    Mat src = imread("eastsea.bmp");
    if (src.empty()) return;
    imshow("SRC", src);

    Mat dst;
    int flip_code[] = {-1, 0, 1};    // both, vertical, horizontal

    for (int i = 0; i < 3; ++i) {
        flip(src, dst, flip_code[i]);
        String text = format("Flipcode : %d", flip_code[i]);
        putText(dst, text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0), 1, LINE_AA);
        imshow(text, dst);
    }
    
    waitKey();
    destroyAllWindows();
}

void perspective_transform()
{
    static Point2f srcQuad[4], dstQuad[4];
    static Mat src = imread("card.bmp");
    static int cnt = 0;

    if (src.empty()) {
        std::cerr << "img not found" << std::endl;
        return;
    }
    
    auto on_mouse = [](int event, int x, int y, int flags, void* userdata) {
        if (event == EVENT_LBUTTONDOWN) {
            if (cnt < 4) {
                srcQuad[cnt++] = Point2f(x, y);
                circle(src, Point(x, y), 5, Scalar(0, 0, 255), -1);
                imshow("SRC", src);

                if (cnt == 4) {                                
                    int w = 200, h = 300;

                    dstQuad[0] = Point2f(0, 0);
                    dstQuad[1] = Point2f(w - 1, 0);
                    dstQuad[2] = Point2f(0, h - 1);
                    dstQuad[3] = Point2f(w - 1, h - 1);
                
                    Mat pers = getPerspectiveTransform(srcQuad, dstQuad);   // transforming matrix

                    Mat dst;
                    warpPerspective(src, dst, pers, Size(w, h));

                    imshow("DST", dst);
                }
            }
        }
    };

    namedWindow("SRC");
    setMouseCallback("SRC", on_mouse);

    imshow("SRC", src);

    waitKey();
    destroyAllWindows();
}

void sobel_edge()
{
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    if (src.empty()) return;

    Mat dx;
    Mat dy;
    Sobel(src, dx, CV_32FC1, 1, 0);
    Sobel(src, dy, CV_32FC1, 0, 1);

    imshow("SRC", src);
    //imshow("dx", dx);
    //imshow("dy", dy);

    Mat mag;
    magnitude(dx, dy, mag);
    mag.convertTo(mag, CV_8UC1);    // CV_32FC1 -> CV_8UC1 for imshow()
    Mat edge = mag > 130;           // threshold
    imshow("Mag", mag);
    imshow("Edge", edge);

    //Mat _phase;
    //phase(dx, dy, _phase);


    waitKey();
    destroyAllWindows();
}

void canny_edge()
{
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    if (src.empty()) return;
    Mat dst1;
    Mat dst2;

    Canny(src, dst1, 50, 100);  //Tlow, Thigh
    Canny(src, dst2, 50, 150);

    imshow("SRC", src);
    imshow("DST1", dst1);
    imshow("DST2", dst2);

    waitKey();
    destroyAllWindows();
}

void hough_lines()
{
    Mat src = imread("building.jpg", IMREAD_GRAYSCALE);
    if (src.empty()) return;

    Mat edge;

    Canny(src, edge, 50, 150);

    std::vector<Vec2f> lines;
    HoughLines(edge, lines, 1.0, CV_PI / 180, 250);

    Mat dst;
    cvtColor(edge, dst, COLOR_GRAY2BGR);

    for (int i = 0; i < lines.size(); ++i) {
        float r = lines[i][0];
        float t = lines[i][1];
        double cos_t = cos(t);
        double sin_t = sin(t);
        double x0 = r * cos_t;
        double y0 = r * sin_t;
        double alpha = 1000;

        Point pt1(cvRound(x0 + alpha * (-sin_t)), cvRound(y0 + alpha * cos_t));
        Point pt2(cvRound(x0 - alpha * (-sin_t)), cvRound(y0 - alpha * cos_t));
        line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }

    imshow("SRC", src);
    imshow("DST", dst);

    waitKey();
    destroyAllWindows();
}

void hough_circles()
{
    Mat src = imread("coins3.jpg", IMREAD_GRAYSCALE);
    if (src.empty()) {
        std::cerr << "img load failed" << std::endl;
        return;
    }

    std::vector<Vec3f> circles;

    Mat blurred;
    blur(src, blurred, Size(3, 3));
    
    // coins.png
    //HoughCircles(blurred, circles, HOUGH_GRADIENT, 1, 100, 220, 100);   // 너무 많이 잡아내면 6, 7번째 인자 수치 높이기,, 댕노가다;;
    // coins2.png
    HoughCircles(blurred, circles, HOUGH_GRADIENT, 1, 10, 166, 67);
    // coins3.jpg
    HoughCircles(blurred, circles, HOUGH_GRADIENT, 1, 50, 245, 95); 


    Mat dst;
    cvtColor(src, dst, COLOR_GRAY2BGR);

    for (auto&& _circle : circles) {
        Point center(cvRound(_circle[0]), cvRound(_circle[1]));
        int radius = cvRound(_circle[2]);

        circle(dst, center, radius, Scalar(0, 0, 255), 2, LINE_AA);
    }

    imshow("SRC", src);
    imshow("DST", dst);

    waitKey();
    destroyAllWindows();
}

void color_grayscale()
{
    Mat src = imread("butterfly.jpg");
    Mat dst;
    cvtColor(src, dst, COLOR_BGR2GRAY);
    imshow("SRC", src);
    imshow("DST", dst);

    waitKey();
    destroyAllWindows();
}

void color_split()
{
    Mat src = imread("candies.png");    // png -> 4 channel
    std::vector<Mat> bgr_planes;
    split(src, bgr_planes);

    imshow("SRC", src);
    imshow("B_plane", bgr_planes[0]);
    imshow("G_plane", bgr_planes[1]);
    imshow("R_plane", bgr_planes[2]);

    waitKey();
    destroyAllWindows();

}
