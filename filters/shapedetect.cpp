#include "cast.h"
#include "shapedetect.h"

static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;

	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

static double area(RotatedRect rect)
{
	Point2f p[4];
	rect.points(p);

	double dxa = p[0].x - p[1].x;
	double dya = p[0].y - p[1].y;
	double dxb = p[1].x - p[2].x;
	double dyb = p[2].y - p[2].y;

	return sqrt( (dxa*dxa + dya*dya) * (dxb*dxb + dyb*dyb) );
}

ShapeDetect::ShapeDetect(double e, double cos) :
	epsilon(e),
	cosine(cos)
{
	QMap<int, QString> mapMode;
	mapMode[AREA] = "Area";
	mapMode[COSINE] = "Cosine";

	settings["Mode"]	= new EnumSetting(this, (int&) mode, mapMode);
	settings["Epsilon"] = new DoubleSetting(this, epsilon, 0, 1);
	settings["Cosine"]  = new DoubleSetting(this, cosine, 0, 1);
	settings["Shape no"]= new IntegerSetting(this, shape, 0, 1000);
}

Result * ShapeDetect::applyInternal(Image *img)
{
	Mat &m = img->filtered;
	int i = 0;

	std::vector<std::vector<Point>> contours;
	ShapeResult *result = new ShapeResult;

	findContours(m.clone(), contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	for (std::vector<Point> contour : contours) {
	//std::vector<Point> contour = contours[shape];

		std::vector<Point> approx;
		bool closed = isContourConvex(contour);
		double arc = arcLength(contour, closed);

		approxPolyDP(contour, approx, epsilon * arc, closed);

		switch (approx.size()) {
			case 4:
				if (mode == ShapeDetect::COSINE) {
					double maxCosine = 0;

					for (int j = 2; j < 5; j++) {
						double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
						maxCosine = std::max(maxCosine, cosine);
					}

					if (maxCosine < cosine) {
						RotatedRect rect = minAreaRect(approx);
						//RotatedRect rect = minAreaRect(contour);
						if (i++ == shape)
							result->rects.push_back(rect);
					}
				}
				else if (mode == ShapeDetect::AREA) {
					RotatedRect rect	= minAreaRect(contour);

					double areaContour	= contourArea(contour);
					double areaRect		= area(rect);

					if (areaContour / areaRect > epsilon) {
						if (i++ == shape)
							result->rects.push_back(rect);
					}
				}
				break;
		}
	}

	return result;
}

void ShapeResult::draw(Painter *p) const
{
	QPen pen = p->pen();
	pen.setWidth(2);
	p->setPen(pen);

	for (auto rect : rects) {
		Point2f v[4];
		rect.points(v);

        for (int i = 0; i < 4; i++)
			p->drawLine(toQt((Point2i) v[i]), toQt((Point2i) v[(i+1)%4]));
    }
}

QString ShapeResult::getResult() const
{
	return QString("rects = %1").arg(rects.size());
}
