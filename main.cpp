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

	source = new Source;
	cam = new Camera(source);
	filters = new FilterList;
	images = new ImageList;
	mwindow = new MainWindow;

	QStringList imgs = QCoreApplication::arguments();
	imgs.removeFirst();
	imgs.removeDuplicates();
	images->load(imgs);

	/* Setup pipeline */
	Pattern *pat = new Pattern(Size(2, 2), Size(60, 60), Pattern::QUADRILINEAR_MARKERS);
	filters->add(pat);
	filters->add(new Perspective(cam, pat));
	filters->add(new Resize(Range<int>(400, 1000)));

	filters->add(new Blur(Blur::GAUSSIAN, Size(3, 3)));
	filters->add(new KMeans(4));
	filters->add(new Convert(COLOR_BGR2GRAY));
	filters->add(new Threshold(Threshold::OTSU));

	PadDetect *pads = new PadDetect();
	PadFilter *filter = new PadFilter(pads);

	filters->add(pads);
	filters->add(filter);
	filters->add(new PathPlanner(filter, PathPlanner::NEAREST_NEIGHBOUR));

	mwindow->show();

	return a.exec();
}
