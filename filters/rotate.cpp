#include "rotate.h"

Rotate::Rotate(double a, int i = INTER_LINEAR, double s) :
	angle(a),
	interpolation(i),
	scale(s)
{
	QMap<int, QString> interMap;
	interMap[INTER_NEAREST]		= "Nearest Neighbour";
	interMap[INTER_LINEAR]		= "Linear";
	interMap[INTER_AREA]		= "Pixel Area Relation";
	interMap[INTER_CUBIC]		= "Bi-Cubic (4x4)";
	interMap[INTER_LANCZOS4]	= "Lanczos4 (8x8)";

	settings["Angle"] = new DoubleSetting(this, angle, Range<double>(0, 360));
	settings["Scale"] = new DoubleSetting(this, scale, Range<double>(0.5, 2));
	settings["Interpolation"] = new EnumSetting(this, (int &) interpolation, interMap);
}

Result * Rotate::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	Mat affine = getRotationMatrix2D(Point2f(m.cols/2, m.rows/2), angle, 1);

	warpAffine(m, m, affine, m.size(), interpolation);

	return new Result(toQTransform(affine));
}
