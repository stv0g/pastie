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

#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/videoio.hpp>

#include <list>

#include "source.h"
#include "filters/pattern.h"

using namespace cv;
using namespace std;

class Image;

class Camera
{
	public:
		Camera(Source *s);

		bool calibrate(QList<Image *> imgs, Pattern *pattern);

		/* Getters */
		const Mat & getDistCoeffs() { return distCoeffs; }
		const Mat & getMatrix() { return matrix; }
		double getAvgReprErr() { return avgReprErr; }
		bool isCalibrated() { return calibrated; }

		void reset();

		Source *source;

	protected:
		bool calibrated;
		double avgReprErr;

		Mat matrix;
		Mat distCoeffs;
};

#endif // CAMERA_H
