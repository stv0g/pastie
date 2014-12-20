#include <opencv2/imgproc.hpp>

#include "channel.h"

using namespace cv;

Channel::Channel(int ch) :
	channel(ch)
{
	settings["Channel"] = new IntegerSetting(this, channel);
}

Result * Channel::applyInternal(Image *img)
{
	Mat channels[3];
	split(img->filtered, channels);

	img->filtered = channels[(int) channel];

	return NULL;
}

