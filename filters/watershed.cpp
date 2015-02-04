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

#include "watershed.h"

void Watershed::clicked(Point pos, QMouseEvent *me)
{
	qDebug() << getName() << ": Marker " << ((me->modifiers() & Qt::AltModifier) ? "deleted" : "added") << " at:" << toQt(pos);

	if (me && me->modifiers() & Qt::AltModifier)
		delMarkers.push_back(pos);
	else
		addMarkers.push_back(pos);

	emit filterChanged(this);
}

void Watershed::reset()
{
	qDebug() << getName() << ": Markers cleared";

	delMarkers.clear();
	addMarkers.clear();
}

Result * Watershed::applyInternal(Image *img)
{
	Mat dist, dist_8u, &m = img->getMat();
	Mat kernel, max, mask;

	CV_Assert(m.channels() == 1);

	distanceTransform(m, dist, DIST_L2, 3);

	normalize(dist, dist, 0, 0xff, NORM_MINMAX);
	dist.convertTo(dist_8u, CV_8U);

	kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	dilate(dist_8u, max, kernel);

	mask = (dist_8u > 0);
	m = (max == dist_8u);
	m &= mask;

	return NULL;
}

