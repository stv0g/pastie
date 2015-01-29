#ifndef PADFILTER_H
#define PADFILTER_H

#include "filter.h"

class PadFilter : public Filter
{
	public:
		PadFilter(Filter *source);

		QString getName() const { return "PadFilter"; }

		void clicked(Point p, QMouseEvent *me);
		void reset();

	protected:
		Result * applyInternal(Image *img);

		Filter *source;

		Range<double> areaRange, ratioRange;

		QList<Point2f> addPoints, delPoints;
};

#endif // PADFILTER_H
