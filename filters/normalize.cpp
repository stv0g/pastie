#include "normalize.h"

Result * Normalize::applyInternal(Image *img)
{
	Mat &m = img->filtered;

	CV_Assert(m.type() == CV_8UC3);

	for(int i = 0; i < m.rows; i++) {
		uchar *p = m.ptr<uchar>(i);
		for (int j = 0; j < 3*m.cols; j += 3) {

			double s = 255.0 / (p[j] + p[j+1] + p[j+2]);

			p[j]   *= s;
			p[j+1] *= s;
			p[j+2] *= s;
		}
	}

	return NULL;
}
