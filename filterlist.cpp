#include "mainwindow.h"

#include "filterlist.h"

FilterList::FilterList(QObject *parent) :
	QAbstractTableModel(parent),
	selection(this)
{
	connect(&selection, &QItemSelectionModel::currentChanged, [&] (QModelIndex i, QModelIndex) {
		emit filterSelected(at(i.row()));
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
			break;
	}

	emit filtersChanged();

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

	connect(filter, SIGNAL(filterChanged()), this, SIGNAL(filtersChanged()));

	emit filterAdded(filter);
}

void FilterList::execute(Image *img)
{
	img->filtered = img->original.clone();

	try {
		for (auto filter : *this) {
			img->applyFilter(filter);
		}
	} catch (Exception e) {
		qCritical("%s", e.msg.c_str());
	}

	/* Update times and results */
	emit dataChanged(
		createIndex(0, 4),
		createIndex(size(), 4)
	);
}

void FilterList::reset()
{
	for (Filter *filter : *this)
		filter->reset();
}
