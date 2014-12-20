#ifndef FILTER_H
#define FILTER_H

#include <string>
#include <opencv2/core.hpp>

#include <QVector>
#include <QPainter>
#include <QWidget>

#include "painter.h"
#include "image.h"
#include "result.h"
#include "setting.h"

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

	signals:
		void filterChanged();

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
