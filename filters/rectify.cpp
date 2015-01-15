#include "rectify.h"

Rectify::Rectify(double t) :
	threshold(t),
	thresholdHough(100)
{
	settings["Threshold Degree"] = new DoubleSetting(this, threshold, Range<double>(0, 360));
	settings["Threshold Hough"]  = new IntegerSetting(this, thresholdHough, Range<int>(0, 500));
}

Result * Rectify::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	Mat affine;
	Point2f center;
	double angle, degrees;

	center = Point2f(m.cols / 2.0, m.rows / 2.0);
	angle = getHoughRotation(m);
	degrees = angle * 180.0 / CV_PI;

	affine = getRotationMatrix2D(center, degrees, 1);
	warpAffine(m, m, affine, m.size());

	return new DoubleResult("angle", degrees, toQTransform(affine));
}

double Rectify::getHoughRotation(Mat &m)
{
	Mat bw;
	int dim, maxVal = 0;
	float angle;

	std::vector<Vec2f> lines;

	Canny(m, bw, 60, 180, 3);
	HoughLines(bw, lines, 1, CV_PI / 180.0, thresholdHough);

	dim = lines.size();
	for (int i = 0; i < dim; i++) {
		int val = 0;
		float pi = mapQuadrant(lines[i][1]);

		for (int j = 0; j < dim; j++) {
			float pj = mapQuadrant(lines[j][1]);

			if (pi < 0) pi += CV_PI / 2.0;
			if (pj < 0) pj += CV_PI / 2.0;

			float diff = fabs(pi - pj);

			if (diff <= threshold * CV_PI / 180.0)
				val++;
		}

		if (val > maxVal) {
			maxVal = val;
			angle = pi;
		}
	}

#if 1
	for (int i = 0; i < dim; i++) {
		Point pt1, pt2;

		float rho = lines[i][0], theta = lines[i][1];
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;

		if (fabs(rho - angle) < threshold * CV_PI / 180.0) {
			pt1.x = cvRound(x0 + 1000*(-b));
			pt1.y = cvRound(y0 + 1000*(a));
			pt2.x = cvRound(x0 - 1000*(-b));
			pt2.y = cvRound(y0 - 1000*(a));

			line(m, pt1, pt2, Scalar(0, 0, 255), 1, LINE_AA);
		}
	}
#endif

	return angle;
}

double Rectify::mapQuadrant(double angle)
{
	double result = fmod(angle, CV_PI/2);
	if (result < 0)
		result += 90;

	return result;
}
