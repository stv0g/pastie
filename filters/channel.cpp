#include <opencv2/imgproc.hpp>

#include "channel.h"

using namespace cv;

Channel::Channel(int ch) :
	channel(ch)
{
	settings["Channel"] = new IntegerSetting(this, channel, Range<int>(0, 5));
}

Result * Channel::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	Mat *channels = new Mat[m.channels()];

	if (channel < m.channels()) {
		split(m, channels);
		m = channels[channel];
	}

	delete[] channels;

	return new Result;
}
