#ifndef EDGEDETECT_H
#define EDGEDETECT_H

#include "filter.h"

class EdgeDetect : public Filter
{
	public:
		EdgeDetect(double t, double r);

		QString getName() const { return "EdgeDetect"; }

	protected:
		Result * applyInternal(Image *img);

		double threshold;
		double ratio;
};

#endif // EDGEDETECT_H
