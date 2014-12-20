#ifndef SHAPEDETECT_H
#define SHAPEDETECT_H

#include <vector>

#include "filter.h"

using namespace std;

class ShapeDetect : public Filter
{
	public:
		enum Mode {
			AREA,
			COSINE
		};

		ShapeDetect(double epsilon = 1, double cosine = 0.3);
		QString getName() const { return "ShapeDetect"; };

	protected:
		Result * applyInternal(Image *img);

		double epsilon;
		double cosine;
		int shape = 0;
		enum Mode mode;
};

class ShapeResult : public Result
{
	public:
		void draw(Painter *p) const;

		QString getResult() const;

		QList<RotatedRect> rects;
};

#endif // SHAPEDETECT_H
