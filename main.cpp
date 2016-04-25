#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

using namespace cv;
using namespace std;

int main()
{
    Mat src = imread("test.jpg", 1);
    Mat dst;

    Point2f pc(src.cols/2., src.rows/2.);
    Mat r = getRotationMatrix2D(pc, -45, 1.0);

    warpAffine(src, dst, r, src.size());

    imwrite("test_result.jpg", dst);

    return 0;
}