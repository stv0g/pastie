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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>

#include <QMainWindow>
#include <QSignalMapper>

#include "tabcalibration.h"
#include "tabcamera.h"
#include "tabfilters.h"
#include "tabimages.h"

#include "filterlist.h"
#include "imagelist.h"
#include "camera.h"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

		/* Getter */
		Image * getCurrentImage() { return currentImage; }

	public slots:
		void showAbout();
		void showImage(Image *img);
		void showFrame(Image *img);
		void render();

	protected:
		Ui::MainWindow *ui;

		Image *currentImage;
};

#endif // MAINWINDOW_H
