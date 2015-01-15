#include <QFileDialog>
#include <QDir>

#include "imagelist.h"
#include "mainwindow.h"

extern MainWindow *mwindow;

ImageList::ImageList(QObject *parent) :
	QAbstractTableModel(parent),
	selection(this)
{
	connect(&selection, &QItemSelectionModel::currentChanged, [&](QModelIndex current, QModelIndex) {
		emit newImage(at(current.row()));
	});
}

ImageList::~ImageList()
{
	clear();
}

int ImageList::rowCount(const QModelIndex &) const {
	return count();
}

int ImageList::columnCount(const QModelIndex &) const {
	return 6;
}

QVariant ImageList::data(const QModelIndex &index, int role) const
{
	Image *img = at(index.row());
	QFileInfo fi = QFileInfo(img->getPath());

	if (role == Qt::DisplayRole) {
		switch (index.column()) {
			case 0: return fi.baseName();
			case 1: return fi.suffix().toLower();
			case 2: return QString("%1 kB").arg(fi.size() / 1024.0, 0, 'f', 2);
			case 3: if (img->isLoaded()) return img->getSourceMat().channels();
			case 4: if (img->isLoaded()) return img->getSourceMat().cols;
			case 5: if (img->isLoaded()) return img->getSourceMat().rows;
		}
	}
	else if (role ==Qt::TextAlignmentRole) {
		switch (index.column()) {
			case 4:  return Qt::AlignRight;
			default: return Qt::AlignLeft;
		}
	}

	return QVariant();
}

QVariant ImageList::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			switch (section) {
				case 0: return QString(tr("Name"));
				case 1: return QString(tr("Type"));
				case 2: return QString(tr("Size"));
				case 3: return QString(tr("Channels"));
				case 4: return QString(tr("Width"));
				case 5: return QString(tr("Height"));
			}
		}
	}

	return QVariant();
}

void ImageList::clear() {
	QList<Image *>::iterator it;

	beginResetModel();
	for (Image *img : *this) {
		delete img;
		removeOne(img);
	}
	endResetModel();
}

QList<Image *> ImageList::getSelected()
{
	QList<Image *> sel;
	for (QModelIndex index : selection.selectedRows())
		sel.append(at(index.row()));

	return sel;
}

Image * ImageList::getCurrent()
{
	if (!selection.currentIndex().isValid())
		selection.select(createIndex(0, 0), QItemSelectionModel::Current);

	return value(selection.currentIndex().row());
}

void ImageList::nextImage()
{
	QModelIndex oldIdx = selection.currentIndex();
	QModelIndex newIdx = index(oldIdx.row()-1, oldIdx.column());

	if (newIdx.isValid())
		selection.setCurrentIndex(newIdx, QItemSelectionModel::Current);
}

void ImageList::prevImage()
{
	QModelIndex oldIdx = selection.currentIndex();
	QModelIndex newIdx = index(oldIdx.row()+1, oldIdx.column());

	if (newIdx.isValid())
		selection.setCurrentIndex(newIdx, QItemSelectionModel::Current);
}

void ImageList::load(QStringList files)
{
	QStringList valid = { "png", "jpg", "png" };

	for (QString file : files) {
		QFileInfo checkFile(file);
		if (checkFile.exists() && checkFile.isFile() &&
			valid.contains(checkFile.suffix().toLower())) {
			add(new Image(file));
		}
	}
}

void ImageList::save(QString path)
{
	getCurrent()->save(path);
}

void ImageList::loadFilePicker()
{
	QStringList files = QFileDialog::getOpenFileNames(mwindow, tr("Open Image"), QDir::currentPath(),
		tr("Image Files (*.png *.jpg *.bmp)"));
	load(files);
}

void ImageList::saveFilePicker()
{
	QString path = QFileDialog::getSaveFileName(mwindow,
		tr("Save Image"), getCurrent()->getPath(),
		tr("Image Files (*.png *.jpg *.bmp)"));
	save(path);
}

void ImageList::add(Image *img)
{
	beginInsertRows(QModelIndex(), size(), size());
	push_back(img);
	endInsertRows();
}
