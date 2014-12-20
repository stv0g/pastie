#ifndef MORPH_H
#define MORPH_H

#include "filter.h"

class Morph : public Filter
{
	public:
		Morph(int op, int shape, Size size = Size(3, 3), Point anchor = Point(-1, -1), int iterations = 1);
		QString getName() const { return "Morph"; }

	public slots:
		void reset();

	protected:
		Result * applyInternal(Image *img);

		Mat kernel;

		int op;
		int shape;
		int iterations;
		Size size;
		Point anchor;
};

#endif // MORPH_H
