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
	connect(ui->inpAccelXY,		SIGNAL(valueChanged()),			this,	SLOT(setAccel()));
	connect(ui->inpAccelZ,		SIGNAL(valueChanged()),			this,	SLOT(setAccel()));
	connect(ui->inpBedSizeX,	SIGNAL(valueChanged()),			this,	SLOT(setBedSize()));
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

			connect(s, SIGNAL(valueChanged()), this, SLOT(updateCalibPoints));

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
