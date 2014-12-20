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
	ui->tblImages->resizeColumnsToContents();
	ui->tblImages->setColumnWidth(0, 400);

	connect(ui->tblImages->verticalHeader(), SIGNAL(sectionCountChanged(int,int)),	this, SLOT(resizeTable(int,int)));
	connect(ui->btnClear, &QPushButton::clicked, images, &ImageList::clear);
	connect(ui->btnLoad,  &QPushButton::clicked, images, &ImageList::loadFilePicker);
	connect(ui->btnSave,  &QPushButton::clicked, images, &ImageList::saveFilePicker);
}

TabImages::~TabImages()
{
	delete ui;
}

void TabImages::resizeTable(int, int)
{
	ui->tblImages->resizeColumnsToContents();
	ui->tblImages->resizeRowsToContents();
}
