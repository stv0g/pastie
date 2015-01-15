#include "paddetect.h"

static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;

	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

PadDetect::PadDetect(double e, double cos) :
	epsilon(e),
	areaRatio(0.85),
	cosine(cos),
	mode(AREA)
{
	QMap<int, QString> mapMode;
	mapMode[AREA] = "Area";
	mapMode[COSINE] = "Cosine";

	settings["Mode"]	= new EnumSetting(this, (int&) mode, mapMode);
	settings["Epsilon"] = new DoubleSetting(this, epsilon, Range<double>(0, 1));
	settings["Area Ratio"] = new DoubleSetting(this, areaRatio, Range<double>(0, 1));
	settings["Cosine"]  = new DoubleSetting(this, cosine, Range<double>(0, 1));
}

Result * PadDetect::applyInternal(Image *img)
{
	Mat &m = img->getMat();

	std::vector<std::vector<Point>> contours;
	PadResult *result = new PadResult;

	findContours(m.clone(), contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	for (std::vector<Point> contour : contours) {
		std::vector<Point> approx;
		bool closed = isContourConvex(contour);
		double arc = arcLength(contour, closed);

		switch (mode) {
			case PadDetect::COSINE:
				approxPolyDP(contour, approx, epsilon * arc, closed);

				if (approx.size() == 4) {
					double maxCosine = 0;

					for (int j = 2; j < 5; j++) {
						double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
						maxCosine = std::max(maxCosine, cosine);
					}

					if (maxCosine < cosine) {
						Pad pad = minAreaRect(approx);
						//RotatedRect rect = minAreaRect(contour);
						result->push_back(pad);
					}
				}
				break;

			case PadDetect::AREA: {
				Pad pad	= minAreaRect(contour);

				double areaContour	= contourArea(contour);
				double areaRect		= pad.getArea();

				if (areaContour / areaRect > areaRatio)
					result->push_back(pad);

				break;
			}
		}
	}

	return result;
}

void PadResult::drawResult(Painter *p) const
{
	QPen pe;
	QBrush br;
	QColor clr(Qt::magenta);

	clr.setAlpha(32);

	br.setStyle(Qt::SolidPattern);
	br.setColor(clr);

	pe.setColor(Qt::magenta);
	pe.setWidth(2);

	p->setBrush(br);
	p->setPen(pe);

	for (auto pad : *this)
		p->drawPad(pad);
}

QString PadResult::getResult() const
{
	return QString("count = %1").arg(size());
}

QList<Pad>::iterator PadResult::getNearest(const Point &p)
{
	float minDist = FLT_MAX;
	QList<Pad>::iterator nearest = end();

	for (auto it = begin(); it != end(); it++) {
		float dist = norm(it->center - Point2f(p));
		if (dist < minDist) {
			nearest = it;
			minDist = dist;
		}
	}

	return nearest;
}
