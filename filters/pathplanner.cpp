#include "pathplanner.h"
#include "range.h"

PathPlanner::PathPlanner(Filter *s, enum Algorithm a) :
	algo(a),
	source(s)
{
	QMap<int, QString> algoMap;
	algoMap[NEAREST_NEIGHBOUR] = "Nearest Neighbour";
	algoMap[REPETETIVE_NEAREST_NEIGHBOUR] = "Repetitive Nearest Neighbour";

	settings["Algorithm"] = new EnumSetting(this, (int &) algo, algoMap);
}

Result * PathPlanner::applyInternal(Image *img)
{
	PathResult *path = NULL;
	PadResult *padResult = dynamic_cast<PadResult*>(img->getResult(source));

	if (padResult) {
		if (padResult->size() < 1 || padResult->size() > 1000) {
			qWarning() << "Found too much/less pads (" << padResult->size() << ")! Skipping PathPlanner";
			return path;
		}

		switch (algo) {
			case NEAREST_NEIGHBOUR: {
				path = new PathResult;
				PadResult pads = *padResult; /* create a copy */
				PadResult::iterator start = pads.begin(); /* fixed */
				Pad last = *start;

				path->append(*start);
				pads.erase(start);

				while (!pads.isEmpty()) {
					PadResult::iterator nearest = pads.getNearest(last);
					path->append(*nearest);
					pads.erase(nearest);

					last = *nearest;
				}
			} break;

			case REPETETIVE_NEAREST_NEIGHBOUR: {
				double minLength = FLT_MAX;

				for (int i = 0; i < padResult->size(); i++) {
					double currentLength = 0;
					PathResult *currentPath = new PathResult;
					PadResult pads = *padResult;
					PadResult::iterator start = pads.begin() + i;
					Pad last = *start;

					currentPath->append(*start);
					pads.erase(start);

					while (!pads.isEmpty()) {
						PadResult::iterator nearest = pads.getNearest(last);

						CV_Assert(nearest != pads.end());

						currentLength += last.getDistance(*nearest);
						if (currentLength > minLength)
							break;

						currentPath->append(*nearest);
						pads.erase(nearest);

						last = *nearest;
					}

					if (currentLength > minLength)
						delete currentPath;
					else {
						if (path)
							delete path;

						minLength = currentLength;
						path = currentPath;
					}	
				}
			} break;
		}

		return path;
	}
	else
		qWarning() << getName() << "Missing pad list!";

	return new Result;
}

void PathResult::drawResult(Painter *p) const
{
	if (size() >= 2) {
		QPen pe = p->pen();
		QBrush br(Qt::SolidPattern);

		pe.setWidth(2);

		const_iterator prev = begin();
		const_iterator curr = begin() + 1;
		for (int i = 0; curr != end(); i++, curr++, prev++) {
			QColor c = QColor::fromHsl(360.0 / size() * i, 255, 128);

			pe.setColor(c);
			br.setColor(c);
			p->setPen(pe);
			p->setBrush(br);
			p->drawLine(toQt(*prev), toQt(*curr));
			p->drawEllipse(toQt(*curr), 1, 1);
		}

		p->drawMarker(toQt((Point2i) (Point2f) first()));
		p->drawMarker(toQt((Point2i) (Point2f) last()));
	}
}

double PathResult::getLength() const
{
	double length = 0;

	if (size() >= 2) {
		const_iterator curr = begin() + 1;
		const_iterator prev = begin();

		for (; curr != end(); prev++, curr++)
			length += prev->getDistance(*curr);
	}

	return length;
}

QString PathResult::getResult() const
{
	return QString("pads = %1, length = %2").arg(size()).arg(getLength());
}
