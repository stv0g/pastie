#ifndef RESIZE_H
#define RESIZE_H

#include "filter.h"

class Resize : public Filter
{
	public:
		Resize(Range<int> r = Range<int>(320, 1024), int inter = INTER_LINEAR);

		QString getName() const { return "Resize"; }

	protected:
		Range<int> range;
		int interpolation;

		Result * applyInternal(Image *img);
};

#endif // RESIZE_H
