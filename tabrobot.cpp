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
 * @copyright  Kitware Inc.
 * @license	   http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author     Steffen Vogel <steffen.vogel@rwth-aachen.de>
 * @link       http://www.steffenvogel.de
 * @package    CTK
 */

#include <QGridLayout>
#include <QFileDialog>

#include <opencv2/core.hpp>

#include "mainwindow.h"
#include "tabrobot.h"
#include "ui_tabrobot.h"
#include "robot.h"

using namespace cv;

extern MainWindow *mwindow;
extern Robot *robot;

TabRobot::TabRobot(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabRobot)
{
	ui->setupUi(this);

	/* Connect signals */
	connect(ui->btnStart,		&QPushButton::clicked,			robot,	&Robot::start);
	connect(ui->btnAbort,		&QPushButton::clicked,			robot,	&Robot::stop);
	connect(ui->btnCalibrate,	&QPushButton::clicked,			robot,	&Robot::calibrate);
	connect(ui->btnHome,		&QPushButton::clicked,			robot,	&Robot::home);
	connect(ui->btnSave,		&QPushButton::clicked,			this,	&TabRobot::saveFilePicker);
	connect(ui->btnConnect,		&QPushButton::clicked,			this,	&TabRobot::connectOctoPrint);
	connect(ui->inpAccelXY,		SIGNAL(valueChanged(double)),	this,	SLOT(setAccel()));
	connect(ui->inpAccelZ,		SIGNAL(valueChanged(double)),	this,	SLOT(setAccel()));
	connect(ui->inpBedSizeX,	SIGNAL(valueChanged(double)),	this,	SLOT(setBedSize()));
	connect(ui->inpSwingZ,		SIGNAL(valueChanged(double)),	robot,	SLOT(setSwingZ(double)));
	connect(ui->inpOffsetZ,		SIGNAL(valueChanged(double)),	robot,	SLOT(setOffsetZ(double)));
	connect(ui->cmbBedShape,	SIGNAL(currentIndexChanged(int)),robot,	SLOT(setBedShape(int)));

	/* Complete Ui */
	QGridLayout *l = qobject_cast<QGridLayout*>(ui->wdgCalibPoints->layout());
	if (!l)
		return;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			QDoubleSpinBox *s = new QDoubleSpinBox(ui->wdgCalibPoints);

			s->setRange(-15, +15);
			l->addWidget(s, j+1, i+2);

			connect(s, SIGNAL(valueChanged(double)), this, SLOT(updateCalibPoints));

			calibWdgs[i][j] = s;
		}
	}
}

void TabRobot::saveFilePicker()
{
	QString path = QFileDialog::getSaveFileName(mwindow,
		tr("Save GCode"), QString(),
		tr("GCode File (*.gco *.gcode *.g *gc)"));

	robot->save(path);
}

void TabRobot::connectOctoPrint()
{
	robot->connect(QHostAddress(ui->inpHost->text()), ui->inpPort->text().toUInt(), ui->inpApiKey->text());
}

void TabRobot::setAccel()
{
	Point3f accel(
		ui->inpAccelXY->value(),
		ui->inpAccelXY->value(),
		ui->inpAccelZ->value()
	);

	robot->setAccel(accel);
}

void TabRobot::setBedSize()
{
	Size2f size(
		ui->inpBedSizeX->value(),
		ui->inpBedSizeY->value()
	);

	robot->setBedSize(size);
}

void TabRobot::updateCalibPoints()
{
	Point2f calibPoints[3];

	for (int i = 0; i < 3; i++) {
			calibPoints[i].x = calibWdgs[i][0]->value();
			calibPoints[i].x = calibWdgs[i][0]->value();
	}

	robot->setCalibPoints(calibPoints);
}

TabRobot::~TabRobot()
{
	delete ui;
}
