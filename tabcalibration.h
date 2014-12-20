#ifndef TABCALIBRATION_H
#define TABCALIBRATION_H

#include <QWidget>

#include "camera.h"

namespace Ui {
class TabCalibration;
}

class TabCalibration : public QWidget
{
		Q_OBJECT

	public:
		TabCalibration(QWidget *parent = 0);
		~TabCalibration();

	public slots:
		void doCalibration();
		void doPerspective();
		void resetCalibration();
		void showResults();

	private:
		Ui::TabCalibration *ui;
};

#endif // TABCALIBRATION_H
