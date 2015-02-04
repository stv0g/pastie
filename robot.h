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
 * @copyright  2015 Steffen Vogel
 * @license	   http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author     Steffen Vogel <steffen.vogel@rwth-aachen.de>
 * @link       http://www.steffenvogel.de
 */

#ifndef ROBOT_H
#define ROBOT_H

#include <opencv2/core.hpp>

#include <QQueue>
#include <QHostAddress>

#include <filters/pathplanner.h>
#include "filters/pattern.h"

using namespace cv;

class GCode
{
	public:
		GCode(QByteArray cmd);

		GCode & arg(char param, int v);
		GCode & arg(char param, double v);
		GCode & arg(QByteArray arg);

		/* Factories */
		static GCode home();
		static GCode moveZ(double z);
		static GCode moveXY(Point2f pos);
		static GCode move(Point3f pos);
		static GCode calibrate();
		static GCode stop();
		static GCode comment(QString c);

		operator QByteArray();

	protected:
		QByteArray cmd;
		QList<QByteArray> args;
};

class Robot : public QObject
{
		Q_OBJECT

	public:
		enum BedShape {
			BED_CIRCULAR,
			BED_RECTANGULAR
		};

		Robot(Pattern *pa, PathPlanner *pp);

		void prolog();
		void epilog();

		void processPath();

		void moveDelta(Point3f delta);

		/* Transformation between Robot / Image coordinates */
		Point2f map(Point2f);
		Point2f unmap(Point2f);

	public slots:
		/* Setters */
		void setCalibPoints(Point2f p[3]);
		void setPattern(Pattern *p) { pattern = p; }
		void setPath(PathPlanner *p) { planner = p; }

		void setAccel(Point3f a) { accel = a; }
		void setOffsetZ(double o) { zOffset = o; }
		void setSwingZ(double s) { zSwing = s; }

		void setBedShape(enum BedShape s) { bedShape = s; }
		void setBedSize(Size2f s) { bedSize = s; }

		void home();
		void calibrate();
		void stop();
		void start();
		void save(QString fileName);
		void connect(QHostAddress addr, int port, QString key);

		void updateTransformation();

	signals:
		void connected();
		void finished();

	protected:
		Point2f calibPoints[3];
		Mat affine;
		double zOffset;
		double zSwing;
		Point3f accel;

		enum BedShape bedShape;
		Size2f bedSize;

		Pattern *pattern;
		PathPlanner *planner;

		QQueue<GCode> queue;
};

#endif // ROBOT_H
