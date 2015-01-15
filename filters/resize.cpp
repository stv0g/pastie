#include "resize.h"

Resize::Resize(Range<int> r, int i) :
	range(r),
	interpolation(i)
{
	QMap<int, QString> interMap;
	interMap[INTER_NEAREST]		= "Nearest Neighbour";
	interMap[INTER_LINEAR]		= "Linear";
	interMap[INTER_AREA]		= "Pixel Area Relation";
	interMap[INTER_CUBIC]		= "Bi-Cubic (4x4)";
	interMap[INTER_LANCZOS4]	= "Lanczos4 (8x8)";

	settings["Range"] = new RangeSetting(this, range, Range<int>(200, 3000));
	settings["Interpolation"] = new EnumSetting(this, (int &) interpolation, interMap);
}

Result * Resize::applyInternal(Image *img)
{
	Mat &m = img->getMat();

	if (!range.contains(m.cols)) {
		double f = 1;

		if (m.cols < range.min)
			f = (double) range.min / m.cols;
		else if (m.cols > range.max)
			f = (double) range.max / m.cols;

		resize(m, m, Size(), f, f, interpolation);

		return new Result(QTransform::fromScale(f, f));
	}
	else
		return new Result;
}
