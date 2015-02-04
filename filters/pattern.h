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

#ifndef PATTERN_H
#define PATTERN_H

#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

#include "filter.h"

using namespace cv;

class Image;
class PatternResult;

class Pattern : public Filter
{
	public:
		enum Type {
			CHESSBOARD,
			CIRCLES_GRID,
			ASYMMETRIC_CIRCLES_GRID,
			QUADRILINEAR_MARKERS
		} type;

		Pattern(Size si, int sp, Type t = Type::CHESSBOARD);
		Pattern(Size si, Size sp, Type t = Type::CHESSBOARD);

		Result * applyInternal(Image *img);

		/* Getter */
		QString				 getName() const { return "Pattern"; }
		std::vector<int>	 getCornerIndizes() const;
		std::vector<Point3f> getPoints() const;

	protected:
		Size size;
		Size spacing;
		int flags = CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK;

		/* QUADRILINEAR Settings */
		Range<double> areaRange;
		Range<double> ratioRange;
		bool showThresh;
};

class PatternResult : public Result
{
	public:
		PatternResult(bool found, std::vector<Point2f> op);

		void drawResult(Painter *p) const;
		QString getResult() const;

		/* Getter */
		std::vector<Point2f> getPoints() const { return points; }
		bool isFound() const { return found; }

	protected:
		bool found;
		std::vector<Point2f> points;
};

#endif // PATTERN_H
