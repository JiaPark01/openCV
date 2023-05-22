#include <opencv2/opencv.hpp>   // this includes iostream
// main2.cpp
extern void matop();
extern void matop2();
extern void matop3();
extern void matop4();
extern auto matop5() -> void;
extern void matop6();

// main3.cpp
extern void camera_in();
extern void video_in();
extern void video_out();
extern void drawLines();
extern void drawPolys();
extern void keyboard_event();
extern void mouse_event();

// main4.cpp
extern void trackbar_event();
extern void mask_setTo();
extern void mask_copyTo();

// main5.cpp
extern void brightness1();
extern void brightness3();
extern void brightness4();
extern void show_hist();
extern void histogram_stretching();
extern void histogram_equalization();

// main6.cpp
extern void arithmetic();
extern void logical();
extern void filter_embossing();
extern void blurring_mean();
extern void blurring_gaussian();
extern void unsharp_mask();
extern void noise_gaussian();
extern void filter_bilateral();
extern void filter_median();
extern void affine_transform();
extern void affine_translation();

// main7.cpp
extern void affine_scale();
extern void affine_rotation();
extern void affine_flip();
extern void perspective_transform();
extern void sobel_edge();
extern void canny_edge();
extern void hough_lines();
extern void hough_circles();
extern void color_grayscale();
extern void color_split();

// main8.cpp
extern void using_threshold();
extern void adaptive_threshold();
extern void erode_dilate();

// main9.cpp
extern void labeling_basic();
extern void labeling_stats();
extern void contours_basic();
extern void contours_hier();
extern void find_polygon();

// main10.cpp
extern void template_matching();
extern void detect_face();
extern void detect_walkers();

// detect_face.cpp
extern void detect_dnn_face();

// knn.cpp
extern void do_knn_machine_learning();

int main()
{
    std::cout << CV_VERSION << std::endl;
    
    //::matop();
    //::matop2();
    //::matop3();
    //::matop4();
    //::matop5();
    //::matop6();

    //::camera_in();
    //::video_in();
    //::video_out();
    //::drawLines();
    //::drawPolys();
    //::keyboard_event();
    //::mouse_event();

    //::trackbar_event();
    //(*::mask_setTo)();
    //::mask_copyTo();

    //::brightness1();
    //::brightness3();
    //::brightness4();
    //::show_hist();
    //::histogram_stretching();
    //::histogram_equalization();
    
    //::arithmetic();
    //::logical();
    //::filter_embossing();
    //::blurring_mean();
    //::blurring_gaussian();
    //::unsharp_mask();
    //::noise_gaussian();
    //::filter_bilateral();
    //::filter_median();
    //::affine_transform();
    //::affine_translation();

    //::affine_scale();
    //::affine_rotation();
    //::affine_flip();
    //::perspective_transform();
    //::sobel_edge();
    //::canny_edge();
    //::hough_lines();
    //::hough_circles();
    //::color_grayscale();
    //::color_split();
    
    //::using_threshold();
    //::adaptive_threshold();
    //::erode_dilate();

    //::labeling_basic();
    //::labeling_stats();
    //::contours_basic();
    //::contours_hier();
    //::find_polygon();

    //::template_matching();
    ::detect_face();
    //::detect_walkers();

    //::detect_dnn_face();

    //::do_knn_machine_learning();

    return 0;
}
