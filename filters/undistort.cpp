#include "undistort.h"

Undistort::Undistort(Camera *c) :
	cam(c)
{
    reset();
}

void Undistort::reset()
{
	Mat newCamMat = getOptimalNewCameraMatrix(cam->getMatrix(), cam->getDistCoeffs(), cam->source->getSize(), 1);

    initUndistortRectifyMap(cam->getMatrix(), cam->getDistCoeffs(),
							Mat(), newCamMat, cam->source->getSize(), CV_16SC2, map1, map2);
}

Result * Undistort::applyInternal(Image *img)
{
	if (cam->isCalibrated()) {
#if 1
        remap(img->filtered, img->filtered, map1, map2, INTER_LINEAR);
#else
        Mat dest;
		undistort(mat, dest, cam->getMatrix(), cam->getDistCoeffs());
		mat = dest;
#endif
    }

	return NULL;
}
