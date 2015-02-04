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

#ifndef BLUR_H
#define BLUR_H

#include "filter.h"

class Blur : public Filter
{
	public:
		enum Type {
			BILATERAL,
			BILATERAL_ADAPTIVE,
			BOX,
			BOX_NORMALIZED,
			GAUSSIAN,
			MEDIAN
		};

		Blur(enum Type t, Size ks, double sp = 10, double clr = 20, double sigma = 2, int bdr = BORDER_DEFAULT);

		QString getName() const { return "Blur"; }

	protected:
		Result * applyInternal(Image *img);

		enum Blur::Type type;

		Size size;
		Point anchor = Point(-1, -1);
		int border;

		double sigma;
		double color;
		double space;
};

#endif // BLUR_H
