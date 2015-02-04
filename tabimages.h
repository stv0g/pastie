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
