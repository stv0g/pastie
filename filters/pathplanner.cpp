#include "pathplanner.h"
#include "range.h"

PathPlanner::PathPlanner(Filter *s, enum Algorithm a) :
	algo(a),
	source(s)
{
	QMap<int, QString> algoMap;
	algoMap[NEAREST_NEIGHBOUR] = "Nearest Neighbour";
	algoMap[REPETETIVE_NEAREST_NEIGHBOUR] = "Repetitive Nearest Neighbour";

	//settings["Algorithm"] = new EnumSetting(this, (int &) algo, algoMap);
}

Result * PathPlanner::applyInternal(Image *img)
{
	PadResult *list = dynamic_cast<PadResult*>(img->getResult(source));
	if (list) {
		PathResult *path = new PathResult;
		PadResult vertices = *list; /* create a copy */

		if (vertices.size() < 1 || vertices.size() > 1000) {
			qWarning() << "Found too much/less pads (" << vertices.size() << ")! Skipping PathPlanner";
			return path;
		}

		switch (algo) {
			case NEAREST_NEIGHBOUR:
				path->append(vertices.takeFirst());

				while (!vertices.isEmpty()) {
					auto nearest = vertices.getNearest(path->last().center);
					path->append(*nearest);
					vertices.erase(nearest);
				}
				break;

			case REPETETIVE_NEAREST_NEIGHBOUR:
				// FIXME: implement
				break;
		}

		return path;
	}
	else
		qWarning() << getName() << "Missing pad list!";

	return new Result;
}

#if 0
Mat PathPlanner::calcDistanceMat(QList<Point2f> vertices)
{
	int size = vertices.size();
	Mat distance = Mat::zeros(size, size, CV_32F);

	for (int i = 0; i < size; i++) {
		for (int j = i+1; j < size; j++) {
			distance.at<float>(i, j) = Pad::distance(pads[i], pads[j]);
		}
	}

	return distance;
}
#endif

void PathResult::drawResult(Painter *p) const
{
	if (size()) {
		QPen pe = p->pen();
		QBrush br(Qt::SolidPattern);

		pe.setWidth(2);

		for (int i = 1; i < size(); i++) {
			QColor c = QColor::fromHsl(360.0 / size() * i, 255, 128);

			pe.setColor(c);
			br.setColor(c);
			p->setPen(pe);
			p->setBrush(br);
			p->drawLine(toQt(at(i-1)), toQt(at(i)));
			p->drawEllipse(toQt(at(i)), 1, 1);
		}

		p->drawMarker(toQt((Point2i) (Point2f) first()));
		p->drawMarker(toQt((Point2i) (Point2f) last()));
	}
}

float PathResult::getLength() const
{
	float length = 0;
	for (int i = 1; i < size(); i++)
		length += norm(at(i-1).center - at(i).center);

	return length;
}

QString PathResult::getResult() const
{
	return QString("pads = %1, length = %2").arg(size()).arg(getLength());
}
