#include "painter.h"
#include "filters/filter.h"

Painter::Painter(QPaintDevice *dev) :
	QPainter(dev)
{
	setRenderHint(Antialiasing);
}

void Painter::drawOverlay(Image *img)
{
	for (Filter *filter : img->results.keys()) {
		Result *result = img->getResult(filter);
		if (result && filter->isShown()) {
			save();

			/*QTransform t;
			for (auto it2 = it+1; it2 != filters->end(); it2++) {
				Result *result2 = img->getResult(*it2);
				if (result2 && (*it2)->isShown())
					t *= result2->getTransform();
			}
			p.setWorldTransform(t * transform);*/

			result->drawResult(this);
			restore();
		}
	}
}

void Painter::drawMarker(const QPoint &center, int r)
{
	QBrush br(Qt::NoBrush);
	setBrush(br);

	r *= ratio;

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

	p.setWidthF((double) p.widthF() * ratio);

	QPainter::setPen(p);
}
