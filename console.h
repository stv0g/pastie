#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>

#include "debugstream.h"

class Console : public QPlainTextEdit
{
	public:
		Console(QWidget *parent = 0);

		void setup();

	protected:
		DebugStream qcout;
		DebugStream qcerr;
};

#endif // CONSOLE_H
