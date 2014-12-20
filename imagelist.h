#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QList>
#include <QAbstractTableModel>
#include <QItemSelectionModel>

#include "image.h"

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

	signals:
		void newImage(Image *img);

	public slots:
		void load(QStringList files);
		void save(QString path);
		void loadFilePicker();
		void saveFilePicker();
		void clear();

		QList<Image *> selectedImages();
		Image * currentImage();
		Image * prevImage();
		Image * nextImage();

	protected:
		Image *current;
};

#endif // IMAGELIST_H
