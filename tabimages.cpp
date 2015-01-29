#include <QMimeData>

#include "tabimages.h"
#include "ui_tabimages.h"

extern ImageList *images;

TabImages::TabImages(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabImages)
{
	ui->setupUi(this);

	ui->tblImages->setModel(images);
	ui->tblImages->setSelectionModel(&images->selection);
	ui->tblImages->setColumnWidth(0, 200);
	ui->tblImages->resizeColumnsToContents();
	ui->tblImages->resizeRowsToContents();

	setAcceptDrops(true);

	connect(ui->tblImages->verticalHeader(), &QHeaderView::sectionCountChanged, [&]() {
		ui->tblImages->resizeColumnsToContents();
		ui->tblImages->resizeRowsToContents();
	});

	connect(ui->btnClear, &QPushButton::clicked, images, &ImageList::clear);
	connect(ui->btnLoad,  &QPushButton::clicked, images, &ImageList::loadFilePicker);
	connect(ui->btnSave,  &QPushButton::clicked, images, &ImageList::saveFilePicker);
}

void TabImages::dragEnterEvent(QDragEnterEvent *dee)
{
	dee->acceptProposedAction();
}

void TabImages::dropEvent(QDropEvent *de)
{
	const QMimeData* mimeData = de->mimeData();

	// check for our needed mime type, here a file or a list of files
	if (mimeData->hasUrls())
	{
		QStringList pathList;
		QList<QUrl> urlList = mimeData->urls();

		// extract the local paths of the files
		for (QUrl url : urlList)
			pathList += url.toLocalFile();

		// call a function to open the files
		images->load(pathList);
	}
}

TabImages::~TabImages()
{
	delete ui;
}
