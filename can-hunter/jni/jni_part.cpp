#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <android/log.h>
#include <sstream>

using namespace std;
using namespace cv;

#include "MotorControl.h"

SerialMotors motors;

int GetAverageX(Mat &image) {
	int accumulator;
	int count;

	for (unsigned ii = 0; ii < image.rows; ++ii) {
		for (unsigned jj = 0; jj < image.cols; ++jj) {
			if (image.at < uchar > (ii, jj) != 0) {
				++count;
				accumulator += jj;
			}
		}
	}
	return accumulator / count;
}

extern "C" {

JNIEXPORT jint JNICALL Java_nz_net_plan9_can_1hunter_MainActivity_FindCan(
		JNIEnv*, jobject, jlong addrIn, jlong addrOut);

JNIEXPORT jint JNICALL Java_nz_net_plan9_can_1hunter_MainActivity_FindCan(
		JNIEnv*, jobject, jlong addrIn, jlong addrOut)
{
	Mat& mIn = *(Mat*) addrIn;
	Mat mBlur;
	Mat mHSV;
	Mat mFiltered(mIn.size(), CV_8UC1);
	Mat mRotated(Size(mIn.rows, mIn.cols), CV_8UC1);
	Mat mErode(mRotated.size(), CV_8UC1);
	Mat mTranspose(mErode.size(), CV_8UC1);
	Mat mOutTemp;
	Mat& mOut = *(Mat*) addrOut;
	__android_log_print(ANDROID_LOG_DEBUG, "nz.net.plan9.can_hunter",
			"size is %d of %d", mIn.rows, mIn.cols);

	static int skip = 0;


	/*stringstream str;
	static int i = 0;
	str << "/storage/sdcard0/images/test_" << ++i << ".png";
	imwrite(str.str(),mIn);
*/
	GaussianBlur(mIn, mBlur, Size(15, 15), 0);
	cvtColor(mBlur, mHSV, COLOR_BGR2HSV);
	inRange(mHSV, Scalar(105, 128, 64), Scalar(130, 256, 212), mFiltered);

	Mat element = getStructuringElement(MORPH_CROSS, Size(5, 5));
	erode(mFiltered, mErode, element);

	int largest_area = 0;
	int largest_contour_index = 0;
	Rect bounding_rect;

	vector < vector<Point> > contours; // Vector for storing contour
	vector < Vec4i > hierarchy;

	findContours(mErode, contours, hierarchy, CV_RETR_CCOMP,
			CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image

	for (int ii = 0; ii < contours.size(); ii++) // iterate through each contour.
	{
		double a = contourArea(contours[ii], false); //  Find the area of contour
		if (a > largest_area) {
			largest_area = a;
			largest_contour_index = ii; //Store the index of largest contour
			bounding_rect = boundingRect(contours[ii]); // Find the bounding rectangle for biggest contour
		}
	}

	mOut = mIn;
	if (!contours.empty())
	{
		drawContours( mOut, contours, largest_contour_index, Scalar(0,255,255), 1, 8, hierarchy );
	}
	if (motors.AllGood()) {
		rectangle(mOut, Rect(0,0,100,2), Scalar(255, 255, 0), 1, 8, 0);
	} else {
		rectangle(mOut, Rect(0,0,100,2), Scalar(255, 0, 255), 1, 8, 0);
	}
	rectangle(mOut, bounding_rect, Scalar(255, 0, 0), 2, 8, 0);

	rectangle(mOut, Rect(0,4,100,6), Scalar(255, 255, 255), 1, 8, 0);
	if (skip > 0)
	{
		rectangle(mOut, Rect(0,4,100,6), Scalar(255, 0, 0), 1, 8, 0);
		--skip;
		return 0;
	}


	const int pulse_length = 1000; // minimum of 1000
	const char speed = 150;
	int full_area = bounding_rect.height * bounding_rect.width;
	if (full_area > 2000)
	{
		float ratio = static_cast<float> (largest_area)/static_cast<float> (full_area);
		__android_log_print(ANDROID_LOG_DEBUG, "nz.net.plan9.can_hunter",
				"full_areal %d ratio %f", full_area, ratio);

		rectangle(mOut, bounding_rect, Scalar(0, 0, 255), 2, 8, 0);

		if (ratio > 0.2)
		{
			rectangle(mOut, bounding_rect, Scalar(0, 255, 0), 2, 8, 0);
			int average = bounding_rect.y + bounding_rect.height;

			__android_log_print(ANDROID_LOG_DEBUG, "nz.net.plan9.can_hunter",
					"Average is %d of %d", average, mOut.rows);

			float errorRatio = static_cast<float> (average) /static_cast<float>(mErode.rows) - 0.5;

			if (average > 0) {
				motors.SetMotors(speed, (1-errorRatio)*speed, pulse_length);
			} else {
				motors.SetMotors((1-errorRatio)*speed, speed, pulse_length);
			}
			return full_area;
		}
	}
	// didn't find a can so just spin
	motors.SetMotors(+speed, -speed, 5*pulse_length);
	skip = 5;
	return 0;
}

} // extern C
