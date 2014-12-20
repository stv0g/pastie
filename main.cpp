#include "mainwindow.h"
#include <QApplication>
#include <QCameraInfo>

/* Filters */
#include "filters.h"
#include "imagelist.h"
#include "filterlist.h"
#include "source.h"
#include "camera.h"

Source *source;
Camera *cam;
FilterList *filters;
ImageList *images;
MainWindow *mwindow;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
	foreach (const QCameraInfo &cameraInfo, cameras)
		qDebug() << "Found Qt camera: " << cameraInfo.deviceName() << ": " << cameraInfo.description();

	filters = new FilterList;
	images = new ImageList;
	source = new Source;
	cam = new Camera(source);
	mwindow = new MainWindow;

	QStringList imgs = QCoreApplication::arguments();
	imgs.removeFirst();
	imgs.removeDuplicates();
	imgs.sort();
	images->load(imgs);

	/* Setup pipeline */
#if 1
	filters->add(new Blur(Blur::GAUSSIAN, Size(3, 3)));
	filters->add(new KMeans(4));
	filters->add(new Convert(COLOR_BGR2HSV));
	filters->add(new Channel(1));
	//filters->add(new Convert(COLOR_BGR2GRAY));
	//filters->add(new HistEqualize());
	filters->add(new Threshold(Threshold::OTSU));
	//filters->add(new EdgeDetect(80, 3));
	//filters->add(new Watershed());
	filters->add(new Morph(MORPH_CLOSE, MORPH_RECT));
	filters->add(new ShapeDetect());
	//filters->add(new MaxChannel());
	//filters->add(new Normalize());
#else
	filters->add(new Undistort(cam));
	filters->add(new Pattern(Size(13, 9), 60));
	filters->add(new Perspective(cam, (Pattern *) filters->last()));
#endif

	mwindow->show();

	return a.exec();
}
