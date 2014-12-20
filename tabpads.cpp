#include "tabpads.h"
#include "ui_tabpads.h"

TabPads::TabPads(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabPads)
{
	ui->setupUi(this);
}

TabPads::~TabPads()
{
	delete ui;
}
