#include "watershed.h"

Watershed::Watershed()
{

}

Result * Watershed::applyInternal(Image *img)
{
	Mat dist, dist_8u, &m = img->filtered;

	CV_Assert(m.channels() == 1);

	distanceTransform(m, dist, DIST_L2, 3);
	normalize(m, m, 0, 0xff, NORM_MINMAX);

	dist.convertTo(dist_8u, CV_8U);
	m =  dist_8u;

	return NULL;
}

