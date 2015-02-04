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
 * @copyright  2015 Steffen Vogel
 * @license	   http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author     Steffen Vogel <steffen.vogel@rwth-aachen.de>
 * @link       http://www.steffenvogel.de
 */

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
		enum MouseMode {
			MOUSE_NONE,
			MOUSE_ZOOMING,
			MOUSE_DRAGGING
		} mouseMode;

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

		void reset();
};

#endif // VIEWER_H
