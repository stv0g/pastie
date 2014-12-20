#include <QElapsedTimer>
#include <QFormLayout>

#include "cast.h"
#include "filter.h"

using namespace cv;

Filter::~Filter()
{
	delete widget;
}

QWidget * Filter::getWidget(QWidget *parent)
{
	if (!widget) {
		widget = new QWidget(parent);

		QFormLayout *layout = new QFormLayout(widget);
		layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

		for	(QString key : settings.keys())
			layout->addRow(key, settings[key]->getWidget(widget));
	}

	return widget;
}

Result * Filter::apply(Image *img)
{
	QElapsedTimer timer;

	if (enabled) {
		timer.start();
		result = applyInternal(img); /* Virtual call */
		time = timer.nsecsElapsed();
	}

	return result;
}
