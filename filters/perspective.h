#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

#include "pattern.h"
#include "filter.h"

class Perspective : public Filter
{
	public:
		Perspective(Camera *cam, Pattern *p);

		QString getName() const { return "Perspective"; }

	protected:
		Result * applyInternal(Image *img);

		Mat map;
		Camera *cam;
		Pattern *pattern;
};

#endif // PERSPECTIVE_H
