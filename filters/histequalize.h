#ifndef HISTEQUALIZE_H
#define HISTEQUALIZE_H

#include "filter.h"

class HistEqualize : public Filter
{
	public:
		QString getName() const { return "HistEqualize"; }

	protected:
		Result * applyInternal(Image *img);

};

#endif // HISTEQUALIZE_H
