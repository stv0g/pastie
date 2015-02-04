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

#include <opencv2/core.hpp>

#include "filters/pattern.h"
#include "filters/undistort.h"
#include "mainwindow.h"
#include "imagelist.h"
#include "tabcalibration.h"
#include "ui_tabcalibration.h"

extern Camera *cam;
extern ImageList *images;
extern FilterList *filters;
extern MainWindow *mwindow;

TabCalibration::TabCalibration(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabCalibration)
{
	ui->setupUi(this);

	connect(ui->btnCalibrate, &QPushButton::clicked, this, &TabCalibration::doCalibration);
	connect(ui->btnPerspective, &QPushButton::clicked, this, &TabCalibration::doPerspective);
	connect(ui->btnReset,	  &QPushButton::clicked, this, &TabCalibration::resetCalibration);
}

TabCalibration::~TabCalibration()
{
	delete ui;
}

void TabCalibration::doPerspective()
{

}

void TabCalibration::doCalibration()
{
	int index =		ui->cmbBoardPattern->currentIndex();
	float spacing = ui->spinBoardSpacing->value();

	Size size = Size(ui->spinBoardSizeX->value(), ui->spinBoardSizeY->value());
	Pattern *pattern = new Pattern(size, spacing, Pattern::Type(index));
	Undistort *undistort = new Undistort(cam);

	if (cam->calibrate(images->getSelected(), pattern)) {
		filters->prepend(pattern);
		filters->prepend(undistort);
		showResults();
	}
	else {
		delete pattern;
		delete undistort;
	}
}

void TabCalibration::resetCalibration()
{
	filters->reset();
	cam->reset();

	showResults();
}

void TabCalibration::showResults()
{
	/* Reprojection Error */
	ui->lblAvgReprErr->setText(QString::number(cam->getAvgReprErr()));

	/* Distortion coefficients */
	Mat mat = cam->getDistCoeffs();
	ui->lblDistCoeff_0->setText(QString::number(mat.at<float>(0), 'g', 4));
	ui->lblDistCoeff_1->setText(QString::number(mat.at<float>(1), 'g', 4));
	ui->lblDistCoeff_2->setText(QString::number(mat.at<float>(2), 'g', 4));
	ui->lblDistCoeff_3->setText(QString::number(mat.at<float>(3), 'g', 4));
	ui->lblDistCoeff_4->setText(QString::number(mat.at<float>(4), 'g', 4));

	/* Camera Matrix */
	mat = cam->getMatrix();
	ui->lblMatrix_0_0->setText(QString::number(mat.at<float>(0, 0), 'g', 4));
	ui->lblMatrix_0_1->setText(QString::number(mat.at<float>(0, 1), 'g', 4));
	ui->lblMatrix_0_2->setText(QString::number(mat.at<float>(0, 2), 'g', 4));
	ui->lblMatrix_1_0->setText(QString::number(mat.at<float>(1, 0), 'g', 4));
	ui->lblMatrix_1_1->setText(QString::number(mat.at<float>(1, 1), 'g', 4));
	ui->lblMatrix_1_2->setText(QString::number(mat.at<float>(1, 2), 'g', 4));
	ui->lblMatrix_2_0->setText(QString::number(mat.at<float>(2, 0), 'g', 4));
	ui->lblMatrix_2_1->setText(QString::number(mat.at<float>(2, 1), 'g', 4));
	ui->lblMatrix_2_2->setText(QString::number(mat.at<float>(2, 2), 'g', 4));
}
