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

		Image *img;

		QVector<Point> clicks;

	protected:
		QPoint transform(QPoint pos);
		QPoint transformInv(QPoint pos);

		void paintEvent(QPaintEvent *pe);
		void resizeEvent(QResizeEvent *re);
		void mouseReleaseEvent(QMouseEvent *me);

		QImage qimg;
		QRect viewport;

		Size size;
		double ratio;

	signals:
		void clicked(QPoint pos);

	public slots:
		void showImage(Image *img);
		void updateImage();
		void updateViewport();
};

#endif // VIEWER_H
