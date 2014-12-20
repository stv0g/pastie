#include <QFileDialog>
#include <QDir>

#include "imagelist.h"
#include "mainwindow.h"

extern MainWindow *mwindow;

ImageList::ImageList(QObject *parent) :
	QAbstractTableModel(parent),
	selection(this)
{
	connect(&selection, &QItemSelectionModel::currentChanged, this, &ImageList::currentImage);
}

ImageList::~ImageList()
{
	clear();
}

int ImageList::rowCount(const QModelIndex &) const {
	return count();
};

int ImageList::columnCount(const QModelIndex &) const {
	return 6;
};

QVariant ImageList::data(const QModelIndex &index, int role) const
{
	Image *img = at(index.row());
	QFileInfo fi = QFileInfo(img->path);

	if (role == Qt::DisplayRole) {
			switch (index.column()) {
				case 0: return fi.baseName();
				case 1: return fi.suffix();
				case 2: return fi.size();
				case 3: return img->original.cols;
				case 4: return img->original.rows;
				case 5: return img->original.channels();
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
				case 1: return QString(tr("Typ"));
				case 2: return QString(tr("Size"));
				case 3: return QString(tr("Width"));
				case 4: return QString(tr("Height"));
				case 5: return QString(tr("Channels"));
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

QList<Image *> ImageList::selectedImages()
{
	QList<Image *> sel;

	for (QModelIndex index : selection.selectedRows())
		sel.append(at(index.row()));

	return sel;
}

Image * ImageList::currentImage()
{
	current = at(selection.currentIndex().row());
	emit newImage(current);
	return current;
}

Image * ImageList::nextImage()
{
	QModelIndex oldIdx = selection.currentIndex();
	QModelIndex newIdx = index(oldIdx.row()-1, oldIdx.column());

	if (newIdx.isValid()) {
		selection.setCurrentIndex(newIdx, QItemSelectionModel::Current);

		current = at(newIdx.row());
		emit newImage(current);
	}

	return current;
}

Image * ImageList::prevImage()
{
	QModelIndex oldIdx = selection.currentIndex();
	QModelIndex newIdx = index(oldIdx.row()+1, oldIdx.column());

	if (newIdx.isValid()) {
		selection.setCurrentIndex(newIdx, QItemSelectionModel::Current);

		current = at(newIdx.row());
		emit newImage(current);
	}

	return current;
}

void ImageList::load(QStringList files)
{
	for (QString file : files)
		add(new Image(file));

	current = first();
	emit newImage(current);
}

void ImageList::save(QString path)
{	
	currentImage()->save(path);
}

void ImageList::loadFilePicker()
{
	QStringList files = QFileDialog::getOpenFileNames(mwindow, tr("Open Image"), QDir::currentPath(),
		tr("Image Files (*.png *.jpg *.bmp)"));
	load(files);
}

void ImageList::saveFilePicker()
{
	QString path = QFileDialog::getSaveFileName(mwindow, tr("Save Image"), current->path,
												tr("Image Files (*.png *.jpg *.bmp)"));
	save(path);
}

void ImageList::add(Image *img)
{
	beginInsertRows(QModelIndex(), size(), size());
	push_back(img);
	endInsertRows();
}
