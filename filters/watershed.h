#ifndef WATERSHED_H
#define WATERSHED_H

#include "filter.h"

class Watershed : public Filter
{
	public:
		Watershed();

		QString getName() const { return "Watershed"; }

	protected:
		Result * applyInternal(Image *img);

};

#endif // WATERSHED_H
