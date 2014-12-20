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
			ASYMMETRIC_CIRCLES_GRID
		} type;

		Pattern(Size si, double sp, Type t = Type::CHESSBOARD);

		Result * applyInternal(Image *img);

		/* Getter */
		QString				 getName() const { return "Pattern"; }
		std::vector<int>	 getCornerIndizes() const;
		std::vector<Point3f> getPoints() const;

	protected:
		Size size;
		int flags = CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK;
		double spacing;
};

class PatternResult : public Result
{
	public:
		PatternResult(bool found, std::vector<Point2f> op);

		void draw(Painter *p) const;
		QString getResult() const;

		/* Getter */
		std::vector<Point2f> getPoints() const { return points; }
		bool isFound() const { return found; }

	protected:
		bool found;
		std::vector<Point2f> points;
};

#endif // PATTERN_H
