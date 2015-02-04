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

#ifndef PAINTER_H
#define PAINTER_H

#include <QPainter>

#include "image.h"
#include "cast.h"
#include "pad.h"

class Painter : public QPainter
{
	public:
		Painter(QPaintDevice *dev);

		void drawOverlay(Image *p);

		void drawMarker(const QPoint &center, int radius = 8);
		void drawPad(const Pad &pad);

		/* Proxy to adjust width */
		void setPen(const QPen &pen);

		double getRatio() const { return ratio; }
		void setRatio(double r) { ratio = r; }

	protected:
		double ratio;
};

#endif // PAINTER_H
