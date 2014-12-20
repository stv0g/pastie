#ifndef CHANNEL_H
#define CHANNEL_H

#include "filter.h"

class Channel : public Filter
{
	public:
		Channel(int ch);

		QString getName() const { return "Channel"; }

	protected:
		Result * applyInternal(Image *img);

		int channel;
};

#endif // CHANNEL_H
