#include <opencv2/imgproc.hpp>

#include "blur.h"

Blur::Blur(enum Type t, Size ks, double sp, double clr, double si, int bdr) :
	type(t),
	size(ks),
	border(bdr),
	sigma(si),
	color(clr),
	space(sp)
{
	QMap<int, QString> mapType;
	QMap<int, QString> mapBorder;

	mapType[BILATERAL]			= "Bilateral";
	//mapType[BILATERAL_ADAPTIVE] = "Adaptive Bilateral";
	mapType[BOX]				= "Box";
	mapType[BOX_NORMALIZED]		= "Box Normalized";
	mapType[GAUSSIAN]			= "Gaussian";
	mapType[MEDIAN]				= "Median";

	mapBorder[BORDER_DEFAULT]	= "Default";
	//mapBorder[BORDER_CONSTANT]	= "Constant";
	//mapBorder[BORDER_REPLICATE] = "Replicate";
	mapBorder[BORDER_ISOLATED]	= "Isolated";
	mapBorder[BORDER_REFLECT]	= "Reflect";
	mapBorder[BORDER_REFLECT_101] = "Reflect 101";
	mapBorder[BORDER_WRAP]		= "Wrap";

	settings["Type"]		= new EnumSetting(this, (int&) type, mapType);
	settings["Border"]		= new EnumSetting(this, (int&) border, mapBorder);
	settings["Kernel Size"] = new SizeSetting(this, size);
	settings["Space"]		= new DoubleSetting(this, space);
	settings["Color"]		= new DoubleSetting(this, color);
	settings["Sigma"]		= new DoubleSetting(this, sigma);
}

Result * Blur::applyInternal(Image *img)
{
	Mat m = img->filtered.clone();
	Mat &n = img->filtered;

	switch (type) {
		case BILATERAL:				bilateralFilter(m, n, -1, space, color); break;
		case BILATERAL_ADAPTIVE:	break; // FIXME adaptiveBilateralFilter(m, n, size, color, space); break;
		case BOX:					boxFilter(m, n, -1, size, anchor, true, border); break;
		case BOX_NORMALIZED:		blur(m, n, size, anchor, border); break;
		case GAUSSIAN:				GaussianBlur(m, n, size, sigma, 0, border); break;
		case MEDIAN:				medianBlur(m, n, size.width); break;
	}

	return NULL;
}

