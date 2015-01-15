#include "morph.h"

Morph::Morph(int o, int t, Size s, Point a, int i) :
	op(o),
	shape(t),
	iterations(i),
	size(s),
	anchor(a)
{
	QMap<int, QString> mapOp;
	QMap<int, QString> mapShape;

	mapOp[MORPH_OPEN]		= "Open";
	mapOp[MORPH_CLOSE]		= "Close";
	mapOp[MORPH_ERODE]		= "Erode";
	mapOp[MORPH_DILATE]		= "Dilate";
	mapOp[MORPH_BLACKHAT]	= "Blackhat";
	mapOp[MORPH_GRADIENT]	= "Gradient";

	mapShape[MORPH_RECT]	= "Rectangle";
	mapShape[MORPH_ELLIPSE]	= "Ellipse";
	mapShape[MORPH_CROSS]	= "Cross";

	settings["Iterations"]	= new IntegerSetting(this, iterations);
	settings["Operation"]	= new EnumSetting(this, op, mapOp);
	settings["Shape"]		= new EnumSetting(this, shape, mapShape);
	settings["Size"]		= new SizeSetting(this, size, Range<int>(1, 32));

	reset();
}

void Morph::reset()
{
	kernel = getStructuringElement(shape, size);
}

Result * Morph::applyInternal(Image *img)
{
	Mat &m = img->getMat();

	switch (op) {
		case MORPH_DILATE:		dilate(m, m, kernel, anchor, iterations); break;
		case MORPH_ERODE:		 erode(m, m, kernel, anchor, iterations); break;
		case MORPH_OPEN:
		case MORPH_CLOSE:
		case MORPH_BLACKHAT:
		case MORPH_GRADIENT:
					  morphologyEx(m, m, op, kernel, anchor, iterations); break;
		default:
			CV_Error(100, "Invalid morph operation");
	}

	return new Result;
}
