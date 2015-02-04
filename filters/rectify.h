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

#ifndef RECTIFY_H
#define RECTIFY_H

#include "filter.h"

class Rectify : public Filter
{
	public:
		Rectify(double threshold);

		QString getName() const { return "Rotate"; }

	protected:
		double threshold;
		int thresholdHough;

		Result * applyInternal(Image *img);

		double getHoughRotation(Mat &m);
		double mapQuadrant(double angle);

};

#endif // RECTIFY_H
