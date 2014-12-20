#ifndef CONVERT_H
#define CONVERT_H

#include "filter.h"

class Convert : public Filter
{
	public:
		Convert(int code);

		QString getName() const { return "Convert"; }

	protected:
		Result * applyInternal(Image *img);

		int code;
};

#endif // CONVERT_H
