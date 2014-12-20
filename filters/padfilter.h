#ifndef PADFILTER_H
#define PADFILTER_H

#include "filter.h"

class PadFilter : public Filter
{
	public:
		QString getName() const { return "PadFilter"; };

	protected:
		Result * applyInternal(Image *img);

};

#endif // PADFILTER_H
