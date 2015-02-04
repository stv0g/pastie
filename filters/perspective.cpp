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

#include <opencv2/imgproc.hpp>

#include <QtDebug>

#include "cast.h"
#include "perspective.h"

Perspective::Perspective(Camera *c, Pattern *p) :
	alpha(1),
	cam(c),
	pattern(p)
{
	settings["Scale"] = new DoubleSetting(this, alpha, Range<double>(0.5, 2));
}

Result * Perspective::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	PatternResult *presult = (PatternResult *) img->getResult(pattern);

	if (presult && presult->isFound()) {
		Mat map;
		std::vector<Point2f> src, dest;
		std::vector<Point2f> srcPoints  = presult->getPoints();
		std::vector<Point3f> destPoints = pattern->getPoints();

		for (int i : pattern->getCornerIndizes()) {
			src.push_back(srcPoints[i]);
			dest.push_back(Point2f(destPoints[i].x, destPoints[i].y));
		}

		double scale = alpha * norm(src[1] - src[0]) / norm(dest[1] - dest[0]);

		for (Point2f &p : dest)
			p *= scale;

		Size newSize(dest[3].x, dest[3].y);

		map = getPerspectiveTransform(src, dest);
		warpPerspective(m, m, map, newSize);

		return new Result(toQTransform(map));
	}

	return NULL;
}

