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
	Image(Mat mat = Mat(), QString path = "");
	Image(QString p);
    ~Image();

	void save(QString path = "");
	void load(QString path = "");

	QString path;

    Mat original;
    Mat filtered;

	void applyFilter(Filter *filter);

	/* Getter */
	Result * getResult(Filter *f) { return results[f]; }

protected:
	QMap<Filter*, Result*> results;

	bool loaded;
	bool saved;
};

#endif // IMAGE_H
