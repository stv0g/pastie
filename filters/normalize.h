#ifndef NORMALIZE_H
#define NORMALIZE_H

#include "filter.h"

class Normalize : public Filter
{
	public:
		Result * applyInternal(Image *img);
		QString getName() const { return "Normalize"; }
};

#endif // NORMALIZE_H
