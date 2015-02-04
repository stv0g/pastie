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

#ifndef FILTERLIST_H
#define FILTERLIST_H

#include <QAbstractTableModel>
#include <QItemSelection>

#include "filters/filter.h"

class FilterList :
	public QAbstractTableModel,
	public QList<Filter *>
{
    Q_OBJECT

	public:
		FilterList(QObject *parent = 0);

		int rowCount(const QModelIndex &) const;
		int columnCount(const QModelIndex &) const;

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags(const QModelIndex & index) const;
		bool setData(const QModelIndex &index, const QVariant &value, int role);

		void prepend(Filter *filter);
		void append(Filter *filter);

		Filter * getCurrent();

		QItemSelectionModel selection;

	public slots:
		void reset();
		void update(Filter *);

    signals:
		void filterChanged(Filter *filter = NULL);
		void filterSelected(Filter *filter);
		void filterAdded(Filter *filter);
		void filterRemoved(Filter *filter);

	protected:
		void add(Filter *filter);
};

#endif // FILTERLIST_H
