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

#include "resize.h"

Resize::Resize(Range<int> r, int i) :
	range(r),
	interpolation(i)
{
	QMap<int, QString> interMap;
	interMap[INTER_NEAREST]		= "Nearest Neighbour";
	interMap[INTER_LINEAR]		= "Linear";
	interMap[INTER_AREA]		= "Pixel Area Relation";
	interMap[INTER_CUBIC]		= "Bi-Cubic (4x4)";
	interMap[INTER_LANCZOS4]	= "Lanczos4 (8x8)";

	settings["Range"] = new RangeSetting(this, range, Range<int>(200, 3000));
	settings["Interpolation"] = new EnumSetting(this, (int &) interpolation, interMap);
}

Result * Resize::applyInternal(Image *img)
{
	Mat &m = img->getMat();

	if (!range.contains(m.cols)) {
		double f = 1;

		if (m.cols < range.min)
			f = (double) range.min / m.cols;
		else if (m.cols > range.max)
			f = (double) range.max / m.cols;

		resize(m, m, Size(), f, f, interpolation);

		return new Result(QTransform::fromScale(f, f));
	}
	else
		return new Result;
}
