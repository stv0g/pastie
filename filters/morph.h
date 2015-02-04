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

#ifndef MORPH_H
#define MORPH_H

#include "filter.h"

class Morph : public Filter
{
	public:
		Morph(int op, int shape, Size size = Size(3, 3), Point anchor = Point(-1, -1), int iterations = 1);
		QString getName() const { return "Morph"; }

	public slots:
		void reset();

	protected:
		Result * applyInternal(Image *img);

		Mat kernel;

		int op;
		int shape;
		int iterations;
		Size size;
		Point anchor;
};

#endif // MORPH_H
