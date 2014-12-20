#include <opencv2/imgproc.hpp>

#include <QPainter>
#include <QtDebug>

#include "cast.h"
#include "pattern.h"
#include "image.h"
#include "convert.h"

Pattern::Pattern(Size si, double sp, Type t) :
	type(t),
	size(si),
	spacing(sp)
{
	//QMap<int, QString> mapType

	settings["Size"]	= new SizeSetting(this, size, 1, 20);
	settings["Spacing"] = new DoubleSetting(this, spacing, 1e-3, 1e2);
	//settings["Type"]	= new EnumSetting(this, type, mapType);

}

Result * Pattern::applyInternal(Image *img)
{
	bool found;
	std::vector<Point2f> points;

	Mat gray;
    if (img->original.channels() > 1)
        cvtColor(img->original, gray, COLOR_RGB2GRAY);
	else
        gray = img->original;

	switch(type) {
		case CHESSBOARD:
			found = findChessboardCorners(gray, size, points, flags);
			if(found) {
				cornerSubPix(gray, points, Size(11, 11), Size(-1, -1),
							 TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
			}
			break;

		case CIRCLES_GRID:
			found = findCirclesGrid(gray, size, points);
			break;

		case ASYMMETRIC_CIRCLES_GRID:
			found = findCirclesGrid(gray, size, points, CALIB_CB_ASYMMETRIC_GRID);
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
					points.push_back(Point3f(j*spacing, i*spacing, 0));
			}
			break;

		case ASYMMETRIC_CIRCLES_GRID:
			for (int i = 0; i < size.height; i++) {
				for (int j = 0; j < size.width; j++)
					points.push_back(Point3f((2*j+i%2)*spacing, i*spacing, 0));
			}
			break;

		default:
			break;
	}

	return points;
}

std::vector<int> Pattern::getCornerIndizes() const
{
	return {
		0,
		size.width - 1,
		size.width * size.height - size.width,
		size.width * size.height - 1
	};
}

PatternResult::PatternResult(bool f, std::vector<Point2f> p) :
	found(f),
	points(p)
{ }

void PatternResult::draw(Painter *p) const
{
	QPen pen = p->pen();

	for (unsigned i = 0; i < points.size(); i++) {
		QColor clr = found ? QColor::fromHsl(360.0 / points.size() * i, 255, 128) : Qt::gray;

		pen.setColor(clr);
		p->setPen(pen);
		p->drawMarker(toQt((Point2i) points[i]));

		if (i != 0)
			p->drawLine(toQt((Point2i) points[i-1]), toQt((Point2i) points[i]));
	}
}

QString PatternResult::getResult() const
{
	return QString("found = %1, points = %2").arg(found).arg(points.size());
}
