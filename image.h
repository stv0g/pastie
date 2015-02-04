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

#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QMap>
#include <QImage>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

class Camera;
class Filter;
class Result;

class Image
{
	public:
		Image(Mat mat = Mat(), QString path = QString());
		Image(QString p);
		~Image();

		void save(QString path = QString());
		void load(QString path = QString());

		/* Getter */
		bool isLoaded() const { return loaded; }
		bool isSaved() const { return saved; }

		Result * getResult(Filter *f) { return results[f]; }
		QString getPath() { return path; }
		Mat & getMat();
		QImage getQImage();

		const Mat & getSourceMat();
		void setSourceMat(const Mat &m);

		QMap<Filter*, Result*> results;

	protected:
		QString path;
		Mat source;
		Mat filtered;

		bool loaded;
		bool saved;
};

#endif // IMAGE_H
