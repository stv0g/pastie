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
