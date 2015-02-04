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

#ifndef FILTER_H
#define FILTER_H

#include <string>
#include <opencv2/core.hpp>

#include <QVector>
#include <QPainter>
#include <QWidget>
#include <QMouseEvent>

#include "painter.h"
#include "image.h"
#include "result.h"
#include "setting.h"
#include "range.h"

using namespace cv;

class Filter : public QObject
{
	Q_OBJECT

	public:
		virtual ~Filter();

		Result * apply(Image *img);

		QWidget * getWidget(QWidget *parent = 0);

		/* Setter */
		void setEnabled(bool e)				  { enabled = e; }
		void setShow(bool s)				  { show = s; }

		/* Getter */
		bool			isEnabled()		const { return enabled; }
		bool			isShown()		const { return show; }
		long			getTime()		const { return time; }
		Result *		getLastResult() const { return result; }
		virtual QString getName()		const = 0;

	public slots:
		virtual void reset() { }
		virtual void clicked(Point, QMouseEvent *) { }
		virtual void settingChanged(Setting *);

	signals:
		void filterChanged(Filter *);
		void filterApplied(Filter *);

	protected:
		bool enabled = true;
		bool show = true;
		long time = 0; /* in uSecs */

		Result *result = NULL;
		QWidget *widget = NULL;
		QMap<QString, Setting*> settings;
		virtual Result * applyInternal(Image *img) = 0;
};

#endif // FILTER_H
