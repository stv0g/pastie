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

#include "blur.h"

Blur::Blur(enum Type t, Size ks, double sp, double clr, double si, int bdr) :
	type(t),
	size(ks),
	border(bdr),
	sigma(si),
	color(clr),
	space(sp)
{
	QMap<int, QString> mapType;
	QMap<int, QString> mapBorder;

	mapType[BILATERAL]			= "Bilateral";
	//mapType[BILATERAL_ADAPTIVE] = "Adaptive Bilateral";
	mapType[BOX]				= "Box";
	mapType[BOX_NORMALIZED]		= "Box Normalized";
	mapType[GAUSSIAN]			= "Gaussian";
	mapType[MEDIAN]				= "Median";

	mapBorder[BORDER_DEFAULT]	= "Default";
	//mapBorder[BORDER_CONSTANT]	= "Constant";
	//mapBorder[BORDER_REPLICATE] = "Replicate";
	mapBorder[BORDER_ISOLATED]	= "Isolated";
	mapBorder[BORDER_REFLECT]	= "Reflect";
	mapBorder[BORDER_REFLECT_101] = "Reflect 101";
	mapBorder[BORDER_WRAP]		= "Wrap";

	settings["Type"]		= new EnumSetting(this, (int&) type, mapType);
	settings["Border"]		= new EnumSetting(this, (int&) border, mapBorder);
	settings["Kernel Size"] = new SizeSetting(this, size);
	settings["Space"]		= new DoubleSetting(this, space);
	settings["Color"]		= new DoubleSetting(this, color);
	settings["Sigma"]		= new DoubleSetting(this, sigma);
}

Result * Blur::applyInternal(Image *img)
{
	Mat &n = img->getMat();
	Mat m = n.clone();

	switch (type) {
		case BILATERAL:				bilateralFilter(m, n, -1, space, color); break;
		case BILATERAL_ADAPTIVE:	/*adaptiveBilateralFilter(m, n, size, space, color);*/ break;
		case BOX:					boxFilter(m, n, -1, size, anchor, true, border); break;
		case BOX_NORMALIZED:		blur(m, n, size, anchor, border); break;
		case GAUSSIAN:				GaussianBlur(m, n, size, sigma, 0, border); break;
		case MEDIAN:				medianBlur(m, n, size.width); break;
	}

	return new Result;
}

