#ifndef UNDISTORT_H
#define UNDISTORT_H

#include "filter.h"
#include "camera.h"

class Undistort : public Filter
{
	public:
		Undistort(Camera *c);
        void reset();

		QString getName() const { return "Undistort"; }

	protected:
		Result * applyInternal(Image *img);

		Mat map1;
		Mat map2;

		Camera *cam;
};

#endif // UNDISTORT_H
