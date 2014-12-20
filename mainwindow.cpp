#include <opencv2/core.hpp>

#include <QDialog>
#include <QtDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_about.h"

using namespace cv;

extern FilterList *filters;
extern ImageList *images;
extern Camera *cam;
extern Source *source;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{	
	ui->setupUi(this);
	ui->logConsole->setup();
	ui->splitterH->setSizes({
		static_cast<int>(0.8 * ui->splitterH->width()),
		static_cast<int>(0.2 * ui->splitterH->width())
	});

	/* Connect actions */
	connect(images,					&ImageList::newImage,		ui->viewer,				&Viewer::showImage);
	connect(source,					&Source::newImage,			ui->viewer,				&Viewer::showImage);
	connect(filters,				&FilterList::filtersChanged,ui->viewer,				&Viewer::updateImage);
	connect(ui->actionRedraw,		&QAction::triggered,		ui->viewer,				&Viewer::updateImage);
	connect(ui->actionCalibrate,	&QAction::triggered,		ui->tabCalibration,		&TabCalibration::doCalibration);
	connect(ui->actionSnapshot,		&QAction::triggered,		ui->tabCamera,			&TabCamera::doSnapshot);
	connect(ui->actionAbout,		&QAction::triggered,		this,					&MainWindow::showAbout);
	connect(ui->actionExit,			&QAction::triggered,		this,					&MainWindow::close);
	connect(ui->actionClear,		&QAction::triggered,		images,					&ImageList::clear);
	connect(ui->actionLoad,			&QAction::triggered,		images,					&ImageList::loadFilePicker);
	connect(ui->actionSave,			&QAction::triggered,		images,					&ImageList::saveFilePicker);
	connect(ui->actionNextImage,	&QAction::triggered,		images,					&ImageList::nextImage);
	connect(ui->actionPrevImage,	&QAction::triggered,		images,					&ImageList::prevImage);
	connect(ui->actionPlay,			&QAction::triggered,		source,					&Source::play);
	connect(ui->actionTabImages,	&QAction::triggered,		[=]() { ui->tabWidget->setCurrentWidget(ui->tabImages); });
	connect(ui->actionTabFilters,	&QAction::triggered,		[=]() { ui->tabWidget->setCurrentWidget(ui->tabFilters); });
	connect(ui->actionTabCamera,	&QAction::triggered,		[=]() { ui->tabWidget->setCurrentWidget(ui->tabCamera); });
	connect(ui->actionTabCalibration,&QAction::triggered,		[=]() { ui->tabWidget->setCurrentWidget(ui->tabCalibration); });

	updateGeometry();
	showMaximized();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::showAbout()
{
	QDialog dialog(this);
	Ui::AboutDialog about;

	about.setupUi(&dialog);
	dialog.exec();
}
