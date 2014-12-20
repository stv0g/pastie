#ifndef RESULT_H
#define RESULT_H

#include <QString>

class Painter;

class Result
{
	public:
		virtual ~Result() { };

		virtual void draw(Painter *) const { };
		virtual QString getResult() const = 0;
};

class IntegerResult : public Result
{
	public:
		IntegerResult(QString n, int v) :
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
		DoubleResult(QString n, double v) :
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
