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

#include "rotate.h"

Rotate::Rotate(double a, int i = INTER_LINEAR, double s) :
	angle(a),
	interpolation(i),
	scale(s)
{
	QMap<int, QString> interMap;
	interMap[INTER_NEAREST]		= "Nearest Neighbour";
	interMap[INTER_LINEAR]		= "Linear";
	interMap[INTER_AREA]		= "Pixel Area Relation";
	interMap[INTER_CUBIC]		= "Bi-Cubic (4x4)";
	interMap[INTER_LANCZOS4]	= "Lanczos4 (8x8)";

	settings["Angle"] = new DoubleSetting(this, angle, Range<double>(0, 360));
	settings["Scale"] = new DoubleSetting(this, scale, Range<double>(0.5, 2));
	settings["Interpolation"] = new EnumSetting(this, (int &) interpolation, interMap);
}

Result * Rotate::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	Mat affine = getRotationMatrix2D(Point2f(m.cols/2, m.rows/2), angle, 1);

	warpAffine(m, m, affine, m.size(), interpolation);

	return new Result(toQTransform(affine));
}
