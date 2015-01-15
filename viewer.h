#ifndef VIEWER_H
#define VIEWER_H

#include <QGLWidget>
#include <QImage>
#include <QVector>

#include <opencv2/core.hpp>

#include "image.h"

using namespace cv;

class Viewer : public QGLWidget
{
		Q_OBJECT
	public:
		explicit Viewer(QWidget *parent = 0);

	protected:
		void paintEvent(QPaintEvent *pe);
		void resizeEvent(QResizeEvent *re);

		void wheelEvent(QWheelEvent *we);
		void mousePressEvent(QMouseEvent *me);
		void mouseMoveEvent(QMouseEvent *me);
		void mouseReleaseEvent(QMouseEvent *me);

		QPoint map(const QPoint &p) const;
		QPoint unmap(const QPoint &p) const;

		void updateTransform();
		void updateWindow(const QRect &r);

		Image *img;
		QImage qimg;
		QRect viewport;
		QRect window;
		QPoint first, last;
		QTransform transform;

	public slots:
		void showImage(Image *img);
		void updateImage();

		void reset();
};

#endif // VIEWER_H
