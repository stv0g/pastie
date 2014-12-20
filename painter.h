#ifndef PAINTER_H
#define PAINTER_H

#include <QPainter>

class Painter : public QPainter
{
	public:
		Painter(QPaintDevice *device, double ratio = 1);

		void drawMarker(const QPoint &center, int radius = 8);

	protected:
		double ratio;

};

#endif // PAINTER_H
