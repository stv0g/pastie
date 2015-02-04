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
