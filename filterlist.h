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

		void add(Filter *filter);

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
};

#endif // FILTERLIST_H
