#ifndef TABROBOT_H
#define TABROBOT_H

#include <QWidget>
#include <QDoubleSpinBox>

namespace Ui {
class TabRobot;
}

class TabRobot : public QWidget
{
		Q_OBJECT

	public:
		explicit TabRobot(QWidget *parent = 0);
		~TabRobot();

	protected slots:
		void saveFilePicker();
		void connectOctoPrint();
		void updateCalibPoints();
		void setAccel();
		void setBedSize();

	protected:
		QDoubleSpinBox *calibWdgs[3][2];

	private:
		Ui::TabRobot *ui;
};

#endif // TABROBOT_H
