#ifndef TABIMAGES_H
#define TABIMAGES_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>

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

	protected:
		void dropEvent(QDropEvent *de);
		void dragEnterEvent(QDragEnterEvent *dee);

	private:
		Ui::TabImages *ui;
};

#endif // TABIMAGES_H
