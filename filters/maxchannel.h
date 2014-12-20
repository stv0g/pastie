#ifndef MAXCHANNEL_H
#define MAXCHANNEL_H

#include "filter.h"

class MaxChannel : public Filter
{
	public:
		Result * applyInternal(Image *img);

		QString getName() const { return "MaxChannel"; }

};

#endif // MAXCHANNEL_H
