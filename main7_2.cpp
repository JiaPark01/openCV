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
        //static int cnt = 0;
        //int axis[4][22] = {0, };

        if (event == EVENT_LBUTTONDOWN) {
            if (cnt < 4) {
                srcQuad[cnt] = Point2f(x, y);
                //axis[cnt][0] = x;
                //axis[cnt][1] = y;
                ++cnt;

                circle(src, Point(x, y), 5, Scalar(0, 0, 255), -1);
                imshow("SRC", src);

                if (cnt == 4) {
/*
                    for (int i = 0; i < 4; i += 2) {
                        if (axis[i][0] > axis[i+1][0]) {
                            int tempX = axis[i][0];
                            int tempY = axis[i][1];
                            axis[i][0] = axis[i+1][0];
                            axis[i][1] = axis[i+1][1];
                            axis[i+1][0] = tempX;
                            axis[i+1][1] = tempY;
                        }
                    }

                    for (int i = 0; i < 2; ++i) {
                        if (axis[i][1] > axis[i+2][1]) {
                            int tempX = axis[i][0];
                            int tempY = axis[i][1];
                            axis[i][0] = axis[i+2][0];
                            axis[i][1] = axis[i+2][1];
                            axis[i+2][0] = tempX;
                            axis[i+2][1] = tempY;
                        }
                    }
                    for (int i = 0; i < 4; ++i) {
                        srcQuad[i] = Point2f(axis[i][0], axis[i][1]);
                    }
*/                                       
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
