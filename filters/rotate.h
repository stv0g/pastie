#ifndef ROTATE_H
#define ROTATE_H

#include "filter.h"

class Rotate : public Filter
{
	public:
		Rotate(double a, int inter, double scale = 1.0);

		QString getName() const { return "Rotate"; }

	protected:
		Result * applyInternal(Image *img);

		double angle;
		int interpolation;
		double scale;
};

#endif // ROTATE_H
