#include "maxchannel.h"

Result * MaxChannel::applyInternal(Image *img)
{
	Mat &m = img->getMat();

	CV_Assert(m.type() == CV_8UC3);

	for(int i = 0; i < m.rows; i++) {
		uchar *p = m.ptr<uchar>(i);
		for (int j = 0; j < 3*m.cols; j += 3) {
			p[j]	= (p[j  ] > p[j+1] && p[j  ] > p[j+2]) ? 0xff : 0;
			p[j+1]  = (p[j+1] > p[j  ] && p[j+1] > p[j+2]) ? 0xff : 0;
			p[j+2]	= (p[j+2] > p[j  ] && p[j+2] > p[j+2]) ? 0xff : 0;
		}
	}

	return new Result;
}

