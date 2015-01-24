#include "robot.h"

GCode::GCode(QByteArray c) :
	cmd(c)
{ }

GCode & GCode::arg(char param, int v)
{
	QByteArray arg;

	arg.push_back(param);
	arg.push_back(QByteArray::number(v));

	args.push_back(arg);
}

GCode & GCode::arg(char param, double v)
{

}

GCode::operator QByteArray()
{
	QByteArray ret;


}

/********************************************/

Robot::Robot()
{

}

void Robot::prolog()
{
	home();
	calibrate();
}

void Robot::epilog()
{
	home();
}

void Robot::home()
{
	queue << GCode("G28");
}

void Robot::calibrate()
{
	queue << GCode("G29");
}

void Robot::stop()
{
	queue << GCode("M124");
}

void Robot::moveZ(double z)
{
	queue << GCode("G0")
				.arg('Z', z);
}

void Robot::moveXY(Point2f pos)
{
	queue << GCode("G0")
				.arg('X', pos.x)
				.arg('Y', pos.y);
}

void Robot::move(Point3f pos)
{
	queue << GCode("G0")
				.arg('X', pos.x)
				.arg('Y', pos.y)
				.arg('Z', pos.z);
}

void Robot::moveDelta(Point3f delta)
{
	queue << GCode("G91");
	move(delta);
	queue << GCode("G90");
}

void Robot::processPath(PathResult *path)
{
	queue.clear();

	prolog();

	for (Pad pad : *path) {
		moveZ(5.0);
		moveXY(pad.center);
		moveZ(0.0);
	}

	epilog();
}
