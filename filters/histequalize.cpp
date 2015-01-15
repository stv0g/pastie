#include "histequalize.h"

Result * HistEqualize::applyInternal(Image *img)
{
	Mat &m = img->getMat();

	equalizeHist(m, m);

	return new Result;
}
