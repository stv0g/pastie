#include <vector>

#include "robot.h"
#include "mainwindow.h"

extern MainWindow *mwindow;

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
	QByteArray arg;

	arg += param;
	arg += QByteArray::number(v);

	args += arg;
}

GCode::operator QByteArray()
{
	QByteArray ret;

	ret += cmd;

	if (args.size()) {
		ret += " ";

		for (QByteArray arg : args) {
			ret += arg;

			if (arg != args.last())
				ret += " ";
		}
	}

	return ret;
}

/************ GCode Factories ***************/

GCode GCode::moveZ(double z)
{
	return GCode("G0")
				.arg('Z', z);
}

GCode GCode::moveXY(Point2f pos)
{
	return GCode("G0")
				.arg('X', pos.x)
				.arg('Y', pos.y);
}

GCode GCode::move(Point3f pos)
{
	return GCode("G0")
				.arg('X', pos.x)
				.arg('Y', pos.y)
				.arg('Z', pos.z);
}

GCode GCode::home()
{
	return GCode("G28");
}

GCode GCode::calibrate()
{
	return GCode("G29");
}

GCode GCode::stop()
{
	return GCode("M124");
}

GCode GCode::comment(QString c)
{
//	return GCode(";" + c);
}

/****************** Robot *******************/

Robot::Robot(Pattern *pa, PathPlanner *pp) :
	pattern(pa),
	planner(pp)
{ }

void Robot::connect(QHostAddress addr, int port, QString key)
{
	Q_UNUSED(addr)
	Q_UNUSED(port)
	Q_UNUSED(key)
}

void Robot::save(QString fileName)
{
	QFile file(fileName);
	file.open(QFile::WriteOnly);
	if (!file.isWritable()) {
		qCritical() << "Failed to open file: " << fileName;
		return;
	}

	processPath();

	while (!queue.isEmpty()) {
		QByteArray gco = queue.dequeue();
		file.write(gco);
		qDebug() << "GCode: " << gco;
	}

	file.close();
}

void Robot::home()
{
	// FIXME: send GCode to OctoPrint
}

void Robot::calibrate()
{
	// FIXME: send GCode to OctoPrint
}

void Robot::start()
{
	// FIXME: send GCode to OctoPrint
}

void Robot::stop()
{
	// FIXME: send GCode to OctoPrint
}

void Robot::moveDelta(Point3f delta)
{
	queue << GCode("G91");
	queue << GCode::move(delta);
	queue << GCode("G90");
}

void Robot::prolog()
{
	queue << GCode::home();
	queue << GCode::calibrate();
}

void Robot::epilog()
{
	queue << GCode::home();
}

void Robot::processPath()
{
	Image *img = mwindow->getCurrentImage();
	if (!img)
		return;

	PathResult *path = dynamic_cast<PathResult*>(img->getResult(planner));
	if (!path)
		return;

	queue.clear();

	prolog();

	int i = 0;
	double distance = 0;
	double time = 0;

	for (Pad pad : *path) {
		Point2f p = map(pad.center);

		/* Check Bed Bounds */
		if (bedShape == BED_RECTANGULAR) {
			if (p.x > bedSize.width ||
				p.y > bedSize.height ||
				p.x < 0 ||
				p.y < 0
			) {
				qCritical() << "Pad outside of bed: " << toQt(p);
				return;
			}
		}
		else if (bedShape == BED_CIRCULAR) {
			if (pow(p.x / bedSize.width, 2) + pow(p.y / bedSize.height, 2) > 1) {
				qCritical() << "Pad outside of bed: " << toQt(p);
				return;
			}
		}

		queue << GCode::comment(QString("Pad %1: x=%2, y=%3").arg(i).arg(pad.center.x).arg(pad.center.y));
		queue << GCode::moveZ(zOffset + zSwing);
		queue << GCode::moveXY(p);
		queue << GCode::moveZ(zOffset);

		i++;
	}

	epilog();

	qDebug() << "Finished G-Code processing: distance = " << distance << "time = " << time;
}

Point2f Robot::map(Point2f p)
{
	//Point2f a = affine * Point3f(p.x, p.y, 1);

	//return Point3f(a.x, a.y, zOffset);
}

Point2f Robot::unmap(Point2f p)
{
	//return affine.inv() * Point2f(p.x, p.y);
}

void Robot::updateTransformation()
{
	std::vector<Point2f> src(calibPoints, calibPoints+3);
	std::vector<Point2f> dst;

	Image *img = mwindow->getCurrentImage();
	if (!img)
		return;

	PatternResult *pat = dynamic_cast<PatternResult*>(img->getResult(pattern));
	if (!pat)
		return;

	dst = pat->getPoints();

	qDebug() << "Mapping: " << toQt(src[0]) << "=>" << toQt(dst[0]) << ", "
							<< toQt(src[1]) << "=>" << toQt(dst[1]) << ", "
							<< toQt(src[2]) << "=>" << toQt(dst[2]);

	affine = getAffineTransform(src, dst);

	qDebug() << "Affine: " << toQTransform(affine);
}

void Robot::setCalibPoints(Point2f p[3])
{
	for (int i = 0; i < 3; i++)
		calibPoints[i] = p[i];

	updateTransformation();
}

