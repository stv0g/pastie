#include <opencv2/imgproc.hpp>

#include <QtDebug>

#include "cast.h"
#include "perspective.h"

Perspective::Perspective(Camera *c, Pattern *p) :
	alpha(1),
	cam(c),
	pattern(p)
{
	settings["Scale"] = new DoubleSetting(this, alpha, Range<double>(0.5, 2));
}

Result * Perspective::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	PatternResult *presult = (PatternResult *) img->getResult(pattern);

	if (presult && presult->isFound()) {
		Mat map;
		std::vector<Point2f> src, dest;
		std::vector<Point2f> srcPoints  = presult->getPoints();
		std::vector<Point3f> destPoints = pattern->getPoints();

		for (int i : pattern->getCornerIndizes()) {
			src.push_back(srcPoints[i]);
			dest.push_back(Point2f(destPoints[i].x, destPoints[i].y));
		}

		double scale = alpha * norm(src[1] - src[0]) / norm(dest[1] - dest[0]);

		for (Point2f &p : dest)
			p *= scale;

		Size newSize(dest[3].x, dest[3].y);

		map = getPerspectiveTransform(src, dest);
		warpPerspective(m, m, map, newSize);

		return new Result(toQTransform(map));
	}

	return NULL;
}

