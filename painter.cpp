#include "painter.h"

Painter::Painter(QPaintDevice *device, double r) :
	QPainter(device),
	ratio(r)
{ }


void Painter::drawMarker(const QPoint &center, int r)
{
	drawEllipse(center, r, r);
	drawLine(center + QPoint(+r, +r), center - QPoint(+r, +r));
	drawLine(center + QPoint(+r, -r), center - QPoint(+r, -r));
}

