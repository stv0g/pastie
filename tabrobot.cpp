#include "tabrobot.h"
#include "ui_tabrobot.h"

TabRobot::TabRobot(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabRobot)
{
	ui->setupUi(this);
}

TabRobot::~TabRobot()
{
	delete ui;
}
