#ifndef WATERSHED_H
#define WATERSHED_H

#include "filter.h"

class Watershed : public Filter
{
	public:
		QString getName() const { return "Watershed"; }

		bool clicked(Point pos, QMouseEvent *me = 0);

	public slots:
		void reset();

	protected:
		Result * applyInternal(Image *img);

		QList<Point> addMarkers;
		QList<Point> delMarkers;
};

#endif // WATERSHED_H
