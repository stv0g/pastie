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

	img->results[this] = result;

	emit filterApplied(this);

	return result;
}

void Filter::settingChanged(Setting *)
{
	emit filterChanged(this);
}
