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

	return *this;
}

GCode & GCode::arg(char param, double v)
{
	QByteArray arg;

	arg += param;
	arg += QByteArray::number(v);

	args += arg;

	return *this;
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
	return GCode(QString(";" + c).toLatin1());
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
	Mat src(3, 1, CV_32F);

	src.at<float>(0) = p.x;
	src.at<float>(1) = p.y;
	src.at<float>(2) = 1.0;

	Mat dst = affine * src;

	return Point2f(dst.at<float>(0), dst.at<float>(1));
}

Point2f Robot::unmap(Point2f p)
{
	Mat src(3, 1, CV_32F);

	src.at<float>(0) = p.x;
	src.at<float>(1) = p.y;
	src.at<float>(2) = 1.0;

	Mat dst = affine.inv() * src;

	return Point2f(dst.at<float>(0), dst.at<float>(1));
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

