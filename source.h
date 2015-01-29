#ifndef SOURCE_H
#define SOURCE_H

#include <QObject>
#include <QTimer>

#include <opencv2/videoio.hpp>

#include "image.h"

using namespace cv;

class Source :
	public QObject,
	public VideoCapture
{
	Q_OBJECT

	public:
		Source();

		Size getSize();
		bool setSize(const Size s);

		double getFPS();
		bool setFPS(double f);

		Image * getSnapshot();

	public slots:
		void play(bool);

	signals:
		void newFrame(Image *img);

	protected slots:
		void tick();

	protected:
		Image last;
		QTimer tmr;

		double fps;
};

#endif // SOURCE_H
