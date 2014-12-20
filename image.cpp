#include <opencv2/highgui.hpp>

#include "image.h"
#include "filters/filter.h"

using namespace cv;

Image::Image(Mat m, QString p) :
	path(p),
	original(m.clone()),
	loaded(false),
	saved(false)
{ }

Image::Image(QString p) :
	path(p),
	saved(false)
{
	load();
}

Image::~Image()
{
	qDeleteAll(results);
	results.clear();
}

void Image::load(QString p)
{
	if (p != "")
		path = p;

	original = cv::imread(path.toStdString());
	loaded = (bool) original.data;
}

void Image::save(QString p)
{
	if (p != "")
		path = p;

	saved = imwrite(path.toStdString(), original);
}

void Image::applyFilter(Filter *filter)
{
// FIXME!
//	if (results.contains(filter))
//		delete results[filter];

	Result *result = filter->apply(this);
	if (result)
		results[filter] = result;
}
