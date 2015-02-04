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

#include "convert.h"

Convert::Convert(int c) :
	Filter(),
	code(c)
{
	QMap<int, QString> mapCode;
	mapCode[COLOR_BGR2HSV]	= "BGR to HSV";
	mapCode[COLOR_BGR2GRAY] = "BGR to Gray";
	mapCode[COLOR_BGR2Lab]  = "BGR to Lab";
	mapCode[COLOR_BGR2Luv]	= "BGR to Luv";
	mapCode[COLOR_BGR2HLS]  = "BGR to HLS";
	mapCode[COLOR_BGR2YUV]  = "BGR to YUV";
	mapCode[COLOR_BGR2YCrCb]= "BGR to YCrCb";
	// FIXME add more codes

	settings["Code"] = new EnumSetting(this, code, mapCode);
}

Result * Convert::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	Mat n = m.clone();

	cvtColor(n, m, code);

	return new Result;
}

