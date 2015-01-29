#ifndef KMEANS_H
#define KMEANS_H

#include "range.h"
#include "filter.h"

class KMeans : public Filter
{
	Q_OBJECT

	public:
		KMeans(int k);

		QString getName() const { return "KMeans"; }

		void reset();
		void clicked(Point, QMouseEvent *);


	protected:
		Result * applyInternal(Image *img);

		QList<Point> colorFilterPoints;
		Range<int> colorFilterRange;

		int k, iterations;
		Mat labels;
		bool iterative;
};

#endif // KMEANS_H
