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

#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "filter.h"

using namespace std;

class Threshold : public Filter
{
	public:
		enum Type {
			ADAPTIVE_GAUSS,
			ADAPTIVE_MEAN,
			OTSU,
			TRIANGLE,
			FIXED
		};

		Threshold(enum Threshold::Type t, double v = 0, int sz = 7, double c = 100);

		QString getName() const { return "Threshold"; }

	protected:
		Result * applyInternal(Image *img);

		enum Threshold::Type type;
		int method;
		bool invert = false;
		int size;
		double value, c;
};

#endif // THRESHOLD_H
