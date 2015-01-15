#ifndef PAINTER_H
#define PAINTER_H

#include <QPainter>

#include "cast.h"
#include "pad.h"

class Painter : public QPainter
{
	public:
		Painter(QPaintDevice *dev) :
			QPainter(dev)
		{ }

		void drawMarker(const QPoint &center, int radius = 8);
		void drawPad(const Pad &pad);

		/* Proxy to adjust width */
		void setPen(const QPen &pen);

		double getRatio();
};

#endif // PAINTER_H
