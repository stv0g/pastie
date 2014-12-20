#include <QDir>
#include <QtDebug>

#include <opencv2/core.hpp>

#include "source.h"

using namespace cv;

Source::Source() :
	VideoCapture(CAP_ANY)
{
	if (!isOpened())
		throw Exception();

	setSize(Size(640, 480));
	setFPS(15);

	connect(&tmr, SIGNAL(timeout()), this, SLOT(tick()));
}

Size Source::getSize()
{
	return Size(
		get(CAP_PROP_FRAME_WIDTH),
		get(CAP_PROP_FRAME_HEIGHT)
	);
}

bool Source::setSize(const Size s)
{
		   set(CAP_PROP_FRAME_WIDTH,  s.width);
	return set(CAP_PROP_FRAME_HEIGHT, s.height);
}

double Source::getFPS()
{
	return fps;
}

bool Source::setFPS(double f)
{
	fps = f;

	if (tmr.isActive())
		tmr.start(1000.0 / fps);

	return set(CAP_PROP_FPS, fps);
}

Image * Source::getSnapshot()
{
	static unsigned no;

	Mat mat;
	QString fileName = QString("%1/snapshot_%2.png")
			.arg(QDir::tempPath()).arg(no++);

	read(mat);

	return new Image(mat, fileName);
}

void Source::play(bool run)
{
	if (run)
		tmr.start(1000.0 / fps);
	else
		tmr.stop();
}

void Source::tick()
{
	static bool running;

	if (running)
		return;

	running = true;

	if (read(last.original))
		emit newImage(&last);

	running = false;
}

