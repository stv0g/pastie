#include <opencv2/imgproc.hpp>

#include <QtDebug>

#include "cast.h"
#include "perspective.h"

Perspective::Perspective(Camera *c, Pattern *p) :
	cam(c),
	pattern(p)
{ }

Result * Perspective::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	PatternResult *presult = (PatternResult *) img->getResult(pattern);

	if (presult && presult->isFound()) {
		Mat map;
		std::vector<Point2f> src, dest, tmp;
		std::vector<Point2f> srcPoints  = presult->getPoints();
		std::vector<Point3f> destPoints = pattern->getPoints();

		for (int i : pattern->getCornerIndizes()) {
			src.push_back(srcPoints[i]);
			tmp.push_back(Point2f(destPoints[i].x, destPoints[i].y));
		}

		int destWidth = 0.9 * m.cols;
		int destHeight = tmp[2].y * (destWidth / tmp[1].x);

		dest.push_back(Point2f((m.cols - destWidth) / 2, (m.rows - destHeight) / 2));
		dest.push_back(dest[0] + Point2f(destWidth, 0));
		dest.push_back(dest[0] + Point2f(0,		 destHeight));
		dest.push_back(dest[0] + Point2f(destWidth, destHeight));

		map = getPerspectiveTransform(src, dest);
		warpPerspective(m, m, map, m.size());

		return new Result(toQTransform(map));
	}

	return NULL;
}

