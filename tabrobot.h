#ifndef TABROBOT_H
#define TABROBOT_H

#include <QWidget>

namespace Ui {
class TabRobot;
}

class TabRobot : public QWidget
{
		Q_OBJECT

	public:
		explicit TabRobot(QWidget *parent = 0);
		~TabRobot();

	private:
		Ui::TabRobot *ui;
};

#endif // TABROBOT_H
