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

#include "derivative.h"

Derivative::Derivative(Type t, Size o, int s) :
	type(t),
	order(o),
	size(s),
	maxima(false)
{
	QMap<int, QString> typeMap;
	typeMap[SOBEL] = "Sobel";
	typeMap[SCHARR] = "Scharr";

	settings["Type"]	= new EnumSetting(this, (int&) type, typeMap);
	settings["Order"]	= new SizeSetting(this, order);
	settings["Size"]	= new IntegerSetting(this, size, Range<int>(1, 10));
	settings["Maxima"]	= new BooleanSetting(this, maxima, "Find local maximas in source image.");
	settings["Scale"]	= new DoubleSetting(this, scale);
	settings["Delta"]	= new DoubleSetting(this, delta);
}

Result * Derivative::applyInternal(Image *img)
{
	Mat &n = img->getMat();

	switch (type) {
		case SOBEL:	 Sobel(n, n, n.depth(), order.width, order.height, size, scale, delta); break;
		case SCHARR: Scharr(n, n, n.depth(), order.width, order.height, scale, delta); break;
	}

	if (maxima)
		compare(n, 0, n, CMP_EQ);

	return new Result;
}

