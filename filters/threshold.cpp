#include "threshold.h"

Threshold::Threshold(enum Threshold::Type t, double v, int sz, double d) :
	Filter(),
	type(t),
	size(sz),
	value(v),
	c(d)
{
	QMap<int, QString> typeMap;
	typeMap[ADAPTIVE_GAUSS] = "Adaptive Gauss";
	typeMap[ADAPTIVE_MEAN]  = "Adaptive Mean";
	typeMap[FIXED]          = "Fixed";
	typeMap[OTSU]			= "Otsu's method";
	typeMap[TRIANGLE]		= "Triangle";

	settings["Type"]		= new EnumSetting(this, (int&) type, typeMap);
	settings["Threshold"]	= new DoubleSetting(this, value, 0, 0xff);
	settings["C"]			= new DoubleSetting(this, c, 0, 0xff);
	settings["Size"]		= new IntegerSetting(this, size, 1, 100);
	settings["Inverted"]	= new BooleanSetting(this, invert);
}

Result * Threshold::applyInternal(Image *img)
{
	Mat &m = img->filtered;

	int mode = (invert) ? THRESH_BINARY_INV : THRESH_BINARY;

	if (type == ADAPTIVE_GAUSS)
			adaptiveThreshold(m, m, 0xff, ADAPTIVE_THRESH_GAUSSIAN_C, mode, size, c);
	else if (type == ADAPTIVE_MEAN)
			 adaptiveThreshold(m, m, 0xff, ADAPTIVE_THRESH_MEAN_C, mode, size, c);
	else {
		int method;

		switch (type) {
			case OTSU:		method = THRESH_OTSU; break;
			case TRIANGLE:	method = THRESH_TRIANGLE; break;
			default:		method = 0;
		}

		double ret = threshold(img->filtered, img->filtered, value, 0xff, mode | method);
		return new DoubleResult("tresh", ret);
	}

	return NULL;
}
