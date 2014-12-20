#include <QLayout>
#include <QtDebug>
#include <QLabel>

#include "ui_doublesetting.h"
#include "setting.h"
#include "viewer.h"
#include "filter.h"

Setting::Setting(Filter *f, QString tip)
	: toolTip(tip)
{
	connect(this, SIGNAL(valueChanged()), f, SIGNAL(filterChanged()));
	connect(this, SIGNAL(valueChanged()), f, SLOT(reset()));
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
	spinner->setValue(value);
	slider->setValue(value);
	slider->setMinimum(min);
	slider->setMaximum(max);
	slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	connect(slider, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), [&](int v) {
		value = v;
		spinner->setValue(v);
		emit  valueChanged();
	});
	connect(spinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int v) {
		value = v;
		slider->setValue(value);
		emit valueChanged();
	});

	return widget;
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
	spinner->setValue(value);
	slider->setValue(value * SCALE);
	slider->setMinimum(min * SCALE);
	slider->setMaximum(max * SCALE);
	slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	connect(slider, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), [&](int v) {
		value = (double) v / SCALE;
		spinner->setValue(value);
		emit  valueChanged();
	});
	connect(spinner, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double v) {
		value = v;
		slider->setValue(value * SCALE);
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
	spinnerH->setMinimum(min);
	spinnerH->setMaximum(max);
	spinnerW->setMinimum(min);
	spinnerW->setMaximum(max);

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
