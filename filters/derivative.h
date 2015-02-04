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

#ifndef DERIVATIVE_H
#define DERIVATIVE_H

#include "filter.h"

class Derivative : public Filter
{
	public:
		enum Type {
			SOBEL,
			SCHARR
		};

		Derivative(enum Type t, Size order = Size(1, 1), int size = 3);
		QString getName() const { return "Derivative"; }

	protected:

		Result * applyInternal(Image *img);

		enum Type type;

		Size order;
		int size;
		double scale;
		double delta;
		bool maxima;
};

#endif // DERIVATIVE_H
