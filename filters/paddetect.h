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

#ifndef PADDETECT_H
#define PADDETECT_H

#include <QLinkedList>

#include "pad.h"
#include "filter.h"

using namespace std;

class PadDetect : public Filter
{
	public:
		enum Mode {
			AREA,
			COSINE
		};

		PadDetect(double epsilon = .1, double cosine = 0.3);
		QString getName() const { return "PadDetect"; };

	protected:
		Result * applyInternal(Image *img);

		double epsilon;
		double areaRatio;
		double cosine;
		enum Mode mode;
};

class PadResult : public Result, public QLinkedList<Pad>
{
	public:
		void drawResult(Painter *p) const;

		QString getResult() const;

		iterator getNearest(const Point2f &p);
		iterator getNearest(const Pad &p);

};

#endif // PADDETECT_H
