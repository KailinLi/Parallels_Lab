#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
using namespace cv;
using namespace std;

extern void erosion_image(uchar *image_data, uchar *res_data, int rows, int cols);

int main(int argc, char **argv)
{
    if (argc != 2) return -1;
    Mat image = imread(argv[1], IMREAD_GRAYSCALE);      // read image
    threshold(image, image, 105, 255, CV_THRESH_BINARY_INV);  // turn image to binary
    Mat res = image.clone();
    uchar *image_data = image.ptr<uchar>(0);
    uchar *res_data = res.ptr<uchar>(0);
    int rows = image.rows;
    int cols = image.cols;
    erosion_image(image_data, res_data, rows, cols);

    // imwrite("./output.jpg", res);

    return 0;
}
