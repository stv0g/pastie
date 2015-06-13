#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

#include "filter.h"

class Homography : public Filter
{
	Q_OBJECT

	public:
		Homography(const Mat &h, int inter);
		~Homography();

		QString getName() const { return "Homography"; }

	protected:
		Result * applyInternal(Image *img);

		double h[9];
		int interpolation;
		bool inverse;
};

#endif // HOMOGRAPHY_H
