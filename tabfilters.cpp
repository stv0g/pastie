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

#include <QtDebug>

#include "tabfilters.h"
#include "ui_tabfilters.h"

extern FilterList *filters;

TabFilters::TabFilters(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabFilters)
{
	ui->setupUi(this);

	ui->tblFilters->setModel(filters);
	ui->tblFilters->setSelectionModel(&filters->selection);
	ui->tblFilters->resizeColumnsToContents();
	ui->tblFilters->resizeRowsToContents();

	for (Filter *filter : *filters)
		ui->wdgSettings->addWidget(filter->getWidget(this));

	connect(ui->tblFilters->verticalHeader(), &QHeaderView::sectionCountChanged, [&]() {
		ui->tblFilters->resizeColumnsToContents();
		ui->tblFilters->resizeRowsToContents();
	});
	connect(filters, &FilterList::filterSelected, [&] (Filter *filter) {
		ui->wdgSettings->setCurrentWidget(filter->getWidget());
		qDebug() << "Set settings widget to " << filter->getName();
	});
	connect(filters, &FilterList::filterAdded,    [&] (Filter *filter) {
		ui->wdgSettings->addWidget(filter->getWidget(this));
	});
}

TabFilters::~TabFilters()
{
	delete ui;
}

void TabFilters::resizeTable(int, int)
{

}
