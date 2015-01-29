#include "mainwindow.h"
#include "tabimages.h"
#include "ui_tabimages.h"

extern MainWindow *mwindow;
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

	connect(ui->tblImages->verticalHeader(), &QHeaderView::sectionCountChanged, [&]() {
		ui->tblImages->resizeColumnsToContents();
		ui->tblImages->resizeRowsToContents();
	});

	connect(ui->btnClear, &QPushButton::clicked, images, &ImageList::clear);
	connect(ui->btnLoad,  &QPushButton::clicked, images, &ImageList::loadFilePicker);
	connect(ui->btnSave,  &QPushButton::clicked, images, &ImageList::saveFilePicker);
}

TabImages::~TabImages()
{
	delete ui;
}
