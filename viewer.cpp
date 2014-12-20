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

void Viewer::resizeEvent(QResizeEvent *)
{
	updateViewport();
}

void Viewer::paintEvent(QPaintEvent *)
{
	Painter p(this, 1 / ratio);

	p.fillRect(rect(), Qt::black);

	if (!img)
		return;

	p.setViewport(viewport);
	p.setWindow(qimg.rect());

	p.drawImage(p.window(), qimg);

	QPen pen = p.pen();
	pen.setColor(Qt::green);
	pen.setWidth(2 / ratio);
	p.setPen(pen);

	for (auto filter : *filters) {
		Result *result = img->getResult(filter);

		if (filter->isShown() && result)
			result->draw(&p);
	}
}

void Viewer::mouseReleaseEvent(QMouseEvent *me)
{
	if (viewport.contains(me->pos())) {
		QPoint pos = transformInv(me->pos());

		qDebug() << "Clicked at: " << pos.x() << "," << pos.y();

		clicks.push_back(Point(pos.x(), pos.y()));
		emit clicked(pos);
	}
}

QPoint Viewer::transform(QPoint pos)
{
	return pos * ratio + viewport.topLeft();
}

QPoint Viewer::transformInv(QPoint pos)
{
	return (pos - viewport.topLeft()) / ratio;
}

void Viewer::showImage(Image *next)
{
	if (!next)
		return;

	if (next != img)
		filters->reset();

	img = next;
	updateImage();
}

void Viewer::updateImage()
{
	if (!img)
		return;

	filters->execute(img);

	qimg = toQImage(img->filtered);

	if (size != img->filtered.size()) {
		size = img->filtered.size();
		updateViewport();
	}

	update();
}

void Viewer::updateViewport()
{
	double viewRatio = (double) size.width / size.height;

	int viewHeight = width() / viewRatio;
	int viewWidth = width();

	if (viewHeight > height()) {
		viewWidth = height() * viewRatio;
		viewHeight = height();
	}

	viewport = QRect(
					(width() - viewWidth) / 2,
					(height() - viewHeight) / 2,
					viewWidth,
					viewHeight
			   );

	qDebug() << "New viewport" << viewport << " in widget " << rect() << " for frame " << toQt(size);

	ratio = (double) viewport.width() / size.width;
}
