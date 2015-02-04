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
	ui->splitterV->setSizes({
		static_cast<int>(0.75 * ui->viewer->width()),
		static_cast<int>(0.25 * ui->viewer->width())
	});

	/* Connect actions */
	connect(images,					&ImageList::newImage,		this,					&MainWindow::showImage);
	connect(source,					&Source::newFrame,			this,					&MainWindow::showFrame);
	connect(filters,				&FilterList::filterChanged, this,					&MainWindow::render);
	connect(ui->actionRedraw,		&QAction::triggered,		this,					&MainWindow::render);
	connect(ui->actionAbout,		&QAction::triggered,		this,					&MainWindow::showAbout);
	connect(ui->actionExit,			&QAction::triggered,		this,					&MainWindow::close);
	connect(ui->actionReset,		&QAction::triggered,		ui->viewer,				&Viewer::reset);
	connect(ui->actionCalibrate,	&QAction::triggered,		ui->tabCalibration,		&TabCalibration::doCalibration);
	connect(ui->actionSnapshot,		&QAction::triggered,		ui->tabCamera,			&TabCamera::doSnapshot);
	connect(ui->actionClear,		&QAction::triggered,		images,					&ImageList::clear);
	connect(ui->actionLoad,			&QAction::triggered,		images,					&ImageList::loadFilePicker);
	connect(ui->actionSave,			&QAction::triggered,		images,					&ImageList::saveFilePicker);
	connect(ui->actionNextImage,	&QAction::triggered,		images,					&ImageList::nextImage);
	connect(ui->actionPrevImage,	&QAction::triggered,		images,					&ImageList::prevImage);
	connect(ui->actionReset,		&QAction::triggered,		filters,				&FilterList::reset);
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

void MainWindow::showImage(Image *i)
{
	source->play(false);

	showFrame(i);
}

void MainWindow::showFrame(Image *i)
{
	currentImage = i;

	render();
}

void MainWindow::render()
{
	currentImage->getMat().release();

	try {
		for (Filter *filter : *filters)
			filter->apply(currentImage);
	} catch (Exception e) {
		qCritical("%s", e.what());
	}

	qDebug() << "Rendering completed";

	ui->viewer->showImage(currentImage);
}
