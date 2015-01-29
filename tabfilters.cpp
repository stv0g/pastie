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
