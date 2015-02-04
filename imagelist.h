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

#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QList>
#include <QAbstractTableModel>
#include <QItemSelectionModel>

class Image;

class ImageList :
		public QAbstractTableModel,
		protected QList<Image *>
{
		Q_OBJECT
	public:
		explicit ImageList(QObject *parent = 0);
		~ImageList();

		void add(Image *img);


		int rowCount(const QModelIndex &) const;
		int columnCount(const QModelIndex &) const;

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

		QItemSelectionModel selection;

		QList<Image *> getSelected();
		Image * getCurrent();

	signals:
		void newImage(Image *img);

	public slots:
		void load(QStringList files);
		void save(QString path);
		void loadFilePicker();
		void saveFilePicker();
		void clear();

		void prevImage();
		void nextImage();
};

#endif // IMAGELIST_H
