#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <unistd.h>

using namespace cv;
using namespace std;

extern "C" {
JNIEXPORT void JNICALL Java_nz_net_plan9_can_1hunter_MainActivity_FindCan(JNIEnv*, jobject, jlong addrIn, jlong addrOut);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        return 1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    Mat outImage(image.size(), CV_8UC1);
    //outImage = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    Java_nz_net_plan9_can_1hunter_MainActivity_FindCan(NULL, NULL, reinterpret_cast<jlong> (&image), reinterpret_cast<jlong> (&outImage));

    imwrite( argv[2], outImage );

    usleep(1000000);

    return 0;
}
