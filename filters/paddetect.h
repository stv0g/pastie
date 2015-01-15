#ifndef PADDETECT_H
#define PADDETECT_H

#include <vector>

#include "pad.h"
#include "filter.h"

using namespace std;

class PadDetect : public Filter
{
	public:
		enum Mode {
			AREA,
			COSINE
		};

		PadDetect(double epsilon = .1, double cosine = 0.3);
		QString getName() const { return "PadDetect"; };

	protected:
		Result * applyInternal(Image *img);

		double epsilon;
		double areaRatio;
		double cosine;
		enum Mode mode;
};

class PadResult : public Result, public QList<Pad>
{
	public:
		void drawResult(Painter *p) const;

		QString getResult() const;

		QList<Pad>::iterator getNearest(const Point &p);
};

#endif // PADDETECT_H
