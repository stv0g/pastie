#ifndef PAINTER_H
#define PAINTER_H

#include <QPainter>

#include "image.h"
#include "cast.h"
#include "pad.h"

class Painter : public QPainter
{
	public:
		Painter(QPaintDevice *dev);

		void drawOverlay(Image *p);

		void drawMarker(const QPoint &center, int radius = 8);
		void drawPad(const Pad &pad);

		/* Proxy to adjust width */
		void setPen(const QPen &pen);

		double getRatio() const { return ratio; }
		void setRatio(double r) { ratio = r; }

	protected:
		double ratio;
};

#endif // PAINTER_H
