#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <QtDebug>
#include <QMouseEvent>

#include "cast.h"
#include "painter.h"
#include "filterlist.h"
#include "viewer.h"
#include "camera.h"

using namespace cv;

extern Camera *cam;
extern FilterList *filters;

Viewer::Viewer(QWidget *parent) :
	QGLWidget(parent),
	img(NULL)
{ }

void Viewer::reset()
{
	Mat &m = img->getMat();
	updateWindow(QRect(0, 0, m.cols, m.rows));
}

void Viewer::resizeEvent(QResizeEvent *)
{
	updateTransform();
}

void Viewer::paintEvent(QPaintEvent *)
{
	Painter p(this);
	QPen pe;

	p.setRenderHint(QPainter::Antialiasing);
	p.fillRect(rect(), Qt::black);
	p.setClipRect(viewport);
	p.setTransform(transform);

	pe.setStyle(Qt::SolidLine);
	pe.setWidth(2);
	p.setPen(pe);

	if (img) {
		p.drawImage(window, qimg, window);

		for (auto it = filters->begin(); it != filters->end(); it++) {
			Result *result = img->getResult(*it);
			if (result && (*it)->isShown()) {
				p.save();

				/*QTransform t;
				for (auto it2 = it+1; it2 != filters->end(); it2++) {
					Result *result2 = img->getResult(*it2);
					if (result2 && (*it2)->isShown())
						t *= result2->getTransform();
				}
				p.setWorldTransform(t * transform);*/

				result->drawResult(&p);
				p.restore();
			}
		}
	}

	if (!first.isNull() && !last.isNull()) {
		QRect sel(unmap(first), unmap(last));

		pe.setWidth(2);
		pe.setStyle(Qt::SolidLine);
		pe.setColor(Qt::black);
		p.setPen(pe);
		p.drawRect(sel.normalized());

		pe.setStyle(Qt::DotLine);
		pe.setBrush(Qt::white);
		p.setPen(pe);
		p.drawRect(sel.normalized());
	}
}

void Viewer::wheelEvent(QWheelEvent * we)
{
	QPoint numPixels = we->pixelDelta();
	QPoint numDegrees = we->angleDelta() / 8;

	int d = 0;

	if (!numPixels.isNull())
		d = numPixels.y();
	else if (!numDegrees.isNull())
		d = numDegrees.y();

	double ratio = (double) window.width() / window.height();

	if (d > 0 || (d < 0 && window.height() + 2*d > 32)) {
		updateWindow(window.adjusted(
			-d * ratio, -d,
			 d * ratio,  d
		));

		update();
		we->accept();
	}
	else
		we->ignore();
}

void Viewer::mousePressEvent(QMouseEvent *me)
{	
	if (viewport.contains(me->pos())) {
		if (me->modifiers() & Qt::AltModifier)
			first = me->pos();
		else
			last = me->pos();
	}
}

void Viewer::mouseMoveEvent(QMouseEvent *me)
{
	if (viewport.contains(me->pos())) {
		if (me->modifiers() & Qt::AltModifier) {
			update();
		}
		else {
			QPoint delta = (me->pos() - last) / transform.m11();
			window.moveCenter(window.center() - delta);

			updateWindow(window);
			update();
		}
	}

	last = me->pos();
}

void Viewer::mouseReleaseEvent(QMouseEvent *me)
{
	if (viewport.contains(me->pos())) {
		if (me->modifiers() & Qt::AltModifier) {
			if (!first.isNull() && !last.isNull())
				updateWindow(QRect(unmap(first), unmap(last)).normalized());
		}
		else if (last.isNull()) {
			QPoint pos = unmap(me->pos());
			Filter *f = filters->getCurrent();

			qDebug() << "Clicked at: " << pos << "(" << me->pos() << ")";

			if (f) {
				if (f->clicked(toCv(pos), me))
					updateImage();
			}
		}
	}

	first = last = QPoint();

	update();
}

void Viewer::showImage(Image *next)
{
	if (next) {
		if (next != img)
			filters->reset();

		img = next;
		updateImage();
	}
}

void Viewer::updateImage()
{
	if (img) {
		filters->execute(img);

		Mat &m = img->getMat();
		qimg = toQImage(m);

		if (window.size() != toQt(m.size()))
			updateWindow(QRect(0, 0, m.cols, m.rows));

		update();
	}
}

void Viewer::updateWindow(const QRect &win)
{
	window = win;
	updateTransform();
}

void Viewer::updateTransform()
{
	QSizeF s = window.size();
	s.scale(size(), Qt::KeepAspectRatio);

	viewport.setRect((double) (width() - s.width()) / 2,
					 (double) (height() - s.height()) / 2,
					  s.width(), s.height() );

	double sc = (double) viewport.width() / window.width();

	transform.reset();
	transform.translate(viewport.x(), viewport.y());
	transform.scale(sc, sc);
	transform.translate( -window.x(),  -window.y());
}

QPoint Viewer::map(const QPoint &p) const
{
	return transform.map(p);
}

QPoint Viewer::unmap(const QPoint &p) const
{
	return transform.inverted().map(p);
}
