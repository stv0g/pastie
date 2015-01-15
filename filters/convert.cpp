#include "convert.h"

Convert::Convert(int c) :
	Filter(),
	code(c)
{
	QMap<int, QString> mapCode;
	mapCode[COLOR_BGR2HSV]	= "BGR to HSV";
	mapCode[COLOR_BGR2GRAY] = "BGR to Gray";
	mapCode[COLOR_BGR2Lab]  = "BGR to Lab";
	mapCode[COLOR_BGR2Luv]	= "BGR to Luv";
	mapCode[COLOR_BGR2HLS]  = "BGR to HLS";
	mapCode[COLOR_BGR2YUV]  = "BGR to YUV";
	mapCode[COLOR_BGR2YCrCb]= "BGR to YCrCb";
	// FIXME add more codes

	settings["Code"] = new EnumSetting(this, code, mapCode);
}

Result * Convert::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	Mat n = m.clone();

	cvtColor(n, m, code);

	return new Result;
}

