#ifndef TABIMAGES_H
#define TABIMAGES_H

#include <QWidget>

#include "imagelist.h"

namespace Ui {
class TabImages;
}

class TabImages : public QWidget
{
		Q_OBJECT

	public:
		explicit TabImages(QWidget *parent = 0);
		~TabImages();

	protected slots:
		void resizeTable(int, int);

	private:
		Ui::TabImages *ui;
};

#endif // TABIMAGES_H
