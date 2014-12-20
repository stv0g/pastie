#ifndef FOURIER_H
#define FOURIER_H

#include "filter.h"

class Fourier : public Filter
{
	public:
                void apply(Image *img);

                QString getName() const { return "Fourier"; }
};

#endif // FOURIER_H
