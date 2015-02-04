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
	Mat m;

	if (read(m)) {
		last.setSourceMat(m);
		emit newFrame(&last);
	}
}

