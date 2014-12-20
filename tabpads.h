#ifndef TABPADS_H
#define TABPADS_H

#include <QWidget>

namespace Ui {
class TabPads;
}

class TabPads : public QWidget
{
		Q_OBJECT

	public:
		explicit TabPads(QWidget *parent = 0);
		~TabPads();

	private:
		Ui::TabPads *ui;
};

#endif // TABPADS_H
