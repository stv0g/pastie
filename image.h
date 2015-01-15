#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QMap>

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

		void applyFilter(Filter *filter);

		/* Getter */
		bool isLoaded() const { return loaded; }
		bool isSaved() const { return saved; }

		Result * getResult(Filter *f) { return results[f]; }
		QString getPath() { return path; }
		Mat & getMat();

		const Mat & getSourceMat();
		void setSourceMat(const Mat &m);

	protected:
		QMap<Filter*, Result*> results;

		QString path;
		Mat source;
		Mat filtered;

		bool loaded;
		bool saved;
};

#endif // IMAGE_H
