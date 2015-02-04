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

#include <QLayout>
#include <QtDebug>
#include <QLabel>

#include "setting.h"
#include "viewer.h"
#include "filter.h"

Setting::Setting(Filter *f, QString tip) :
	filter(f),
	toolTip(tip)
{
	connect(this, &Setting::valueChanged, [&]() {
		filter->settingChanged(this);
	});
}

QWidget * Setting::getWidget(QWidget *parent)
{
	if (!widget)
		widget = createWidget(parent);

	return widget;
}

QWidget * BooleanSetting::createWidget(QWidget *parent)
{
	box = new QCheckBox(parent);

	connect(box, static_cast<void (QCheckBox::*)(bool)>(&QCheckBox::clicked), [&](bool v) {
		value = v;
		emit valueChanged();
	});

	return box;
}

QWidget * IntegerSetting::createWidget(QWidget *parent)
{
	QWidget *widget = new QWidget(parent);
	QHBoxLayout *layout = new QHBoxLayout(widget);

	slider = new QSlider(Qt::Horizontal, widget);
	spinner = new QSpinBox(widget);

	widget->setToolTip(toolTip);
	layout->setMargin(0);
	layout->addWidget(slider);
	layout->addWidget(spinner);
	spinner->setRange(limits.min, limits.max);
	slider->setRange(limits.min, limits.max);
	spinner->setValue(value);
	slider->setValue(value);
	slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	slider->setTracking(false);

	connect(slider, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), [&](int v) {
		value = v;
		emit  valueChanged();
	});
	connect(slider, static_cast<void (QSlider::*)(int)>(&QSlider::sliderMoved), spinner, &QSpinBox::setValue);
	connect(spinner, static_cast<void (QSpinBox::*)()>(&QSpinBox::editingFinished), [&]() {
		value = spinner->value();
		slider->setValue(value);
		emit valueChanged();
	});

	return widget;
}

double DoubleSetting::getSliderValue(int v)
{
	double value = ((double) v / slider->maximum());
	return value * (limits.max - limits.min) + limits.min;
}

void DoubleSetting::setSliderValue(double value)
{
	slider->setValue(slider->maximum() * (value - limits.min) / (limits.max - limits.min));
}

QWidget * DoubleSetting::createWidget(QWidget *parent)
{
	QWidget *widget = new QWidget(parent);
	QHBoxLayout *layout = new QHBoxLayout(widget);

	slider = new QSlider(Qt::Horizontal, widget);
	spinner = new QDoubleSpinBox(widget);

	widget->setToolTip(toolTip);
	layout->setMargin(0);
	layout->addWidget(slider);
	layout->addWidget(spinner);
	layout->setMargin(0);
	spinner->setRange(limits.min, limits.max);
	slider->setRange(0, SCALE);
	spinner->setValue(value);
	setSliderValue(value);
	slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	slider->setTracking(false);

	connect(slider, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), [&](int v) {
		value = getSliderValue(v);
		emit  valueChanged();
	});
	connect(slider, static_cast<void (QSlider::*)(int)>(&QSlider::sliderMoved), [&](int v) {
		value = getSliderValue(v);
		spinner->setValue(value);
	});
	connect(spinner, static_cast<void (QDoubleSpinBox::*)()>(&QDoubleSpinBox::editingFinished), [&]() {
			value = spinner->value();
			setSliderValue(value);
			emit valueChanged();
	});

	return widget;
}

QWidget * RangeSetting::createWidget(QWidget *parent)
{
	QWidget *widget = new QWidget(parent);
	QHBoxLayout *layout = new QHBoxLayout(widget);

	spinnerL = new QSpinBox(widget);
	spinnerU = new QSpinBox(widget);
	slider = new RangeSlider(Qt::Horizontal, widget);

	layout->setMargin(0);
	layout->addWidget(spinnerL);
	layout->addWidget(slider);
	layout->addWidget(spinnerU);
	spinnerL->setRange(limits.min, limits.max);
	spinnerU->setRange(limits.min, limits.max);
	slider->setRange(limits.min, limits.max);
	spinnerL->setValue(range.min);
	spinnerU->setValue(range.max);
	slider->setValues(range.min, range.max);
	slider->setTracking(false);

	connect(slider, static_cast<void (RangeSlider::*)(int, int)>(&RangeSlider::positionsChanged), [&](int l, int u) {
		spinnerL->setValue(l);
		spinnerU->setValue(u);
	});
	connect(slider, static_cast<void (RangeSlider::*)(int, int)>(&RangeSlider::valuesChanged), [&](int l, int u) {
		range.min = l;
		range.max = u;
		emit  valueChanged();
	});
	connect(spinnerL, static_cast<void (QSpinBox::*)()>(&QSpinBox::editingFinished), [&]() {
		range.min = spinnerL->value();
		slider->setMinimumValue(range.min);
		emit valueChanged();
	});
	connect(spinnerU, static_cast<void (QSpinBox::*)()>(&QSpinBox::editingFinished), [&]() {
		range.max = spinnerU->value();
		slider->setMaximumValue(range.max);
		emit valueChanged();
	});

	return widget;
}

Range<double> DoubleRangeSetting::getSliderRange(Range<int> r)
{
	Range<double> range = {
		((double) r.min / slider->maximum()) * (limits.max - limits.min) + limits.min,
		((double) r.max / slider->maximum()) * (limits.max - limits.min) + limits.min };

	return range;
}

void DoubleRangeSetting::setSliderRange(Range<double> range)
{
	slider->setMinimumValue(slider->maximum() * (range.min - limits.min) / (limits.max - limits.min));
	slider->setMaximumValue(slider->maximum() * (range.max - limits.min) / (limits.max - limits.min));
}

QWidget * DoubleRangeSetting::createWidget(QWidget *parent)
{
	QWidget *widget = new QWidget(parent);
	QHBoxLayout *layout = new QHBoxLayout(widget);

	spinnerL = new QDoubleSpinBox(widget);
	spinnerU = new QDoubleSpinBox(widget);
	slider = new RangeSlider(Qt::Horizontal, widget);

	layout->setMargin(0);
	layout->addWidget(spinnerL);
	layout->addWidget(slider);
	layout->addWidget(spinnerU);
	spinnerL->setRange(limits.min, limits.max);
	spinnerU->setRange(limits.min, limits.max);
	slider->setRange(0, SCALE);
	spinnerL->setValue(range.min);
	spinnerU->setValue(range.max);
	slider->setTracking(false);
	setSliderRange(range);

	connect(slider, static_cast<void (RangeSlider::*)(int, int)>(&RangeSlider::positionsChanged), [&](int mi, int ma) {
		range = getSliderRange(Range<int>(mi, ma));
		spinnerL->setValue(range.min);
		spinnerU->setValue(range.max);
	});
	connect(slider, static_cast<void (RangeSlider::*)(int, int)>(&RangeSlider::valuesChanged), [&](int mi, int ma) {
		range = getSliderRange(Range<int>(mi, ma));
		emit  valueChanged();
	});
	connect(spinnerL, static_cast<void (QDoubleSpinBox::*)()>(&QDoubleSpinBox::editingFinished), [&]() {
		range.min = spinnerL->value();
		setSliderRange(range);
		emit valueChanged();
	});
	connect(spinnerU, static_cast<void (QDoubleSpinBox::*)()>(&QDoubleSpinBox::editingFinished), [&]() {
		range.max = spinnerU->value();
		setSliderRange(range);
		emit valueChanged();
	});

	return widget;
}

QWidget * EnumSetting::createWidget(QWidget *parent)
{
	QComboBox *combo = new QComboBox(parent);

	for (int key : map.keys())
		combo->addItem(map.value(key), QVariant(key));

	combo->setCurrentText(map[value]);
	combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	connect(combo, &QComboBox::currentTextChanged, [&](QString v) {
		value = map.key(v, map.keys().first());
		emit valueChanged();
	});

	return combo;
}

QWidget * SizeSetting::createWidget(QWidget *parent)
{
	QWidget *widget = new QWidget(parent);
	QHBoxLayout *layout = new QHBoxLayout(widget);
	QLabel *lblX = new QLabel("x", widget);

	spinnerH = new QSpinBox(widget);
	spinnerW = new QSpinBox(widget);

	layout->setMargin(0);
	layout->addWidget(spinnerW);
	layout->addWidget(lblX);
	layout->addWidget(spinnerH);
	spinnerH->setValue(value.height);
	spinnerW->setValue(value.width);
	spinnerH->setRange(limits.min, limits.max);
	spinnerW->setRange(limits.min, limits.max);

	connect(spinnerH, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int v) {
		value.height = v;
		emit valueChanged();
	});
	connect(spinnerW, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int v) {
		value.width = v;
		emit valueChanged();
	});

	return widget;
}
