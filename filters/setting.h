#ifndef SETTING_H
#define SETTING_H

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QComboBox>
#include <QCheckBox>
#include <QMap>

#include <opencv2/core.hpp>

using namespace cv;

class Filter;

class Setting : public QObject
{
	Q_OBJECT

	public:
		Setting(Filter *f, QString tip);
		QWidget * getWidget(QWidget *parent);

	signals:
		void valueChanged();

	protected:
		virtual QWidget * createWidget(QWidget *parent) = 0;

		Filter *filter;
		QWidget *widget = NULL;
		QString toolTip;
};

class IntegerSetting : public Setting
{
	Q_OBJECT

	public:
		IntegerSetting(Filter *f, int &v, int mi = 0, int ma = 100, QString tip = "") :
			Setting(f, tip), value(v), min(mi), max(ma) { }

	protected:
		QWidget * createWidget(QWidget *parent = 0);

		int &value, min, max;
		QSpinBox *spinner;
		QSlider *slider;
};

class BooleanSetting : public Setting
{
	Q_OBJECT

	public:
		BooleanSetting(Filter *f, bool &v, QString tip = "") :
			Setting(f, tip), value(v) { }

	protected:
		QWidget *createWidget(QWidget *parent);

		bool &value;
		QCheckBox *box;
};

class DoubleSetting : public Setting
{
	Q_OBJECT

	public:
		DoubleSetting(Filter *f, double &v, double mi = 0, double ma = 100, QString tip = "") :
			Setting(f, tip), value(v), min(mi), max(ma) { }

	protected:
		static const int SCALE = 1000;

		QWidget * createWidget(QWidget *parent = 0);

		double &value, min, max;
		QDoubleSpinBox *spinner;
		QSlider *slider;
};

class EnumSetting : public Setting
{
		Q_OBJECT

	public:
		EnumSetting(Filter *f, int &v, QMap<int, QString> m, QString tip = "") :
			Setting(f, tip), value(v), map(m) { }

	protected:
		QWidget * createWidget(QWidget *parent);

		int &value;
		QMap<int, QString> map;
		QComboBox *combo;
};

class SizeSetting : public Setting
{
		Q_OBJECT

	public:
		SizeSetting(Filter *f, Size &v, int mi = 1, int ma = 100, QString tip = "") :
			Setting(f, tip), value(v), min(mi), max(ma) { }

	protected:
		QWidget * createWidget(QWidget *parent);

		Size &value;
		int min, max;

		QSpinBox *spinnerH, *spinnerW;
};

#endif // SETTING_H
