#ifndef KMEANS_H
#define KMEANS_H

#include "filter.h"

class KMeans : public Filter
{
	Q_OBJECT

	public:
		KMeans(int k);

		QString getName() const { return "KMeans"; }

	public slots:
		void reset();

	protected:
		Result * applyInternal(Image *img);

		int k;
		int low, high;
		Mat labels;
};

#endif // KMEANS_H
