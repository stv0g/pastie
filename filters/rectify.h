#ifndef RECTIFY_H
#define RECTIFY_H

#include "filter.h"

class Rectify : public Filter
{
	public:
		Rectify(double threshold);

		QString getName() const { return "Rotate"; }

	protected:
		double threshold;
		int thresholdHough;

		Result * applyInternal(Image *img);

		double getHoughRotation(Mat &m);
		double mapQuadrant(double angle);

};

#endif // RECTIFY_H
