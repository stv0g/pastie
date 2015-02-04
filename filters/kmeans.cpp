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

#include <QtDebug>
#include <QBitArray>

#include "kmeans.h"

using namespace std;

KMeans::KMeans(int km) :
	colorFilterRange(1, km),
	k(km),
	iterations(3),
	iterative(false)
{
	settings["K"]			= new IntegerSetting(this, k, Range<int>(1, 10));
	settings["Iterations"]	= new IntegerSetting(this, iterations, Range<int>(1, 10));
	settings["Iterative"]	= new BooleanSetting(this, iterative, "Use labels from last run as starting point.");
	settings["Color"]		= new RangeSetting(this, colorFilterRange, Range<int>(1, 10));
}

void KMeans::reset()
{
	labels.release();
	colorFilterPoints.clear();
}

Result * KMeans::applyInternal(Image *img)
{
	Mat &m = img->getMat();
	Mat reshaped_image, reshaped_image32f;
	Mat centers, centers_u8;

	int flags = KMEANS_PP_CENTERS;
	if (iterative && labels.data)
		flags |= KMEANS_USE_INITIAL_LABELS;

	reshaped_image = m.reshape(1, m.cols * m.rows);
	reshaped_image.convertTo(reshaped_image32f, CV_32FC1, 1.0 / 255.0);

	TermCriteria criteria = TermCriteria(TermCriteria::COUNT | TermCriteria::EPS, 4, 0.1);
	kmeans(reshaped_image32f, k, labels, criteria, iterations, flags, centers);

	/* Filter single color */
	if (colorFilterPoints.size() || colorFilterRange.min > 1 || colorFilterRange.max < k) {
		Mat bw = Mat(m.size(), CV_8UC1);
		QBitArray mask(k);

		/* Calculate label histogram */
		if (colorFilterRange.min > 1 || colorFilterRange.max < k) {
			QList<QPair<int, int>> hist;
			for (int i = 0; i < k; i++)
				hist.push_back(qMakePair(0, i));

			for (auto it  = labels.begin<int>();
					  it != labels.end<int>();
					  it++)
				hist[*it].first++;

			qSort(hist);

			for (QPair<int,int> p : hist)
				qDebug() << "Label = " << p.second << "Count =" << p.first;

			for (int i = colorFilterRange.min-1; i < colorFilterRange.max-1; i++)
				mask.setBit(hist[i].second);
		}

		/* Create label mask */
		Mat labelShape = labels.reshape(1, m.rows);
		for (Point p : colorFilterPoints) {
			int label = labelShape.at<int>(p);
			mask.setBit(label);
		}

		qDebug() << "mask = " << mask << "labelShape = " << labelShape.cols << labelShape.rows;

		/* Filter labels */
		MatIterator_<uchar> pix = bw.begin<uchar>();
		for (auto lbl = labels.begin<int>(); lbl != labels.end<int>(); lbl++, ++pix)
			*pix = mask.testBit(*lbl) ? 0xff : 0;

		m = bw;
	}
	else {
		centers.convertTo(centers_u8, CV_8UC1, 255.0);
		Mat centers_u8c3 = centers_u8.reshape(3);

		MatIterator_<Vec3b> pixel = m.begin<Vec3b>();
		for (auto label  = labels.begin<int>();
				  label != labels.end<int>();
				  label++, ++pixel)
			*pixel = centers_u8c3.ptr<Vec3b>(*label)[0];
	}

	return new Result;
}

void KMeans::clicked(Point p, QMouseEvent *)
{
	colorFilterPoints.append(p);

	emit filterChanged(this);
}
