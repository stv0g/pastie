#include "watershed.h"

void Watershed::clicked(Point pos, QMouseEvent *me)
{
	qDebug() << getName() << ": Marker " << ((me->modifiers() & Qt::AltModifier) ? "deleted" : "added") << " at:" << toQt(pos);

	if (me && me->modifiers() & Qt::AltModifier)
		delMarkers.push_back(pos);
	else
		addMarkers.push_back(pos);

	emit filterChanged(this);
}

void Watershed::reset()
{
	qDebug() << getName() << ": Markers cleared";

	delMarkers.clear();
	addMarkers.clear();
}

Result * Watershed::applyInternal(Image *img)
{
	Mat dist, dist_8u, &m = img->getMat();
	Mat kernel, max, mask;

	CV_Assert(m.channels() == 1);

	distanceTransform(m, dist, DIST_L2, 3);

	normalize(dist, dist, 0, 0xff, NORM_MINMAX);
	dist.convertTo(dist_8u, CV_8U);

	kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	dilate(dist_8u, max, kernel);

	mask = (dist_8u > 0);
	m = (max == dist_8u);
	m &= mask;

	return NULL;
}

