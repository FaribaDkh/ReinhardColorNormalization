#include <iostream>
#include <cmath>
#include "opencv2/imgproc.hpp"
#include "opencv2/photo.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utility.hpp>


using namespace cv;
using namespace std;


void NormReinhard(Mat& src, Mat& ref)
{
	unsigned int sizeSource = src.size[0] * src.size[1] * 4;
	float* sourceDataLab = new float[sizeSource] {0.f};

	Mat target;
	cvtColor(src, src, cv::COLOR_RGB2Lab);
	cvtColor(ref, ref, cv::COLOR_RGB2Lab);

	Scalar mean_src, stddev_src;
	Scalar mean_ref, stddev_ref;

	meanStdDev(src, mean_src, stddev_src);
	meanStdDev(ref, mean_ref, stddev_ref);

	float l_Ratio = float(stddev_ref[0] / stddev_src[0]);
	float a_Ratio = float(stddev_ref[1] / stddev_src[1]);
	float b_Ratio = float(stddev_ref[2] / stddev_src[2]);

	cv::Mat result = Mat::zeros(src.size(), CV_8UC3);

	for (int x = 0; x < src.cols; x++) 
	{
		for (int y = 0; y < src.rows; y++)
		{
			Vec3b intensity = src.at<cv::Vec3b>(x, y);
			Vec3b intensity_result;
			int blue_result = intensity_result[0];
			int green_result = intensity_result[1];
			int red_result = intensity_result[2];

			int blue = intensity[0];
			int green = intensity[1];
			int red = intensity[2];


			blue_result = ((blue - mean_src[0]) * l_Ratio) + mean_ref[0];
			green_result = ((green - mean_src[1]) * a_Ratio)+ mean_ref[1];
			red_result = ((red - mean_src[2]) * b_Ratio )+ mean_ref[2];

			intensity_result[0] = blue_result;
			intensity_result[1] = green_result;
			intensity_result[2] = red_result;

			result.at<Vec3b>(Point(x, y)) = intensity_result;
			//std::cout << result << endl;
		}
	}
	cvtColor(result, result, cv::COLOR_Lab2BGR);
	cv::imwrite("result.png", result);

}

int main(int argc, char** argv)
{
	string image_path = "src.png";
	string ref_path = "ref.png";
	Mat src;
	Mat ref = imread(ref_path);
	src = imread(image_path);
	NormReinhard(src, ref);
	return 0;
}