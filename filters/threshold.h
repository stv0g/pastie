#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "filter.h"

using namespace std;

class Threshold : public Filter
{
	public:
		enum Type {
			ADAPTIVE_GAUSS,
			ADAPTIVE_MEAN,
			OTSU,
			TRIANGLE,
			FIXED
		};

		Threshold(enum Threshold::Type t, double v = 0, int sz = 7, double c = 100);

		QString getName() const { return "Threshold"; }

	protected:
		Result * applyInternal(Image *img);

		enum Threshold::Type type;
		bool invert = false;
		int size;
		double value, c;
};

#endif // THRESHOLD_H
