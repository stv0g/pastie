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

#include "homography.h"

Homography::Homography(const Mat &ho, int i = INTER_LINEAR) :
	interpolation(i)
{
	QMap<int, QString> interMap;
	interMap[INTER_NEAREST]		= "Nearest Neighbour";
	interMap[INTER_LINEAR]		= "Linear";
	interMap[INTER_AREA]		= "Pixel Area Relation";
	interMap[INTER_CUBIC]		= "Bi-Cubic (4x4)";
	interMap[INTER_LANCZOS4]	= "Lanczos4 (8x8)";

	settings["Inverse"] = new BooleanSetting(this, inverse, "Invert the homography before transformation");
	settings["Interpolation"] = new EnumSetting(this, (int &) interpolation, interMap);

	for (int i=0; i<9; i++) {
		h[i] = ho.at<double>(i);
		settings["h[" + QString::number(i) + "]"] = new DoubleSetting(this, h[i], Range<double>(-2, 2));
	}
}

Result * Homography::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	Mat ho(3, 3, CV_64F);

	for (int i=0; i<9; i++)
		ho.at<double>(i) = h[i];

	warpPerspective(m, m, ho, m.size(), interpolation);

	return new Result(toQTransform(ho));
}

Homography::~Homography()
{

}

