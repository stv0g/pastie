#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>

#include <QMainWindow>
#include <QSignalMapper>

#include "tabcalibration.h"
#include "tabcamera.h"
#include "tabfilters.h"
#include "tabimages.h"
#include "tabpads.h"

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

	public slots:
		void showAbout();

	protected:
		Ui::MainWindow *ui;

		void showEvent(QShowEvent *se);

	signals:
		void newImage(Image *img);
};

#endif // MAINWINDOW_H
