#include "derivative.h"

Derivative::Derivative(Type t, Size o, int s) :
	type(t),
	order(o),
	size(s),
	maxima(false)
{
	QMap<int, QString> typeMap;
	typeMap[SOBEL] = "Sobel";
	typeMap[SCHARR] = "Scharr";

	settings["Type"]	= new EnumSetting(this, (int&) type, typeMap);
	settings["Order"]	= new SizeSetting(this, order);
	settings["Size"]	= new IntegerSetting(this, size, Range<int>(1, 10));
	settings["Maxima"]	= new BooleanSetting(this, maxima, "Find local maximas in source image.");
	settings["Scale"]	= new DoubleSetting(this, scale);
	settings["Delta"]	= new DoubleSetting(this, delta);
}

Result * Derivative::applyInternal(Image *img)
{
	Mat &n = img->getMat();

	switch (type) {
		case SOBEL:	 Sobel(n, n, n.depth(), order.width, order.height, size, scale, delta); break;
		case SCHARR: Scharr(n, n, n.depth(), order.width, order.height, scale, delta); break;
	}

	if (maxima)
		compare(n, 0, n, CMP_EQ);

	return new Result;
}

