/*
 * This file is part of Pastie
 *
 * Pastie is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Pastie is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pastie. If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @copyright  Kitware Inc.
 * @license	   http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author     Steffen Vogel <steffen.vogel@rwth-aachen.de>
 * @link       http://www.steffenvogel.de
 * @package    CTK
 */

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
	mouseMode(MOUSE_NONE),
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

	p.fillRect(rect(), Qt::black);
	p.setClipRect(viewport);
	p.setTransform(transform);
	p.setRatio(1 / transform.m11());

	pe.setStyle(Qt::SolidLine);
	pe.setWidth(2);
	p.setPen(pe);

	if (img) {
		p.drawImage(window, qimg, window);

		p.drawOverlay(img);
	}

	if (mouseMode == MOUSE_ZOOMING) {
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
	if (mouseMode != MOUSE_NONE) {
		we->ignore();
		return;
	}

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
		first = me->pos();
		last = me->pos();

		if (me->modifiers() & Qt::AltModifier)
			mouseMode = MOUSE_ZOOMING;
		else
			mouseMode = MOUSE_DRAGGING;
	}
}

void Viewer::mouseMoveEvent(QMouseEvent *me)
{
	if (viewport.contains(me->pos())) {
		if (mouseMode == MOUSE_ZOOMING)
		{ } /* draw zoom rectangle */
		else if (mouseMode == MOUSE_DRAGGING) {
			QPoint delta = (me->pos() - last) / transform.m11();
			window.moveCenter(window.center() - delta);
			updateWindow(window);
		}

		last = me->pos();
	}

	update();
}

void Viewer::mouseReleaseEvent(QMouseEvent *me)
{
	if (viewport.contains(me->pos())) {
		if (mouseMode == MOUSE_ZOOMING) {
			if (!first.isNull() && !last.isNull()) {
				updateWindow(QRect(unmap(first), unmap(last)).normalized());

				first = QPoint();
				last = QPoint();
			}
		}
		else if (mouseMode == MOUSE_DRAGGING) {
			QPoint pos = unmap(me->pos());
			Filter *f = filters->getCurrent();

			if (f) {
				qDebug() << "Clicked at: " << pos << "(" << me->pos() << ") << for filer " << f->getName();
				f->clicked(toCv(pos), me);
			}
		}

		mouseMode = MOUSE_NONE;
		first = last = QPoint();
	}

	update();
}

void Viewer::showImage(Image *next)
{
	img = next;
	qimg = img->getQImage();

	if (window.size() != qimg.size())
		updateWindow(QRect(QPoint(), qimg.size()));

	update();
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
