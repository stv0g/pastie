#include "pad.h"

Pad::Pad(const Point &p) :
	RotatedRect(p, Size(), 0)
{
	points(vertexes);
}

Pad::Pad(const RotatedRect &p) :
	RotatedRect(p)
{
	points(vertexes);
}
