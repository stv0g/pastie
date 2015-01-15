#include "edgedetect.h"

#include <opencv2/imgproc.hpp>

EdgeDetect::EdgeDetect(double t, double r) :
	threshold(t),
	ratio(r)
{
	settings["Threshold"] = new DoubleSetting(this, threshold);
	settings["Ratio"] =     new DoubleSetting(this, ratio);
}

Result * EdgeDetect::applyInternal(Image *img)
{
	Mat &m = img->getMat();

	CV_Assert(m.channels() == 1);

	Canny(m, m, threshold, threshold * ratio);

	return NULL;
}

