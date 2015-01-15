#ifndef CAST_H
#define CAST_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <QtDebug>

#include <QTransform>
#include <QSize>
#include <QPoint>
#include <QRect>
#include <QImage>

using namespace cv;

inline QImage toQImage(const Mat &mat)
{
	Mat *conv = new Mat;

	switch (mat.channels()) {
		case 3: cvtColor(mat, *conv, COLOR_RGB2RGBA);  break;
		case 1: cvtColor(mat, *conv, COLOR_GRAY2RGBA); break;
	}

	return QImage((const unsigned char *) conv->data, conv->cols, conv->rows, conv->step,
		QImage::Format_RGB32, [](void * data) { ((Mat *) data)->release(); }, conv);
}

inline QTransform toQTransform(const Mat &m)
{
	if (m.size() == Size(3, 3))
		return QTransform( m.at<float>(0,0), m.at<float>(0,1), m.at<float>(0,2),
						   m.at<float>(1,0), m.at<float>(1,1), m.at<float>(1,2),
						   m.at<float>(2,0), m.at<float>(2,1), m.at<float>(2,2) );
	else if (m.size() == Size(2, 3))
		return QTransform( m.at<float>(0,0), m.at<float>(0,1),
						   m.at<float>(1,0), m.at<float>(1,1),
						   m.at<float>(0,2), m.at<float>(1,2) );
	else if (m.size() == Size(2, 2))
		return QTransform( m.at<float>(0,0), m.at<float>(0,1),
						   m.at<float>(1,0), m.at<float>(1,1), 0, 0 );
	else
		return QTransform();
}

inline QSize	toQt(const Size2i &s)	{ return QSize (s.width, s.height); }
inline QSizeF	toQt(const Size2f &s)	{ return QSizeF(s.width, s.height); }

inline QPoint	toQt(const Point2i &p)	{ return QPoint (p.x, p.y); }
inline QPointF	toQt(const Point2f &p)	{ return QPointF(p.x, p.y); }

inline QRect	toQt(const Rect2i &r)	{ return QRect (r.x, r.y, r.width, r.height); }
inline QRectF	toQt(const Rect2f &r)	{ return QRectF(r.x, r.y, r.width, r.height); }

inline Size2i	toCv(const QSize  &s)	{ return Size2i(s.width(), s.height()); }
inline Size2d	toCv(const QSizeF &s)	{ return Size2d(s.width(), s.height()); }

inline Point2i	toCv(const QPoint  &p)	{ return Point2i(p.x(), p.y()); }
inline Point2d	toCv(const QPointF &p)	{ return Point2d(p.x(), p.y()); }

inline Rect2i	toCv(const QRect  &r)	{ return Rect2i(r.x(), r.y(), r.width(), r.height()); }
inline Rect2d	toCv(const QRectF &r)	{ return Rect2d(r.x(), r.y(), r.width(), r.height()); }

#endif // CAST_H
