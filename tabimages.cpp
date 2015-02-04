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
 * @copyright  Kitware Inc.
 * @license	   http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author     Steffen Vogel <steffen.vogel@rwth-aachen.de>
 * @link       http://www.steffenvogel.de
 * @package    CTK
 */

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
