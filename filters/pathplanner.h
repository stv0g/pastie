#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include "paddetect.h"
#include "filter.h"

class PathPlanner : public Filter
{
	public:
		enum Algorithm {
			NEAREST_NEIGHBOUR,
			REPETETIVE_NEAREST_NEIGHBOUR
		};

		PathPlanner(Filter *source, enum Algorithm algo = NEAREST_NEIGHBOUR);

		QString getName() const { return "PathPlanner"; }

	protected:
		enum Algorithm algo;

		Filter *source;
		Result * applyInternal(Image *img);

		//Mat calcDistanceMat(QList<Pad> pads);
};

class PathResult : public PadResult
{
	public:
		void drawResult(Painter *p) const;

		double getLength() const;
		QString getResult() const;
};

#endif // PATHPLANNER_H
