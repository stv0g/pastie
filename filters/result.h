#ifndef RESULT_H
#define RESULT_H

#include <opencv2/core.hpp>

#include <QTransform>
#include <QString>

using namespace cv;

class Painter;

class Result
{
	public:
		Result(QTransform t = QTransform()) :
			transform(t)
		{ }

		virtual ~Result() { };

		QTransform getTransform() { return transform; }

		virtual QString getResult() const { return ""; };
		virtual void drawResult(Painter *) const { };

	protected:
		QTransform transform;
};

class IntegerResult : public Result
{
	public:
		IntegerResult(QString n, int v, QTransform t = QTransform()) :
			Result(t),
			name(n),
			value(v)
		 { }

		QString getResult() const {
			return QString("%1 = %2").arg(name).arg(value);
		}

	protected:
		QString name;
		int value;
};

class DoubleResult : public Result
{
	public:
		DoubleResult(QString n, double v, QTransform t = QTransform()) :
			Result(t),
			name(n),
			value(v)
		 { }

		QString getResult() const {
			return QString("%1 = %2").arg(name).arg(value);
		}

	protected:
		QString name;
		double value;
};

#endif // RESULT_H
