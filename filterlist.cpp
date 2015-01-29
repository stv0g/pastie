#include "mainwindow.h"

#include "filterlist.h"

FilterList::FilterList(QObject *parent) :
	QAbstractTableModel(parent),
	selection(this)
{
	connect(&selection, &QItemSelectionModel::currentChanged, [&] (QModelIndex current, QModelIndex) {
		emit filterSelected(at(current.row()));
	});
}

int FilterList::rowCount(const QModelIndex &) const
{
	return size();
}

int FilterList::columnCount(const QModelIndex &) const
{
	return 5;
}

QVariant FilterList::data(const QModelIndex &index, int role) const
{
	Filter *filter = at(index.row());
	Result *result = filter->getLastResult();

	switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:
			switch (index.column()) {
				case 2: return filter->getName();
				case 3: return (filter->isEnabled() && result) ? result->getResult() : QVariant();
				case 4: return (filter->isEnabled()) ? QString::number(filter->getTime() * 1e-6, 'g', 3) : QVariant();
			}
			break;

		case Qt::CheckStateRole:
			switch (index.column()) {
				case 0: return (filter->isEnabled()) ? Qt::Checked: Qt::Unchecked;
				case 1: return (filter->isShown())	 ? Qt::Checked: Qt::Unchecked;
            }
			break;
	}

	return QVariant();
}

bool FilterList::setData(const QModelIndex &index, const QVariant &value, int role)
{
	Filter *filter = at(index.row());

	switch (role) {
		case Qt::CheckStateRole:
			switch (index.column()) {
				case 0: filter->setEnabled(value == Qt::Checked);
				case 1: filter->setShow(value == Qt::Checked); break;
			}

			emit filterChanged(filter);
			break;
	}

	return true;
}

QVariant FilterList::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			switch (section) {
				case 0: return QString(tr("Enabled"));
                case 1: return QString(tr("Show"));
                case 2: return QString(tr("Filter"));
				case 3: return QString(tr("Result"));
				case 4: return QString(tr("Time [mSec]"));
			}
		}
	}

	return QVariant();
}

Qt::ItemFlags FilterList::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = Qt::ItemIsEnabled;

	switch (index.column()) {
        case 0:
        case 1: flags |= Qt::ItemIsUserCheckable; break;
        case 2: flags |= Qt::ItemIsSelectable; break;
	}

	return flags;
}

void FilterList::add(Filter *filter)
{
	beginInsertRows(QModelIndex(), size(), size());
	push_back(filter);
	endInsertRows();

	connect(filter, SIGNAL(filterChanged(Filter *)), this, SIGNAL(filterChanged(Filter *)));
	connect(filter, SIGNAL(filterApplied(Filter*)),  this, SLOT(update(Filter *)));

	emit filterAdded(filter);
}

void FilterList::reset()
{
	qDebug() << "Reset filters";
	for (Filter *filter : *this)
		filter->reset();

	emit filterChanged();
}

void FilterList::update(Filter *f)
{
	emit dataChanged(
		createIndex(indexOf(f),   0),
		createIndex(indexOf(f)+1, 5)
	);
}

Filter * FilterList::getCurrent()
{
	return value(selection.currentIndex().row());
}
