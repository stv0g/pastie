#include "histequalize.h"

Result * HistEqualize::applyInternal(Image *img)
{
    equalizeHist(img->filtered, img->filtered);

	return NULL;
}
