#ifndef TABFILTERS_H
#define TABFILTERS_H

#include <QWidget>

#include "filterlist.h"

namespace Ui {
class TabFilters;
}

class TabFilters : public QWidget
{
		Q_OBJECT

	public:
		explicit TabFilters(QWidget *parent = 0);
		~TabFilters();

	protected slots:
		void resizeTable(int, int);

	private:
		Ui::TabFilters *ui;
};

#endif // TABFILTERS_H
