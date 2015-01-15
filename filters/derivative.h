#ifndef DERIVATIVE_H
#define DERIVATIVE_H

#include "filter.h"

class Derivative : public Filter
{
	public:
		enum Type {
			SOBEL,
			SCHARR
		};

		Derivative(enum Type t, Size order = Size(1, 1), int size = 3);
		QString getName() const { return "Derivative"; }

	protected:

		Result * applyInternal(Image *img);

		enum Type type;

		Size order;
		int size;
		double scale;
		double delta;
		bool maxima;
};

#endif // DERIVATIVE_H
