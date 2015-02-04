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

#include "maxchannel.h"

Result * MaxChannel::applyInternal(Image *img)
{
	Mat &m = img->getMat();

	CV_Assert(m.type() == CV_8UC3);

	for(int i = 0; i < m.rows; i++) {
		uchar *p = m.ptr<uchar>(i);
		for (int j = 0; j < 3*m.cols; j += 3) {
			p[j]	= (p[j  ] > p[j+1] && p[j  ] > p[j+2]) ? 0xff : 0;
			p[j+1]  = (p[j+1] > p[j  ] && p[j+1] > p[j+2]) ? 0xff : 0;
			p[j+2]	= (p[j+2] > p[j  ] && p[j+2] > p[j+2]) ? 0xff : 0;
		}
	}

	return new Result;
}

