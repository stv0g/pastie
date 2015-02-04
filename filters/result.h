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
