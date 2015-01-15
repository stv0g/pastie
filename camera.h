#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/videoio.hpp>

#include <list>

#include "source.h"
#include "filters/pattern.h"

using namespace cv;
using namespace std;

class Image;

class Camera
{
	public:
		Camera(Source *s);

		bool calibrate(QList<Image *> imgs, Pattern *pattern);

		/* Getters */
		const Mat & getDistCoeffs() { return distCoeffs; }
		const Mat & getMatrix() { return matrix; }
		double getAvgReprErr() { return avgReprErr; }
		bool isCalibrated() { return calibrated; }

		void reset();

		Source *source;

	protected:
		bool calibrated;
		double avgReprErr;

		Mat matrix;
		Mat distCoeffs;
};

#endif // CAMERA_H
