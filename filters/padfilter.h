#ifndef PADFILTER_H
#define PADFILTER_H

#include "filter.h"

class PadFilter : public Filter
{
	public:
		PadFilter(Filter *source);

		QString getName() const { return "PadFilter"; }

		bool clicked(Point p, QMouseEvent *me);
		void reset();

	protected:
		Result * applyInternal(Image *img);

		Filter *source;

		Range<double> areaRange, ratioRange;

		QList<Point> addPoints, delPoints;
};

#endif // PADFILTER_H
