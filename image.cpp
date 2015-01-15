#include <opencv2/highgui.hpp>

#include "image.h"
#include "filters/filter.h"

using namespace cv;

Image::Image(Mat m, QString p) :
	path(p),
	source(m),
	loaded(true),
	saved(false)
{ }

Image::Image(QString p) :
	path(p),
	loaded(false),
	saved(false)
{ }

Image::~Image()
{
	qDeleteAll(results);
	results.clear();
}

void Image::load(QString p)
{
	if (p != "")
		path = p;

	source = cv::imread(path.toStdString());
	loaded = (bool) source.data;
}

void Image::save(QString p)
{
	if (p != "")
		path = p;

	saved = imwrite(path.toStdString(), getMat());
}

void Image::applyFilter(Filter *filter)
{
	Result *result = filter->apply(this);
	if (result)
		results[filter] = result;
}

Mat & Image::getMat()
{
	if (!filtered.data)
		filtered = getSourceMat().clone();

	return filtered;
}

const Mat & Image::getSourceMat()
{
	if (!loaded)
		load();

	return source;
}

void Image::setSourceMat(const Mat &m)
{
	source = m;
}
