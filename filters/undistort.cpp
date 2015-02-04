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

#include "undistort.h"

Undistort::Undistort(Camera *c) :
	cam(c)
{
    reset();
}

void Undistort::reset()
{
	Mat newCamMat = getOptimalNewCameraMatrix(cam->getMatrix(), cam->getDistCoeffs(), cam->source->getSize(), 1);

    initUndistortRectifyMap(cam->getMatrix(), cam->getDistCoeffs(),
							Mat(), newCamMat, cam->source->getSize(), CV_16SC2, map1, map2);
}

Result * Undistort::applyInternal(Image *img)
{
	Mat &m = img->getMat();

	if (cam->isCalibrated()) {
#if 1
		remap(m, m, map1, map2, INTER_LINEAR);
#else
        Mat dest;
		undistort(m, dest, cam->getMatrix(), cam->getDistCoeffs());
		m = dest;
#endif
    }

	return NULL;
}
