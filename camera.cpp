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

#include <opencv2/calib3d.hpp>

#include <QtDebug>

#include "filters/pattern.h"
#include "camera.h"
#include "image.h"

Camera::Camera(Source *s) :
	source(s),
	calibrated(false)
{
	reset();
}

void Camera::reset()
{
	calibrated = false;
	avgReprErr = 0;
	distCoeffs = Mat::zeros(8, 1, CV_32F);
	matrix = Mat::eye(3, 3, CV_32F);
}

bool Camera::calibrate(QList<Image *> imgs, Pattern *pattern)
{	
	vector<vector<Point2f>> imagePoints;
	vector<vector<Point3f>> objectPoints;
	vector<Mat> rvecs, tvecs;

	if (calibrated) {
		qWarning() << "Camera is already calibrated. Reset first!";
		return false;
	}

	for (Image *img : imgs) {
		PatternResult *result = (PatternResult*) pattern->apply(img);

		img->results[pattern] = result;

		if (result->isFound())
			imagePoints.push_back(result->getPoints());
	}

	if (imagePoints.size() < 5) {
		qWarning() << "Only " << imagePoints.size() << " patterns detected. This is not enough!";
		return false;
	}

	/* Fill in vector of calibration rig points */
	std::vector<Point3f> planar = pattern->getPoints();
	for (unsigned i = 0; i < imagePoints.size(); i++)
		objectPoints.push_back(planar);

	/* Calculate camera matrix and distortion coefficients */
	calibrateCamera(objectPoints, imagePoints, source->getSize(), matrix, distCoeffs, rvecs, tvecs);

	/* Calculate average reprojection error */
	double totalErr = 0;
	int totalPoints = 0;

	for (unsigned i = 0; i < imagePoints.size(); i++) {
		std::vector<Point2f> imagePoints2;

		projectPoints(objectPoints[i], rvecs[i], tvecs[i], matrix, distCoeffs, imagePoints2);

		totalErr += pow(norm(imagePoints[i], imagePoints2, NORM_L2), 2);
		totalPoints += objectPoints[i].size();
	}

	avgReprErr = totalErr / totalPoints;
	calibrated = true;

	qDebug() << "Calibration completed: Found " << imagePoints.size() << " patterns in " << imgs.size() << " images";

	return true;
}

