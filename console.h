#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>

#include "debugstream.h"

class Console : public QPlainTextEdit
{
	public:
		Console(QWidget *parent = 0);

		void setup();

		void log(QString msg, QtMsgType type);

	protected:
		DebugStream qcout;
		DebugStream qcerr;

		QString format(QString msg, QtMsgType type);
};

#endif // CONSOLE_H
