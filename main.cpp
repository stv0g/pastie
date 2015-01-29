#include "mainwindow.h"
#include <QApplication>
#include <QCameraInfo>

#include "filters.h"
#include "imagelist.h"
#include "filterlist.h"
#include "source.h"
#include "camera.h"
#include "robot.h"

Source *source;
Camera *cam;
Robot *robot;
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

	QStringList imgs = QCoreApplication::arguments();
	imgs.removeFirst();
	imgs.removeDuplicates();
	images->load(imgs);

	/* Setup pipeline */
	Pattern *pattern = new Pattern(Size(2, 2), Size(60, 60), Pattern::QUADRILINEAR_MARKERS);
	PadDetect *pads = new PadDetect();
	PadFilter *filter = new PadFilter(pads);
	PathPlanner *planner = new PathPlanner(filter, PathPlanner::REPETETIVE_NEAREST_NEIGHBOUR);

	filters->append(pattern);
	filters->append(new Perspective(cam, pattern));
	filters->append(new Resize(Range<int>(400, 1000)));
	filters->append(new Blur(Blur::GAUSSIAN, Size(3, 3)));
	filters->append(new KMeans(4));
	filters->append(new Convert(COLOR_BGR2GRAY));
	filters->append(new Threshold(Threshold::OTSU));
	filters->append(new Morph(MORPH_CLOSE, MORPH_RECT));
	filters->append(pads);
	filters->append(filter);
	filters->append(planner);

	robot = new Robot(pattern, planner);
	mwindow = new MainWindow;

	mwindow->show();

	return a.exec();
}
