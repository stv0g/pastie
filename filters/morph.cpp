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

#include "morph.h"

Morph::Morph(int o, int t, Size s, Point a, int i) :
	op(o),
	shape(t),
	iterations(i),
	size(s),
	anchor(a)
{
	QMap<int, QString> mapOp;
	QMap<int, QString> mapShape;

	mapOp[MORPH_OPEN]		= "Open";
	mapOp[MORPH_CLOSE]		= "Close";
	mapOp[MORPH_ERODE]		= "Erode";
	mapOp[MORPH_DILATE]		= "Dilate";
	mapOp[MORPH_BLACKHAT]	= "Blackhat";
	mapOp[MORPH_GRADIENT]	= "Gradient";

	mapShape[MORPH_RECT]	= "Rectangle";
	mapShape[MORPH_ELLIPSE]	= "Ellipse";
	mapShape[MORPH_CROSS]	= "Cross";

	settings["Iterations"]	= new IntegerSetting(this, iterations);
	settings["Operation"]	= new EnumSetting(this, op, mapOp);
	settings["Shape"]		= new EnumSetting(this, shape, mapShape);
	settings["Size"]		= new SizeSetting(this, size, Range<int>(1, 32));

	reset();
}

void Morph::reset()
{
	kernel = getStructuringElement(shape, size);
}

Result * Morph::applyInternal(Image *img)
{
	Mat &m = img->getMat();

	switch (op) {
		case MORPH_DILATE:		dilate(m, m, kernel, anchor, iterations); break;
		case MORPH_ERODE:		 erode(m, m, kernel, anchor, iterations); break;
		case MORPH_OPEN:
		case MORPH_CLOSE:
		case MORPH_BLACKHAT:
		case MORPH_GRADIENT:
					  morphologyEx(m, m, op, kernel, anchor, iterations); break;
		default:
			CV_Error(100, "Invalid morph operation");
	}

	return new Result;
}
