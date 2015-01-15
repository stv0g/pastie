#ifndef ROBOT_H
#define ROBOT_H

#include <opencv2/core.hpp>

#include <QQueue>

#include <filters/pathplanner.h>

using namespace cv;

class GCode
{
	public:
		GCode(QByteArray cmd);

		GCode & arg(char param, int v);
		GCode & arg(char param, double v);

		operator QByteArray();

	protected:
		QByteArray cmd;
		QList<QByteArray> args;
};

class Robot
{
	public:
		Robot();

		void prolog();
		void epilog();

		void home();
		void calibrate();
		void stop();

		void moveZ(double z);
		void moveXY(Point2f pos);
		void move(Point3f pos);
		void moveDelta(Point3f delta);

		void processPath(PathResult *path);

		void save();

		Point2f map(Point2f);
		Point2f unmap(Point2f);

		/* Getter */
		QByteArray getGCode();

		/* Setting */
		void setCalibPoint();

	protected:
		Point2f calibPoints[4];

		QQueue<GCode> queue;

};

#endif // ROBOT_H
