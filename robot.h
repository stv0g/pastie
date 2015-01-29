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
