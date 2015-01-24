#include <opencv2/imgproc.hpp>

#include <QPainter>
#include <QtDebug>

#include "cast.h"
#include "pattern.h"
#include "image.h"
#include "convert.h"

Pattern::Pattern(Size si, int sp, Type t) :
	Pattern(si, Size(sp, sp), t)
{ }

Pattern::Pattern(Size si, Size sp, Type t) :
	type(t),
	size(si),
	spacing(sp),
	areaRange(4000, 40000),
	ratioRange(1, 1.7),
	showThresh(false)
{
	QMap<int, QString> mapType;
	mapType[CHESSBOARD] = "Chessboard";
	mapType[CIRCLES_GRID] = "Circles Grid";
	mapType[ASYMMETRIC_CIRCLES_GRID] = "Asymmetric Circles Grid";
	mapType[QUADRILINEAR_MARKERS] = "Quadrilinear Markers";

	settings["Size"]	= new SizeSetting(this, size, Range<int>(1, 16));
	settings["Spacing"] = new SizeSetting(this, spacing, Range<int>(0, 5000));
	settings["Type"]	= new EnumSetting(this, (int&) type, mapType);

	if (type == QUADRILINEAR_MARKERS) {
		settings["Show Threshold"] = new BooleanSetting(this, showThresh);
		settings["Area Range"] = new DoubleRangeSetting(this, areaRange, Range<double>(0000, 50000));
		settings["Ratio Range"] = new DoubleRangeSetting(this, ratioRange, Range<double>(1, 2));
	}
}

Result * Pattern::applyInternal(Image *img)
{
	bool found = false;
	std::vector<Point2f> points, allPoints;
	std::vector<std::vector<Point>> contours;

	Mat gray, thresh, &m = img->getMat();
	if (m.channels() > 1)
		cvtColor(m, gray, COLOR_RGB2GRAY);
	else
		gray = m.clone();

	switch(type) {
		case CHESSBOARD:
			found = findChessboardCorners(gray, size, points, flags);
			if (found)
				cornerSubPix(gray, points, Size(11, 11), Size(-1, -1),
							 TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
			break;

		case CIRCLES_GRID:
			found = findCirclesGrid(gray, size, points);
			break;

		case ASYMMETRIC_CIRCLES_GRID:
			found = findCirclesGrid(gray, size, points, CALIB_CB_ASYMMETRIC_GRID);
			break;

		case QUADRILINEAR_MARKERS:
			blur(gray, gray, Size(13, 13));
			threshold(gray, thresh, 0, 0xff, THRESH_BINARY_INV | THRESH_OTSU);

			if (showThresh) {
				m = thresh.clone();
				cvtColor(m, m, COLOR_GRAY2BGR);
			}

			findContours(thresh, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

			for (int idx = 0; idx < contours.size(); idx++) {
				std::vector<Point> &contour = contours[idx];
				std::vector<Point> approx;

				double arc = arcLength(contour, true);
				approxPolyDP(contour, approx, arc * 0.1, true);

				if (contour.size() > 5) {
					RotatedRect e = fitEllipse(contour);
					int minSide = std::min(e.size.width, e.size.height);
					int maxSide = std::max(e.size.width, e.size.height);
					double ratio = (double) maxSide / minSide;


					Mat roi = thresh(boundingRect(contour));
					double nz = countNonZero(roi);

					if (showThresh)
						ellipse(m, e, Scalar(0,0xff,0), 3);

					if (areaRange.contains(nz) &&
						ratioRange.contains(ratio)  &&
						isContourConvex(approx)) {

						allPoints.push_back(e.center);

						if (showThresh)
							ellipse(m, e, Scalar(0,0,0xff), 3);
					}
				}
			}

			if (allPoints.size()) {
				convexHull(allPoints, points);

				found = (points.size() == 4);
				if (found)
					std::swap(points[2], points[3]);
			}

			break;

		default:
			return NULL;
	}

	return new PatternResult(found, points);
}

std::vector<Point3f> Pattern::getPoints() const
{
	std::vector<Point3f> points;

	switch(type) {
		case CHESSBOARD:
		case CIRCLES_GRID:
			for (int i = 0; i < size.height; ++i) {
				for (int j = 0; j < size.width; ++j)
					points.push_back(Point3f(j*spacing.width, i*spacing.height, 0));
			}
			break;

		case ASYMMETRIC_CIRCLES_GRID:
			for (int i = 0; i < size.height; i++) {
				for (int j = 0; j < size.width; j++)
					points.push_back(Point3f((2*j+i%2)*spacing.width, i*spacing.height, 0));
			}
			break;

		case QUADRILINEAR_MARKERS:
			points.push_back(Point3f(0, 0, 0));
			points.push_back(Point3f(spacing.width, 0, 0));
			points.push_back(Point3f(0, spacing.height, 0));
			points.push_back(Point3f(spacing.width, spacing.height, 0));
			break;

		default:
			break;
	}

	return points;
}

std::vector<int> Pattern::getCornerIndizes() const
{
	return {
		0, size.width - 1,
		size.area() - size.width,
		size.area() - 1
	};
}

PatternResult::PatternResult(bool f, std::vector<Point2f> p) :
	found(f),
	points(p)
{ }

void PatternResult::drawResult(Painter *p) const
{
	QPen pe = p->pen();

	pe.setWidth(2);

	for (unsigned i = 0; i < points.size(); i++) {
		if (found)
			pe.setColor(QColor::fromHsl(360.0 / points.size() * i, 255, 128));
		else
			pe.setColor(Qt::gray);

		p->setPen(pe);
		p->drawMarker(toQt((Point2i) points[i]));

		if (i != 0)
			p->drawLine(toQt((Point2i) points[i-1]), toQt((Point2i) points[i]));
	}
}

QString PatternResult::getResult() const
{
	return QString("found = %1, points = %2").arg(found).arg(points.size());
}
