#ifndef TABCAMERA_H
#define TABCAMERA_H

#include <QWidget>

#include "camera.h"

namespace Ui {
class TabCamera;
}

class TabCamera : public QWidget
{
		Q_OBJECT

	public:
		TabCamera(QWidget *parent = 0);
		~TabCamera();

	public slots:
		void doSnapshot();

	protected slots:
		void changeResolution(QString resolution);
		void changeFPS(QString fps);
		void changeSetting();

	protected:
		int getCVCap(QWidget *w);

	private:
		Ui::TabCamera *ui;
};

#endif // TABCAMERA_H
