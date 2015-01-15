#include "painter.h"

void Painter::drawMarker(const QPoint &center, int r)
{
	QBrush br(Qt::NoBrush);
	setBrush(br);

	r /= getRatio();

	drawEllipse(center, r, r);
	drawLine(center + QPoint(+r, +r), center - QPoint(+r, +r));
	drawLine(center + QPoint(+r, -r), center - QPoint(+r, -r));
}

void Painter::drawPad(const Pad &pad)
{
	const Point2f *v = pad.getVertexes();
	QPointF vq[4];

	for (int i = 0; i < 4; i++)
		vq[i] = toQt(v[i]);

	drawPolygon(vq, 4);
}

void Painter::setPen(const QPen &pen)
{
	QPen p(pen);

	p.setWidthF((double) p.widthF() / getRatio());

	QPainter::setPen(p);
}

double Painter::getRatio()
{
	return (transform().m11() + transform().m22()) / 2;
}
