/*
 * This file is part of Pastie
 *
 * Pastie is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Pastie is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pastie. If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @copyright  2015 Steffen Vogel
 * @license	   http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author     Steffen Vogel <steffen.vogel@rwth-aachen.de>
 * @link       http://www.steffenvogel.de
 */

#include <opencv2/highgui.hpp>

#include "image.h"
#include "filters/filter.h"

using namespace cv;

Image::Image(Mat m, QString p) :
	path(p),
	source(m),
	loaded(true),
	saved(false)
{ }

Image::Image(QString p) :
	path(p),
	loaded(false),
	saved(false)
{ }

Image::~Image()
{
	qDeleteAll(results);
	results.clear();
}

void Image::load(QString p)
{
	if (!p.isNull())
		path = p;

	source = cv::imread(path.toStdString());
	loaded = (bool) source.data;
}

void Image::save(QString p)
{
	if (!p.isNull())
		path = p;

	saved = imwrite(path.toStdString(), getMat());
}

Mat & Image::getMat()
{
	if (!filtered.data)
		filtered = getSourceMat().clone();

	return filtered;
}

QImage Image::getQImage()
{
	return toQImage(getMat());
}


const Mat & Image::getSourceMat()
{
	if (!loaded)
		load();

	return source;
}

void Image::setSourceMat(const Mat &m)
{
	source = m;
}
