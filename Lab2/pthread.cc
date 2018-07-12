#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

Mat& ScanImageAndReduceC(Mat& I, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);
    int channels = I.channels();
    int nRows = I.rows;
    int nCols = I.cols * channels;

    cout << nRows << " **" << nCols << endl;
    if (I.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }
    int i,j;
    uchar* p;
    for( i = 0; i < nRows; ++i)
    {
        p = I.ptr<uchar>(i);
        for ( j = 0; j < nCols; ++j)
        {
            cout << (int)p[j] << endl;
            p[j] = table[p[j]];
        }
    }
    return I;
}
int main( int argc, char** argv )
{
    if( argc != 3)
    {
     cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    // Mat image (2, 2, CV_8UC3, Scalar(0,0,255));
    // cout << "M =" << endl << image << endl << endl;
    image = imread(argv[1], IMREAD_GRAYSCALE);
    int divideWith = 0; // convert our input string to number - C++ style
    stringstream s;
    s << argv[2];
    s >> divideWith;
    if (!s || !divideWith)
    {
        cout << "Invalid number entered for dividing. " << endl;
        return -1;
    }
    uchar table[256];
    for (int i = 0; i < 256; ++i)
       table[i] = (uchar)(divideWith * (i/divideWith));

    // ScanImageAndReduceC(image, table);
    // threshold(image, image, 150, 1, CV_THRESH_BINARY);

    ScanImageAndReduceC(image, table);

       
    // image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                // Show our image inside it.

    waitKey(0);             // Wait for a keystroke in the window
    return 0;
}
