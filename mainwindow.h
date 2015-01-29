#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>

#include <QMainWindow>
#include <QSignalMapper>

#include "tabcalibration.h"
#include "tabcamera.h"
#include "tabfilters.h"
#include "tabimages.h"

#include "filterlist.h"
#include "imagelist.h"
#include "camera.h"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

		/* Getter */
		Image * getCurrentImage() { return currentImage; }

	public slots:
		void showAbout();
		void showImage(Image *img);
		void showFrame(Image *img);
		void render();

	protected:
		Ui::MainWindow *ui;

		Image *currentImage;
};

#endif // MAINWINDOW_H
