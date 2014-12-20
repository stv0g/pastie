#ifndef BLUR_H
#define BLUR_H

#include "filter.h"

class Blur : public Filter
{
	public:
		enum Type {
			BILATERAL,
			BILATERAL_ADAPTIVE,
			BOX,
			BOX_NORMALIZED,
			GAUSSIAN,
			MEDIAN
		};

		Blur(enum Type t, Size ks, double sp = 10, double clr = 20, double sigma = 2, int bdr = BORDER_DEFAULT);

		QString getName() const { return "Blur"; }

	protected:
		Result * applyInternal(Image *img);

		enum Blur::Type type;

		Size size;
		Point anchor = Point(-1, -1);
		int border;

		double sigma;
		double color;
		double space;
};

#endif // BLUR_H
