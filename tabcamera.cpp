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

#include <opencv2/videoio.hpp>

#include <QtDebug>
#include <QComboBox>

#include "mainwindow.h"
#include "camera.h"
#include "source.h"
#include "imagelist.h"

#include "tabcamera.h"
#include "ui_tabcamera.h"

extern Source *source;
extern Camera *cam;
extern ImageList *images;
extern FilterList *filters;

TabCamera::TabCamera(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabCamera)
{
	ui->setupUi(this);
	ui->cmbCamResolution->setCurrentText(
		QString("%1x%2").arg(source->getSize().width)
						.arg(source->getSize().height)
	);
	ui->cmbCamFPS->setCurrentText(
		QString::number(source->getFPS())
	);

	connect(ui->sldCamBrightness,	&QSlider::sliderReleased,		this, &TabCamera::changeSetting);
	connect(ui->sldCamContrast,		&QSlider::sliderReleased,		this, &TabCamera::changeSetting);
	connect(ui->sldCamExposure,		&QSlider::sliderReleased,		this, &TabCamera::changeSetting);
	connect(ui->sldCamFocus,		&QSlider::sliderReleased,		this, &TabCamera::changeSetting);
	connect(ui->sldCamGain,			&QSlider::sliderReleased,		this, &TabCamera::changeSetting);
	connect(ui->sldCamGamma,		&QSlider::sliderReleased,		this, &TabCamera::changeSetting);
	connect(ui->sldCamHue,			&QSlider::sliderReleased,		this, &TabCamera::changeSetting);
	connect(ui->sldCamSharpness,	&QSlider::sliderReleased,		this, &TabCamera::changeSetting);

	connect(ui->cmbCamFPS,			&QComboBox::currentTextChanged,	this, &TabCamera::changeFPS);
	connect(ui->cmbCamResolution,	&QComboBox::currentTextChanged, this, &TabCamera::changeResolution);
	connect(ui->btnSnapshot,		&QPushButton::clicked,			this, &TabCamera::doSnapshot);
	connect(ui->btnPlay,			&QPushButton::clicked,			source,	&Source::play);
}

TabCamera::~TabCamera()
{
	delete ui;
}

void TabCamera::changeResolution(QString resolution)
{
	QStringList wh = resolution.split('x');

	bool success = source->setSize(Size(wh[0].toInt(), wh[1].toInt()));
	if (!success)
		qWarning() << "Camera did not accept new resolution: " << resolution;

	filters->reset();
	cam->reset();
}

void TabCamera::changeFPS(QString fps)
{
	bool success = source->setFPS(fps.toDouble());
	if (!success)
		qWarning() << "Camera did not accept new FPS: " << fps;
}

void TabCamera::changeSetting()
{
	QSlider *sld = qobject_cast<QSlider *>(sender());

	int prop = getCVCap(sld);
	double value = (double) sld->value() / 1000;

	bool success = source->set(prop, value);
	if (!success)
		qWarning() << "Camera did not accept property " << prop << " with value " << value;
}

int TabCamera::getCVCap(QWidget *w)
{
	if		(w == ui->sldCamBrightness) return CAP_PROP_BRIGHTNESS;
	else if	(w == ui->sldCamContrast)	return CAP_PROP_CONTRAST;
	else if	(w == ui->sldCamExposure)	return CAP_PROP_EXPOSURE;
	else if	(w == ui->sldCamFocus)		return CAP_PROP_FOCUS;
	else if	(w == ui->sldCamGain)		return CAP_PROP_GAIN;
	else if	(w == ui->sldCamGamma)		return CAP_PROP_GAMMA;
	else if	(w == ui->sldCamHue)		return CAP_PROP_HUE;
	else if	(w == ui->sldCamSharpness)	return CAP_PROP_SHARPNESS;
	else								return -1;
}

void TabCamera::doSnapshot()
{
	images->add(source->getSnapshot());
}
