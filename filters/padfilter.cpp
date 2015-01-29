#include "padfilter.h"
#include "paddetect.h"

PadFilter::PadFilter(Filter *s) :
	source(s),
	areaRange(35, 6000),
	ratioRange(1, 3)
{
	settings["Area Range"]  = new DoubleRangeSetting(this, areaRange,  Range<double>(1, 50000));
	settings["Ratio Range"] = new DoubleRangeSetting(this, ratioRange, Range<double>(1, 10));
}

Result * PadFilter::applyInternal(Image *img)
{
	PadResult *pads = dynamic_cast<PadResult*>(img->getResult(source));
	if (pads) {
		PadResult *result = new PadResult;
		for (Pad &pad : *pads) {
			double ratio = pad.getRatio();
			double area  = pad.getArea();

			if (areaRange.contains(area) &&
				ratioRange.contains(ratio))
				result->push_back(pad);
		}

		for (Point2f p : addPoints)
			result->append(Pad(p));

		for (Point2f p : delPoints)
			result->erase(result->getNearest(p));

		return result;
	}
	else
		qWarning() << getName() << "Missing pad list!";

	return new Result;
}

void PadFilter::reset()
{
	addPoints.clear();
	delPoints.clear();
}

void PadFilter::clicked(Point p, QMouseEvent *me)
{
	if (me->modifiers() & Qt::AltModifier)
		delPoints.append(Point2f(p));
	else
		addPoints.append(Point2f(p));

	emit filterChanged(this);
}
