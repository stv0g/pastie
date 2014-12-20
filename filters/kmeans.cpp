#include <QtDebug>

#include "kmeans.h"

using namespace std;

KMeans::KMeans(int km) :
	k(km),
	low(0),
	high(k)
{
	settings["K"]			= new IntegerSetting(this, k, 1, 2*k);
	settings["Low"]	= new IntegerSetting(this, low, 0, 2*k-1,
		"Only set pixels with a label higher or equal to this value.");
	settings["High"]	= new IntegerSetting(this, high, 0, 2*k,
		"Only set Pixels with a label lower or equal to this value.");
}

void KMeans::reset()
{
	labels.release();
}

Result * KMeans::applyInternal(Image *img)
{
	Mat m = img->filtered.clone();
	Mat reshaped_image, reshaped_image32f;
	Mat centers, centers_u8;
	int flags = KMEANS_RANDOM_CENTERS; //KMEANS_PP_CENTERS;

	TermCriteria criteria = TermCriteria(TermCriteria::COUNT | TermCriteria::EPS, 4, 0.1);

	reshaped_image = m.reshape(1, m.cols * m.rows);
	reshaped_image.convertTo(reshaped_image32f, CV_32FC1, 1.0 / 255.0);

	if (labels.data)
		flags |= KMEANS_USE_INITIAL_LABELS;

	kmeans(reshaped_image32f, k, labels, criteria, 1, flags, centers);

	MatIterator_<Vec3b> first = img->filtered.begin<Vec3b>();
	MatIterator_<Vec3b> last = img->filtered.end<Vec3b>();
	MatConstIterator_<int> label = labels.begin<int>();

	centers.convertTo(centers_u8, CV_8UC1, 255.0);
	Mat centers_u8c3 = centers_u8.reshape(3);

	while (first != last) {
		if (*label >= low && *label <= high)
			*first = centers_u8c3.ptr<Vec3b>(*label)[0];
		else
			*first = Vec3b(0, 0, 0);

		++first;
		++label;
	}

	return NULL;
}

